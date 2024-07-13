#include "../include/log.h"

void logInterpreterInfo(const tflite::Interpreter &interpreter)
{
    std::cout << "Number of inputs: " << interpreter.inputs().size() << std::endl;
    std::cout << "Number of outputs: " << interpreter.outputs().size() << std::endl;
    std::cout << "Input(0) name: " << interpreter.GetInputName(0) << std::endl;
    std::cout << "Output(0) name: " << interpreter.GetOutputName(0) << std::endl;
    std::cout << "Input(0) type: " << interpreter.tensor(interpreter.inputs()[0])->type << std::endl;
    std::cout << "Output(0) type: " << interpreter.tensor(interpreter.outputs()[0])->type << std::endl;
    std::cout << "Input(0) dims: " << interpreter.tensor(interpreter.inputs()[0])->dims->size << std::endl;
    std::cout << "Output(0) dims: " << interpreter.tensor(interpreter.outputs()[0])->dims->size << std::endl;
    std::cout << "Input(0) dims: ";
    for (int i = 0; i < interpreter.tensor(interpreter.inputs()[0])->dims->size; i++)
    {
        std::cout << interpreter.tensor(interpreter.inputs()[0])->dims->data[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Output(0) dims: ";
    for (int i = 0; i < interpreter.tensor(interpreter.outputs()[0])->dims->size; i++)
    {
        std::cout << interpreter.tensor(interpreter.outputs()[0])->dims->data[i] << " ";
    }
    std::cout << std::endl;
}