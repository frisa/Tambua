#pragma once

#include <vector>
#include <cstdint>

std::vector<uint8_t> read_bmp(const char* filename, int width, int height);
