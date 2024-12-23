/* Vlasenko Anastasia Igorevna st128044.student.spbu.ru Labwork1 */

#ifndef IMG_H
#define IMG_H

// Подключать только то, что используешь непосредственно в этом файле
#include <fstream>
#include <iostream>
#include <vector>

#pragma pack(push, 1)

struct BMPFILEHEADER
{
    uint16_t FileType;
    uint32_t FileSize;
    uint16_t Reserved1;
    uint16_t Reserved2;
    uint32_t Pixelsoffset;
};

struct BMPIMAGEHEADER
{
    uint32_t HeaderSize;
    int32_t width;
    int32_t height;
    uint16_t NumPlanes;
    uint16_t BitsPerPixel;
    uint32_t Compression;
    uint32_t ImageSize;
    int32_t XRes;
    int32_t YRes;
    int32_t NumColors;
    int32_t NumImpColors;
};

#pragma pack(pop)
struct BMPRGB
{
    int32_t RGB_red{0x00ff0000};
    int32_t RGB_green{0x0000ff00};
    int32_t RGB_blue{0x000000ff};
};

class IMG
{
public:
    // Поля следовало сделать приватными, чтобы не было возможности изменять их вне публичного интерфейса класса
    // Это нарушение инкапсуляции
    BMPFILEHEADER fileheader;
    BMPIMAGEHEADER imageheader;
    BMPRGB rgbheader;

    std::vector<uint8_t> pixels;

    IMG(const char* fileName);
    // Именуешь в разных стилях, выбери какой-то один и придерживайся
    void Rotate_right();
    void Rotate_left();
    void Gaussian_filter();
    void save_pic(const char* fileName);
};


#endif
