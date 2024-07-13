#pragma once

#include <iostream>
#include <memory>
#include "tensorflow/lite/interpreter.h"

class Log
{
public:
    static void logInterpreterInfo(const std::unique_ptr<tflite::Interpreter>& interpreter);
};