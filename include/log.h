#pragma once

#include <iostream>
#include "tensorflow/lite/interpreter.h"

void logInterpreterInfo(const tflite::Interpreter& interpreter);