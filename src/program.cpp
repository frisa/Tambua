#include <iostream>
#include <tensorflow/lite/version.h>

int main() {
    std::cout << "Using tensorflow version: " << TFLITE_VERSION_STRING << std::endl;
    return 0;
}