#include "../include/helpers.h"
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/tools/delegates/delegate_provider.h"
#include "tensorflow/lite/tools/evaluation/utils.h"
#include "tensorflow/lite/profiling/profiler.h"

int main(int argc, char *argv[])
{
    // Check the arguments
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << "<bitmap file> <model file> <label file>" << std::endl;
        return 1;
    }
    // Load the model
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(argv[2]);
    CHECK_TRUE(model != nullptr);

    // Build the interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder interpreter_builder(*model, resolver);
    std::unique_ptr<tflite::Interpreter> interpreter;
    TFL_OK(interpreter_builder(&interpreter));
    CHECK_TRUE(interpreter != nullptr);

    // Load the image
    int width{224}, height{224}, channels{3};
    std::vector<uint8_t> bmp_bytes = read_bmp(argv[1], &width, &height, &channels);
    std::cout << "Image size: " << width << "x" << height << "x" << channels << std::endl;

    // Get the input tensor
    int input = interpreter->inputs()[0];
    std::cout << "Input: " << input << std::endl;

    // Get the input tensor dimensions
    const std::vector<int> inputs = interpreter->inputs();
    const std::vector<int> outputs = interpreter->outputs();
    std::cout << "Number of inputs: " << inputs.size() << std::endl;
    std::cout << "Number of outputs: " << outputs.size() << std::endl;

    // Configure delegates
    tflite::tools::ToolParams params;
    tflite::tools::ProvidedDelegateList delegate_list(&params);
    std::vector<tflite::tools::ProvidedDelegateList::ProvidedDelegate> delegates;

    // Add command line flags
    delegate_list.AddAllDelegateParams();
    bool xnnpack_enabled = false;
    bool gpu_enabled = false;
    int num_threads = 1;
    std::vector<tflite::Flag> flags = { 
        tflite::Flag::CreateFlag("use_xnnpack", &xnnpack_enabled, "XNNPACK delegate is enabled"),
        tflite::Flag::CreateFlag("use_gpu", &gpu_enabled, "GPU delegate is enabled"),
        };
    delegate_list.AppendCmdlineFlags(flags);

    // Check configured delegates and setup them
    if (params.HasParam("use_xnnpack"))
    {
        params.Set<bool>("use_xnnpack", true);
        params.Set<int32_t>("num_threads", num_threads);
    }

    if (params.HasParam("use_gpu"))
    {
        params.Set<bool>("use_gpu", true);
    }

    delegates = delegate_list.CreateAllRankedDelegates();
    std::cout << "Number of delegates: " << delegates.size() << std::endl;
    for (auto &delegate : delegates)
    {
        const auto name = delegate.provider->GetName();
        std::cout << "Delegate: " << name << ", status: ";
        if (kTfLiteOk == interpreter->ModifyGraphWithDelegate(std::move(delegate.delegate)))
        {
            std::cout << " added" << std::endl;
        }
        else
        {
            std::cerr << " not added" << std::endl;
        }
    }

    // Allocate tensors
    TFL_OK(interpreter->AllocateTensors());

    // Fill the input tensor
    TfLiteIntArray *dims = interpreter->tensor(input)->dims;
    int wanted_height = dims->data[1];
    int wanted_width = dims->data[2];
    int wanted_channels = dims->data[3];
    TfLiteType wanted_type = interpreter->tensor(input)->type;
    resize_bmp(interpreter->typed_tensor<uint8_t>(input), bmp_bytes.data(),
               height, width, channels, wanted_height, wanted_width, wanted_channels);

    // Set profiling
    int max_profiling_buffer_entries = 1024;
    std::unique_ptr<tflite::profiling::Profiler> profiler = std::make_unique<tflite::profiling::Profiler>(max_profiling_buffer_entries);
    interpreter->SetProfiler(profiler.get());

    // Run the interpreter
    int loop_count = 1;
    int number_of_warmup_runs = 2;
    std::cout << "Running classification ..." << std::endl;
    profiler->StartProfiling();

    for (int i = 0; i < number_of_warmup_runs; i++)
    {
        if (interpreter->Invoke() != kTfLiteOk)
        {
            std::cerr << "Failed to invoke tflite!" << std::endl;
            return 1;
        }
        else
        {
            std::cout << "WarmUp[" << i << "]" << std::endl;
        }
    }

    struct timeval start_time, stop_time;
    gettimeofday(&start_time, nullptr);

    for (int i = 0; i < loop_count; i++)
    {
        if (interpreter->Invoke() != kTfLiteOk)
        {
            std::cerr << "Failed to invoke tflite!" << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Invoked[" << i << "]" << std::endl;
        }
    }

    gettimeofday(&stop_time, nullptr);
    std::cout << "Invoked - average time: " << (get_us(stop_time) - get_us(start_time)) / (loop_count * 1000) << " ms" << std::endl;
    profiler->StopProfiling();

    // Print profiling information
    auto profile_events = profiler->GetProfileEvents();
    for (int i = 0; i < profile_events.size(); i++)
    {
        auto subgraph_index = profile_events[i]->extra_event_metadata;
        auto op_index = profile_events[i]->event_metadata;
        const auto subgraph = interpreter->subgraph(subgraph_index);
        const auto node_and_registration = subgraph->node_and_registration(op_index);
        const TfLiteRegistration registration = node_and_registration->second;
        PrintProfilingInfo(profile_events[i], subgraph_index, op_index, registration);
    }

    // Get the output tensor
    const float threshold = 0.001f;
    std::vector<std::pair<float, int>> top_results;
    int output = interpreter->outputs()[0];
    int number_of_results = 5;
    TfLiteIntArray *output_dims = interpreter->tensor(output)->dims;
    auto output_size = output_dims->data[output_dims->size - 1];
    get_top_n(interpreter->typed_output_tensor<uint8_t>(0),
              output_size, number_of_results, threshold,
              &top_results);

    // Get the labels
    std::vector<std::string> labels;
    size_t label_count;
    get_label(argv[3], &labels, &label_count);
    for (const auto &result : top_results)
    {
        const float confidence = result.first;
        const int index = result.second;
        std::cout << confidence << ": " << index << " " << labels[index] << std::endl;
    }
    interpreter.reset();
    return 0;
}
