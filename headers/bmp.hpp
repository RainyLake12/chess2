#pragma once
#include <cstdint>
#include <vector>
#include <string>

#pragma pack(push, 1)

struct BMPFileHeader {

	uint16_t bfType;       // must be 'BM' (0x4D42)
    uint32_t bfSize;       // size of file
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;    // offset to pixel data

};


struct BMPInfoHeader {

    uint32_t biSize;          // size of this header (40 bytes)
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;        // must be 1
    uint16_t biBitCount;      // bits per pixel (e.g., 24)
    uint32_t biCompression;   // 0 = none
    uint32_t biSizeImage;     // raw bitmap data size
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;

};

#pragma pack(pop)

struct image {

	int32_t width, height;
	uint32_t *pixels;

};

image parseBMP(std::string filename);