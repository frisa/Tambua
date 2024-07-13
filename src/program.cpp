#include "../include/program.h"
#include "../include/log.h"
#include "../include/bmp.h"

#include <iostream>
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/optional_debug_tools.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << "<bitmap file> <model file>" << std::endl;
        return 1;
    }
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(argv[2]);
    CHECK_TRUE(model != nullptr);
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder interpreter_builder(*model, resolver);
    std::unique_ptr<tflite::Interpreter> interpreter;
    TFL_OK(interpreter_builder(&interpreter));
    Log::logInterpreterInfo(interpreter);
    CHECK_TRUE(interpreter != nullptr);

    int width{224}, height{224}, channels{3};
    std::vector<uint8_t> bmp_bytes = read_bmp(argv[1], &width, &height, &channels);
    interpreter->inputs()[0];
    TFL_OK(interpreter->AllocateTensors());
    interpreter.reset();
    return 0;
}