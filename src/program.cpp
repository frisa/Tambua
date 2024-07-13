#include "../include/program.h"
#include "../include/log.h"
#include "../include/bmp.h"
#include <queue>

#include <iostream>
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/tools/delegates/delegate_provider.h"

void get_top_n(uint8_t *prediction, int prediction_size, size_t num_results,
               float threshold, std::vector<std::pair<float, int>> *top_results);

int main(int argc, char *argv[])
{
    // Check the arguments
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << "<bitmap file> <model file>" << std::endl;
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
    Log::logInterpreterInfo(interpreter);

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

    // Get provided delegates based on params
    tflite::tools::ToolParams delegate_params;
    tflite::tools::ProvidedDelegateList delegate_list_util(&delegate_params);
    std::vector<tflite::tools::ProvidedDelegateList::ProvidedDelegate> delegates;

    delegate_list_util.AddAllDelegateParams();
    delegates = delegate_list_util.CreateAllRankedDelegates();
    std::cout << "Number of delegates: " << delegates.size() << std::endl;
    for (auto &delegate : delegates)
    {
        const auto name = delegate.provider->GetName();
        std::cout << "Delegate: " << name << std::endl;
        if (kTfLiteOk == interpreter->ModifyGraphWithDelegate(std::move(delegate.delegate)))
        {
            std::cout << "Delegate " << name << " added" << std::endl;
        }
        else
        {
            std::cerr << "Delegate " << name << " not added" << std::endl;
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

    // Run the interpreter
    interpreter->Invoke();

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
    for (const auto &result : top_results)
    {
        const float confidence = result.first;
        const int index = result.second;
        std::cout << confidence << ": " << index << " " << std::endl;
    }
    interpreter.reset();
    return 0;
}

void get_top_n(uint8_t *prediction, int prediction_size, size_t num_results,
               float threshold, std::vector<std::pair<float, int>> *top_results)
{
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                        std::greater<std::pair<float, int>>>
        top_result_pq;

    const long count = prediction_size;
    float value = 0.0;
    for (int i = 0; i < count; ++i)
    {
        value = prediction[i] / 255.0;
        if (value < threshold)
        {
            continue;
        }
        top_result_pq.push(std::pair<float, int>(value, i));
        if (top_result_pq.size() > num_results)
        {
            top_result_pq.pop();
        }
    }
    while (!top_result_pq.empty())
    {
        top_results->push_back(top_result_pq.top());
        top_result_pq.pop();
    }
    std::reverse(top_results->begin(), top_results->end());
}