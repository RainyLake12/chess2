#pragma once
#pragma pack(1)
#include <cstdint>
#include <vector>
#include <string>

struct BMPFileHeader {

	uint16_t bfType;       // must be 'BM' (0x4D42)
    uint32_t bfSize;       // size of file
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;    // offset to pixel data

} __attribute__((packed));


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

} __attribute__((packed));

struct image {

	int32_t width, height;
	uint32_t *pixels;

};

image parseBMP(string filename);