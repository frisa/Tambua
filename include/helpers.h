#pragma once
#include <vector>
#include <cstdint>
#include <string>

#define CHECK_TRUE(x) if (!(x)) { \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1); \
    }

#define TFL_OK(x) if (x != kTfLiteOk) { \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1); \
    }

std::vector<uint8_t> decode_bmp(const uint8_t *input, int row_size, int width, int height, int channels, bool top_down);
std::vector<uint8_t> read_bmp(const char* filename, int* width, int* height, int* channels);
void resize_bmp(uint8_t* out, uint8_t* in, int image_height, int image_width,
            int image_channels, int wanted_height, int wanted_width,
            int wanted_channels);

void get_top_n(uint8_t *prediction, int prediction_size, size_t num_results,
               float threshold, std::vector<std::pair<float, int>> *top_results);
void get_label(const std::string &file_name, std::vector<std::string> *result, size_t *found_label_count);