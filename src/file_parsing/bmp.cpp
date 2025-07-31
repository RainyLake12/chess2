#include "bmp.hpp"
#include <string>
#include <fstream>
#include <cmath>
#include <cstdint>

using namespace std;

/*
If an error occurs an image with a width set to the error code will be returned.
Otherwise an image struct will be returned.

Error Codes:
-1 : Not a valid BMP File
*/
image parseBMP(string filename) {

	image img;

	ifstream in(filename, ios::binary);
	BMPFileHeader fileHeader;
	in.read(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));

	if (fileHeader.bfType != 0x4D42) {
		image invalid;
		invalid.width = -1;
		return invalid;
	}

	BMPInfoHeader infoHeader;
	in.read(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));

	vector<uint32_t> pallete = {};

	int palleteSize = 0;
	if (infoHeader.biBitCount < 16) {
		switch (infoHeader.biBitCount) {
			case 1: palleteSize = 2; break;
			case 4: palleteSize = 16; break;
			case 8: palleteSize = 256; break;
		}
	}

	if (palleteSize > 0) {
		for (int i = 0; i < palleteSize; i++) {
			uint32_t color;
			in.read(reinterpret_cast<char *>(&color), 4);
		}
	}

	in.seekg(fileHeader.bfOffBits);
	int size;
	int pixelCount = infoHeader.biWidth * abs(infoHeader.biHeight);

	if (infoHeader.biSize == 0) {
		size = ceil(infoHeader.biWidth * abs(infoHeader.biHeight) * infoHeader.biBitCount / 8.0);
	} else {
		size = infoHeader.biSize;
	}

	uint8_t *rawData = new uint8_t[size];
	img.pixels = new uint32_t[pixelCount];

	in.read(reinterpret_cast<char *>(rawData), size);

	switch (infoHeader.biBitCount) {

		case 1 : {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < 8; j++) {
					if (i * 8 + j > pixelCount) goto breakcase1;
					img.pixels[i * 8 + j] = pallete[(rawData[i] << j ) & 1 ? pallete[1] : pallete[0]];
				}
			}
			breakcase1:
			break;
		}

		case 4: {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < 2; j++) {
					if (i * 2 + j > pixelCount) goto breakcase4;
					img.pixels[i * 2 + j] = pallete[(rawData[i] << j * 4 ) & 15];
				}
			}
			breakcase4:
			break;
		}

		case 8: {
			for (int i = 0; i < size; i++) {
				img.pixels[i] = pallete[rawData[i]];
			}
			break;
		}

		case 16: {
			for (int i = 0; i < size; i+= 2) {
				uint8_t *buffer = new uint8_t[2];
				buffer = rawData + i;

				uint16_t color = *reinterpret_cast<uint16_t *>(buffer);

				int b = (color & 15) >> 4;
				int g = ((color << 4) & 15) >> 4;
				int r = ((color << 8) & 15) >> 4;
				int a = ((color << 12) & 15) >> 4;

				img.pixels[i] = (b + (g >> 8) + (r >> 16) + (a >> 24));
				delete[] buffer;
			}
			break;
		}

		case 24: {
			for (int i = 0; i < size; i+= 3) {
				uint8_t *buffer = new uint8_t[4];
				buffer = rawData + i;

				uint32_t color = *reinterpret_cast<uint32_t *>(buffer);

				int b = (color & 255);
				int g = ((color << 8) & 255);
				int r = ((color << 16) & 255);

				img.pixels[i] = (b + (g >> 8) + (r >> 16));
				delete[] buffer;
			}
			break;
		}

		case 32: {
			for (int i = 0; i < size; i+= 4) {
				uint8_t *buffer;
				buffer = rawData + i;

				uint32_t color = *reinterpret_cast<uint32_t *>(buffer);

				int b = (color & 255);
				int g = ((color << 8) & 255);
				int r = ((color << 16) & 255);
				int a ((color << 24) & 255);

				img.pixels[i] = (b + (g >> 8) + (r >> 16) + (a >> 24));
				delete[] buffer;
			}
			break;
		}

	}

	img.width = infoHeader.biWidth;

	img.height = infoHeader.biHeight;

	uint32_t *temp = new uint32_t[pixelCount];

	for(int i = 0; i < pixelCount; i++) {

		temp[i] = ((img.pixels[i] << 16) & 255) | ((img.pixels[i] << 8) & 255) >> 8 | ((img.pixels[i]) & 255) >> 16 |
		((img.pixels[i] << 24) & 255) >> 24;

	}

	if (img.height < 0) {

		uint32_t *temp2 = new uint32_t[pixelCount];
		for (int x = 0; x < img.width; x++) {
			for (int y = 0; y < img.width; y++) {
				temp2[(img.height - (x * img.width)) + y] = temp[x * img.width + y];
			}
		}

		delete temp;
		temp = temp2;

	}

	delete[] img.pixels;
	img.pixels = temp;
	delete[] rawData;

	return img;
}