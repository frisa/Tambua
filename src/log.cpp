#include "../include/log.h"

void Log::logInterpreterInfo(const std::unique_ptr<tflite::Interpreter>& interpreter)
{
    std::cout << "tensors size: " << interpreter->tensors_size() << std::endl;
    std::cout << "nodes size: " << interpreter->nodes_size() << std::endl;
    std::cout << "inputs: " << interpreter->inputs().size() << std::endl;
    std::cout << "input(0) name: " << interpreter->GetInputName(0) << std::endl;
    int t_size = interpreter->tensors_size();
    for (int i = 0; i < t_size; i++) {
      if (interpreter->tensor(i)->name)
        std::cout << i << ": " << interpreter->tensor(i)->name << ", "
                  << interpreter->tensor(i)->bytes << ", "
                  << interpreter->tensor(i)->type << ", "
                  << interpreter->tensor(i)->params.scale << ", "
                  << interpreter->tensor(i)->params.zero_point << std::endl;
    }
    std::cout << std::endl;
}