#include <iostream>
#include <tensorflow/lite/version.h>
#include <tensorflow/lite/interpreter.h>

int main() {
    std::cout << "Using tensorflow version: " << TFLITE_VERSION_STRING << std::endl;
    std::unique_ptr<tflite::Interpreter> interpreter;
    if (interpreter == nullptr) {
        std::cerr << "Failed to create TensorFlow Lite interpreter." << std::endl;
        return -1;
    }
    return 0;
}