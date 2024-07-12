#include <iostream>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/string_util.h"

int main() {

    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("model.tflite");
    std::cout << "Loading model..." << std::endl;
    if (!model) {
        std::cerr << "Failed to load model." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Successfully loaded model." << std::endl;
    }

    // Build an interpreter to run the model with.
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*model, resolver);
    std::unique_ptr<tflite::Interpreter> interpreter;
    builder(&interpreter);

    if (!interpreter) {
        std::cerr << "Failed to build interpreter." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Successfully built interpreter." << std::endl;
    }
    // Allocate memory for input and output tensors
    interpreter->AllocateTensors();

    // Get input and output tensors.
    TfLiteTensor* input_tensor = interpreter->input_tensor(0);
    TfLiteTensor* output_tensor = interpreter->output_tensor(0);

    // Prepare input data (e.g., for MNIST)
    // Example: create input data for a "Hello, World!" MNIST example
    std::vector<float> input_data = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0};
    memcpy(input_tensor->data.f, input_data.data(), input_data.size() * sizeof(float));

    // Run inference
    interpreter->Invoke();

    // Get the results
    float* output = output_tensor->data.f;
    // Assuming output is a single float result
    float result = output[0];

    // Print the result
    std::cout << "Inference result: " << result << std::endl;

    return 0;
    return 0;
}