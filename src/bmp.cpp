#include "../include/bmp.h"
#include "../include/log.h"
#include <fstream>
#include <iostream>
#include <string>
#include <memory>

#include "tensorflow/lite/builtin_op_data.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"

std::vector<uint8_t> decode_bmp(const uint8_t *input, int row_size, int width, int height, int channels, bool top_down)
{
    std::vector<uint8_t> output(height * width * channels);
    for (int i = 0; i < height; i++)
    {
        int src_pos;
        int dst_pos;

        for (int j = 0; j < width; j++)
        {
            if (!top_down)
            {
                src_pos = ((height - 1 - i) * row_size) + j * channels;
            }
            else
            {
                src_pos = i * row_size + j * channels;
            }

            dst_pos = (i * width + j) * channels;

            switch (channels)
            {
            case 1:
                output[dst_pos] = input[src_pos];
                break;
            case 3:
                // BGR -> RGB
                output[dst_pos] = input[src_pos + 2];
                output[dst_pos + 1] = input[src_pos + 1];
                output[dst_pos + 2] = input[src_pos];
                break;
            case 4:
                // BGRA -> RGBA
                output[dst_pos] = input[src_pos + 2];
                output[dst_pos + 1] = input[src_pos + 1];
                output[dst_pos + 2] = input[src_pos];
                output[dst_pos + 3] = input[src_pos + 3];
                break;
            default:
                std::cout << "Unexpected number of channels: " << channels;
                break;
            }
        }
    }
    return output;
}

std::vector<uint8_t> read_bmp(const char *filename, int *width, int *height, int *channels)
{
    int begin, end;
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return std::vector<uint8_t>();
    }
    begin = file.tellg();
    file.seekg(0, std::ios::end);
    end = file.tellg();
    size_t len = end - begin;
    std::cout << "len: " << len;

    std::vector<uint8_t> img_bytes(len);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(img_bytes.data()), len);

    const int32_t header_size = le32toh(*(reinterpret_cast<const int32_t *>(img_bytes.data() + 10)));
    *width = le32toh(*(reinterpret_cast<const int32_t *>(img_bytes.data() + 18)));
    *height = le32toh(*(reinterpret_cast<const int32_t *>(img_bytes.data() + 22)));
    const int32_t bpp = le32toh(*(reinterpret_cast<const int32_t *>(img_bytes.data() + 28)));
    *channels = bpp / 8;

    std::cout << "width, height, channels: " << *width << ", " << *height << ", " << *channels;

    // there may be padding bytes when the width is not a multiple of 4 bytes
    // 8 * channels == bits per pixel
    const int row_size = (8 * *channels * *width + 31) / 32 * 4;

    // if height is negative, data layout is top down
    // otherwise, it's bottom up
    bool top_down = (*height < 0);

    // Decode image, allocating tensor once the image size is known
    const uint8_t *bmp_pixels = &img_bytes[header_size];
    return decode_bmp(bmp_pixels, row_size, *width, abs(*height), *channels, top_down);
}
void resize_bmp(uint8_t *out, uint8_t *in,
                int image_height, int image_width, int image_channels,
                int wanted_height, int wanted_width, int wanted_channels)
{
    int number_of_pixels = image_height * image_width * image_channels;
    std::unique_ptr<tflite::Interpreter> interpreter(new tflite::Interpreter);
    int base_index = 0;
    interpreter->AddTensors(2, &base_index);
    interpreter->AddTensors(1, &base_index);
    interpreter->SetInputs({0, 1});
    interpreter->SetOutputs({2});

    TfLiteQuantizationParams quant;
    interpreter->SetTensorParametersReadWrite(0, kTfLiteFloat32, "input",
                                              {1, image_height, image_width, image_channels}, quant);
    interpreter->SetTensorParametersReadWrite(1, kTfLiteInt32, "new_size", {2}, quant);
    interpreter->SetTensorParametersReadWrite(2, kTfLiteFloat32, "output",
                                              {1, wanted_height, wanted_width, wanted_channels}, quant);

    tflite::ops::builtin::BuiltinOpResolver resolver;
    const TfLiteRegistration *resize_op = resolver.FindOp(tflite::BuiltinOperator_RESIZE_BILINEAR, 1);
    auto *params = reinterpret_cast<TfLiteResizeBilinearParams *>(malloc(sizeof(TfLiteResizeBilinearParams)));
    params->align_corners = false;
    params->half_pixel_centers = false;
    interpreter->AddNodeWithParameters({0, 1}, {2}, nullptr, 0, params, resize_op, nullptr);
    interpreter->AllocateTensors();
    auto input = interpreter->typed_tensor<float>(0);
    for (int i = 0; i < number_of_pixels; i++)
    {
        input[i] = in[i];
    }
    interpreter->typed_tensor<int>(1)[0] = wanted_height;
    interpreter->typed_tensor<int>(1)[1] = wanted_width;
    interpreter->Invoke();
    auto output = interpreter->typed_tensor<float>(2);
    auto output_number_of_pixels = wanted_height * wanted_width * wanted_channels;
    for (int i = 0; i < output_number_of_pixels; i++)
    {
        out[i] = static_cast<uint8_t>(output[i]);
    }
}