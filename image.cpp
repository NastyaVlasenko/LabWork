/* Vlasenko Anastasia Igorevna st128044.student.spbu.ru Labwork1 */

#include "image.h"

IMG::IMG(const char* fileName)
{
    std::ifstream img(fileName, std::ios::binary);

    if(!img)
    {
        std::cerr<< "File can't be open!"<<std::endl;
    }

    img.read(reinterpret_cast<char*>(&fileheader), sizeof(BMPFILEHEADER));

    if (fileheader.FileType != 0x4D42)
    {
        std::cout<<"Please, choose bmp file"<<std::endl;
    }

    img.read(reinterpret_cast<char*>(&imageheader), sizeof(BMPIMAGEHEADER));

    if (imageheader.BitsPerPixel != 24)
    {
        std::cout<<"Please, choose 24-bit image!"<<std::endl;
    }

    img.seekg(fileheader.Pixelsoffset, std::ios::beg);
    pixels.resize(imageheader.ImageSize);
    img.read((char*)pixels.data(), pixels.size());
}
// Функции поворота очень похожи. Возможно, следовало объединить их, добавив аргумент направления вращения и условие внутрь
void IMG::Rotate_right()
{
    int row = (imageheader.height * 3 + 3) & (~3);
    std::vector<uint8_t>rotatedpixels_r(row * imageheader.width);

    for (int i = 0; i < imageheader.height; ++i)
    {
        for (int j = 0; j < imageheader.width; ++j)
        {
            int index1 = i * ((imageheader.width * 3 + 3) & (~3)) + j * 3;
            int index2 = (imageheader.width - j - 1) * row + i * 3;
            for (int g = 0; g<3; ++g)
            {
                rotatedpixels_r[index2 + g] = pixels[index1 + g];
            }
        }
    }

    std::swap(imageheader.width, imageheader.height);
    pixels = std::move(rotatedpixels_r);
}

void IMG::Rotate_left()
{
    int row = (imageheader.height * 3 + 3) & (~3);
    std::vector<uint8_t>rotatedpixels_l(row * imageheader.width);

    for (int i = 0; i < imageheader.height; ++i)
    {
        for (int j = 0; j < imageheader.width; ++j)
        {
            int index1 = i * ((imageheader.width * 3 + 3) & (~3)) + j * 3;
            int index2 = j * row + (imageheader.height - i - 1) * 3;
            // Можно было создать структуру для пикселя, которая хранилась бы в векторе, тогда можно было бы делать это одним присваиванием 
            for (int g = 0; g<3; ++g)
            {
                rotatedpixels_l[index2 + g] = pixels[index1 + g];
            }
        }
    }

    std::swap(imageheader.width, imageheader.height);
    pixels = std::move(rotatedpixels_l);
}

void IMG::Gaussian_filter()
{
    // Хотелось бы иметь возможность задавать произвольный размер ядра
    double gausskernel [3][3] =
    {
        {0.0585, 0.0965, 0.0585},
        {0.0965, 0.1592, 0.0965},
        {0.0585, 0.0965, 0.0585}
    };

    std::vector<uint8_t> pixels_b(pixels.size());
    int height = imageheader.height;
    int width = imageheader.width;

    for (int y=0; y < height; ++y)
    {
        for (int x=0; x < width; ++x)
        {
            double redsum = 0.0, greensum = 0.0, bluesum = 0.0;
            // Функция получается слишком громоздкая. Хотелось бы вынести эти внутренние циклы в отдельную функцию
            for (int ky = -1; ky <= 1; ++ky)
            {
                for (int kx = -1; kx <= 1; ++kx)
                {
                    int pixel_y = std::min(std::max(y + ky, 0), height - 1);
                    int pixel_x = std::min(std::max(x + kx, 0), width - 1);

                    redsum += gausskernel[ky + 1][kx + 1] *\
 pixels[(pixel_y * width + pixel_x)*3+2];
                    greensum += gausskernel[ky + 1][kx + 1] *\
 pixels[(pixel_y * width + pixel_x)*3+1];
                    bluesum += gausskernel[ky + 1][kx + 1] *\
 pixels[(pixel_y * width + pixel_x)*3];
                }
            }
            pixels_b[(y * width + x)*3 + 2] = static_cast<uint8_t>(redsum);
            pixels_b[(y * width + x)*3 + 1] = static_cast<uint8_t>(greensum);
            pixels_b[(y * width + x)*3] = static_cast<uint8_t>(bluesum);
        }
    }
    pixels = pixels_b;
}

void IMG::save_pic(const char* fileName)
{
    std::ofstream result(fileName, std::ios::binary);

    if(!result)
    {
        std::cerr<< "File can't be saved!"<<std::endl;
    }

    result.write((char*)&fileheader, sizeof(fileheader));
    result.write((char*)&imageheader, sizeof(imageheader));
    result.write((char*)&rgbheader, sizeof(rgbheader));

    result.write((char*)pixels.data(), pixels.size());
}

