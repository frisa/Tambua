#include "../include/program.h"
#include "../include/log.h"
#include "../include/bmp.h"

#include <iostream>
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/tools/delegates/delegate_provider.h"

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
    
    // Get provided delegates
    tflite::tools::ProvidedDelegateList delegate_list_util;
    std::vector<tflite::tools::ProvidedDelegateList::ProvidedDelegate> delegates;
    //delegates = delegate_list_util.CreateAllRankedDelegates();
    std::cout << "Number of delegates: " << delegates.size() << std::endl;

    TFL_OK(interpreter->AllocateTensors());
    interpreter.reset();
    return 0;
}