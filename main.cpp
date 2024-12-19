/* Vlasenko Anastasia Igorevna st128044.student.spbu.ru Labwork1 */

#include "image.h"
#include <cstring>
#include <cerrno>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint>

int main()
{
    std::string fileName;
    std::cout<< "Choose file"<<std::endl;
    std::cin>> fileName;
    std::ifstream img(fileName, std::ios::binary);

    if(!img)
    {
        std::cerr<< "File can't be open!"<<std::endl;
        std::cerr<< "Error:"<< strerror(errno) <<std::endl;
        return -1;
    }

    BMPFILEHEADER fileheader;
    BMPIMAGEHEADER imageheader;
    BMPRGB rgbheader;

    img.read(reinterpret_cast<char*>(&fileheader), sizeof(BMPFILEHEADER));

    if (fileheader.FileType != 0x4D42)
    {
        std::cout<<"Please, choose bmp file"<<std::endl;
        return -1;
    }

    img.read(reinterpret_cast<char*>(&imageheader), sizeof(BMPIMAGEHEADER));

    if (imageheader.BitsPerPixel != 24)
    {
        std::cout<<"Please, choose 24-bit image!"<<std::endl;
        return -1;
    }

    img.read(reinterpret_cast<char*>(&rgbheader), sizeof(BMPRGB));

    int height = imageheader.height;
    int width = imageheader.width;
    size_t head_size = sizeof(BMPFILEHEADER) + sizeof(BMPIMAGEHEADER) + sizeof(BMPRGB);
    size_t row_size = (width * 3 + ((4 - (width*3)) % 4) % 4);
    size_t array_size = row_size * height;

    std::vector<uint8_t>pixels(width*height*3);

    img.seekg(fileheader.Pixelsoffset, std::ios::beg);

    for (int i = height-1; i >= 0; --i)
    {
        for (int j = 0; j < width; ++j)
        {
            for (int g = 0; g<3; ++g)
            {
                img.read(reinterpret_cast<char*>(&pixels[(i * width + j)*3 + g]), 1);
            }
        }
        img.seekg((4 - (width * 3) % 4) % 4, std::ios::cur);
    }
    img.close();

    std::vector<uint8_t>rotatedpixels_r(width*height*3); //rotate clockwise

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            for (int g = 0; g<3; ++g)
            {
                rotatedpixels_r[(i * height + (height - j - 1)) * 3 + g] = pixels[(i * width + j) * 3 + g];
            }
        }
    }

    BMPFILEHEADER fileheader_r = fileheader;
    BMPIMAGEHEADER imageheader_r = imageheader;
    BMPRGB rgbheader_r = rgbheader;

    imageheader_r.height = imageheader.width;
    imageheader_r.width = imageheader.height;

    std::ofstream result_r("clockwise.bmp", std::ios::binary);

    result_r.write(reinterpret_cast<char*>(&fileheader_r), sizeof(BMPFILEHEADER));
    result_r.write(reinterpret_cast<char*>(&imageheader_r), sizeof(BMPIMAGEHEADER));
    result_r.write(reinterpret_cast<char*>(&rgbheader_r), sizeof(BMPRGB));

    for (int i = imageheader_r.height-1; i >= 0; --i)
    {
        for (int j = 0; j < imageheader_r.width; ++j)
        {
            for (int g = 0; g<3; ++g)
            {
                result_r.write(reinterpret_cast<char*>(&rotatedpixels_r[(i * width + j)*3 + g]), 1);
            }
        }
        int padding = (4 - (imageheader_r.width * 3) % 4) % 4;

        if (padding > 0)
        {
            std::vector<uint8_t> pad(padding, 0);
            result_r.write(reinterpret_cast<char*>(pad.data()), padding);
        }
    }
    result_r.close();

    std::vector<uint8_t>rotatedpixels_l(width*height*3); //rotate counterclockwise

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            for (int g = 0; g<3; ++g)
            {
                rotatedpixels_l[((width - j - 1) * height + i) * 3 + g] = pixels[(i * width + j) *3 + g];
            }
        }
    }

    BMPFILEHEADER fileheader_l = fileheader;
    BMPIMAGEHEADER imageheader_l = imageheader;
    BMPRGB rgbheader_l = rgbheader;

    imageheader_l.height = imageheader.width;
    imageheader_l.width = imageheader.height;

    std::ofstream result_l("counterclockwise.bmp", std::ios::binary);

    result_l.write(reinterpret_cast<char*>(&fileheader_l), sizeof(BMPFILEHEADER));
    result_l.write(reinterpret_cast<char*>(&imageheader_l), sizeof(BMPIMAGEHEADER));
    result_l.write(reinterpret_cast<char*>(&rgbheader_l), sizeof(BMPRGB));

    for (int i = imageheader_l.height-1; i >= 0; --i)
    {
        for (int j = 0; j < imageheader_l.width; ++j)
        {
            for (int g = 0; g<3; ++g)
            {
                result_l.write(reinterpret_cast<char*>(&rotatedpixels_r[(i * width + j)*3 + g]), 1);
            }
        }
        int padding_l = (4 - (imageheader_l.width * 3) % 4) % 4;

        if (padding_l > 0)
        {
            std::vector<uint8_t> padl(padding_l, 0);
            result_l.write(reinterpret_cast<char*>(padl.data()), padding_l);
        }
    }
    result_l.close();

    double gausskernel [3][3] =
    {
        {0.0585, 0.0965, 0.0585},
        {0.0965, 0.1592, 0.0965},
        {0.0585, 0.0965, 0.0585}
    };

    std::vector<uint8_t> pixels_b(height * width * 3);

    for (int y=0; y < imageheader_r.height; ++y)
    {
        for (int x=0; x < imageheader_r.width; ++x)
        {
            double redsum = 0.0, greensum = 0.0, bluesum = 0.0;
            for (int ky = -1; ky <= 1; ++ky)
            {
                for (int kx = -1; kx <= 1; ++kx)
                {
                    int pixel_y = std::min(std::max(y + ky, 0), height - 1);
                    int pixel_x = std::min(std::max(x + kx, 0), width - 1);

                    redsum += gausskernel[ky + 1][kx + 1] *\
 rotatedpixels_r[(pixel_y * imageheader_r.width + pixel_x)*3+2];
                    greensum += gausskernel[ky + 1][kx + 1] *\
 rotatedpixels_r[(pixel_y * imageheader_r.width + pixel_x)*3+1];
                    bluesum += gausskernel[ky + 1][kx + 1] *\
 rotatedpixels_r[(pixel_y * imageheader_r.width + pixel_x)*3];
                }
            }
            pixels_b[(y * imageheader_r.width + x)*3 + 2] = static_cast<uint8_t>(redsum);
            pixels_b[(y * imageheader_r.width + x)*3 + 1] = static_cast<uint8_t>(greensum);
            pixels_b[(y * imageheader_r.width + x)*3] = static_cast<uint8_t>(bluesum);
        }
    }
    std::ofstream result_b("blurred.bmp", std::ios::binary);

    result_b.write(reinterpret_cast<char*>(&fileheader), sizeof(BMPFILEHEADER));
    result_b.write(reinterpret_cast<char*>(&imageheader), sizeof(BMPIMAGEHEADER));
    result_b.write(reinterpret_cast<char*>(&rgbheader), sizeof(BMPRGB));

    for (int i = imageheader_r.height - 1; i >= 0; --i)
    {
        for (int j = 0; j < imageheader_r.width; ++j)
        {
            for (int g = 0; g < 3; ++g)
            {
                result_b.write(reinterpret_cast<char*>(&pixels_b[(i * imageheader_r.width + j)*3 + g]), 1);
            }
        }
        int padding_b = (4 - (imageheader_r.width * 3) % 4) % 4;

        if (padding_b > 0)
        {
        std::vector<uint8_t> pad_b(padding_b, 0);
        result_b.write(reinterpret_cast<char*>(pad_b.data()), padding_b);
        }
    }
    result_b.close();
    return 0;
}
