/* Vlasenko Anastasia Igorevna st128044.student.spbu.ru Labwork1 */

#ifndef IMG_H
#define IMG_H

#include <fstream>
#include <iostream>

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
    unsigned char RGB_red;
    unsigned char RGB_green;
    unsigned char RGB_blue;
    unsigned char alfa;
    unsigned char color_space_type;
};


#endif
