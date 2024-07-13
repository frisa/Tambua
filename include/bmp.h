#pragma once

#include <vector>
#include <cstdint>

std::vector<uint8_t> decode_bmp(const uint8_t *input, int row_size, int width, int height, int channels, bool top_down);
std::vector<uint8_t> read_bmp(const char* filename, int* width, int* height, int* channels);
void resize_bmp(uint8_t* out, uint8_t* in, int image_height, int image_width,
            int image_channels, int wanted_height, int wanted_width,
            int wanted_channels);