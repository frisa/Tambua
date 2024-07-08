#include <iostream>
#include <tensorflow/lite/interpreter.h>

int main() {
    std::unique_ptr<tflite::Interpreter> interpreter;
    if (interpreter == nullptr) {
        std::cerr << "Failed to create TensorFlow Lite interpreter." << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Successfully created TensorFlow Lite interpreter." << std::endl;
        interpreter.reset();
    }
    return 0;
}