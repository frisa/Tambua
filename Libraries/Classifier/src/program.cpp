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

std::string DoClassification(const std::string &imagePath, const std::string &modelPath, const std::string &labelsPath)
{
    // Load the model
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());
    CHECK_TRUE(model != nullptr);

    // Build the interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder interpreter_builder(*model, resolver);
    std::unique_ptr<tflite::Interpreter> interpreter;
    TFL_OK(interpreter_builder(&interpreter));
    CHECK_TRUE(interpreter != nullptr);

    // Load the image
    int width{224}, height{224}, channels{3};
    std::vector<uint8_t> bmp_bytes = read_bmp(imagePath.c_str(), &width, &height, &channels);
    std::cout << "Image size: " << width << "x" << height << "x" << channels << std::endl;

    // Get the input tensor
    int input = interpreter->inputs()[0];
    std::cout << "Input: " << input << std::endl;

    // Get the input tensor dimensions
    const std::vector<int> inputs = interpreter->inputs();
    const std::vector<int> outputs = interpreter->outputs();
    std::cout << "Number of inputs: " << inputs.size() << std::endl;
    std::cout << "Number of outputs: " << outputs.size() << std::endl;

    // Create XNNPACK delegate
    tflite::evaluation::TfLiteDelegatePtr delegate = tflite::evaluation::CreateXNNPACKDelegate(4);
    if (!delegate)
    {
        std::cout << "XNNPACK acceleration is unsupported on this platform.";
    }
    else
    {
        if (interpreter->ModifyGraphWithDelegate(std::move(delegate)) != kTfLiteOk)
        {
            std::cout << "Failed to apply XNNPACK delegate" << std::endl;
        }
        else
        {
            std::cout << "Applied XNNPACK delegate" << std::endl;
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
            return "Failed to invoke tflite!";
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
            return "Failed to invoke tflite!";
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
    get_label(labelsPath.c_str(), &labels, &label_count);
    for (const auto &result : top_results)
    {
        const float confidence = result.first;
        const int index = result.second;
        std::cout << confidence << ": " << index << " " << labels[index] << std::endl;
    }
    interpreter.reset();
    return labels[top_results[0].second];
}
