#include "../include/program.h"
#include "../include/log.h"

#include <iostream>
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/optional_debug_tools.h"

int main(int argc, char* argv[]) {

    if (argc!=2)
    {
        std::cerr << "Usage: " << argv[0] << " <tflite model file>" << std::endl;
        return 1;
    }
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(argv[1]);
    CHECK_TRUE(model!=nullptr);
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder interpreter_builder(*model, resolver);
    std::unique_ptr<tflite::Interpreter> interpreter;
    TFL_OK(interpreter_builder(&interpreter));
    logInterpreterInfo(*interpreter);
    CHECK_TRUE(interpreter!=nullptr);
    return 0;   
}