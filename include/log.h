#pragma once

#include <iostream>
#include "tensorflow/lite/interpreter.h"

class Log
{
public:
    static void logInterpreterInfo(const tflite::Interpreter &interpreter);
};

#define LOG(severity) Log(#severity).Stream()