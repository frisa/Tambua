#include "../include/bmp.h"
#include <fstream>
#include <iostream>
#include <string>

std::vector<uint8_t> read_bmp(const char *filename, int width, int height)
{
    int begin, end;
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    return std::vector<uint8_t>();
}