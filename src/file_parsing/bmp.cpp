#include "bmp.hpp"
#include <string>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <iostream>

using namespace std;

/*
If an error occurs an image with a width set to the error code will be returned.
Otherwise an image struct will be returned.

Error Codes:
-1 : Not a valid BMP File
*/
image parseBMP(string filename) {

	image img = {};

	ifstream in(filename, ios::binary);
	if (!in.is_open()) {
		cerr << "Error: Couldn't open BMP file: " << filename << endl;
		return {};
	}

	BMPFileHeader fileHeader;
	in.read(reinterpret_cast<char *>(&fileHeader), sizeof(BMPFileHeader));

	if (in.gcount() != sizeof(BMPFileHeader)) {
		cerr << "Failed to read BMP file header!" << endl;
		return {};
	}


	uint8_t *bm = reinterpret_cast<uint8_t *>(&fileHeader);
	if (!(bm[0] == 0x42 && bm[1] == 0x4D)) {
		cout << "Wrong magic number";
		return {};
	}

	BMPInfoHeader infoHeader;

	in.read(reinterpret_cast<char *>(&infoHeader), sizeof(BMPInfoHeader));
	if (in.gcount() != sizeof(BMPInfoHeader)) {
		cerr << "Failed to read BMP info header!" << endl;
		return {};
	}


	vector<uint32_t> pallete = {};

	if (infoHeader.biBitCount != 1 && infoHeader.biBitCount != 4 && infoHeader.biBitCount != 8
	&& infoHeader.biBitCount != 16 && infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
		cout << "Invalid Bit Count: " << infoHeader.biBitCount;
		return {};
	}

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
			pallete.push_back(color);
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

	if (pixelCount <= 0) {
		cerr << "Invalid pixel count!" << endl;
		exit(1);
	}
	if (size <= 0) {
		cerr << "Invalid pixel data size!" << endl;
		exit(1);
	}


	uint8_t *rawData = new uint8_t[size];
	img.pixels = new uint32_t[pixelCount];

	in.read(reinterpret_cast<char *>(rawData), size);

	switch (infoHeader.biBitCount) {

		case 1 : {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < 8; j++) {
					if (i * 8 + j > pixelCount) goto breakcase1;
					int bit = (rawData[i] >> (7 - j)) & 1;
					img.pixels[i * 8 + j] = pallete[bit];
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
				uint16_t color = *reinterpret_cast<uint16_t *>(rawData + i);

				int b = color & 15;
				int g = (color >> 4) & 15;
				int r = (color >> 8) & 15;
				int a = (color >> 12) & 15;

				img.pixels[i] = (b + (g >> 8) + (r >> 16) + (a >> 24));
			}
			break;
		}

		case 24: {
			for (int i = 0; i < size; i+= 3) {
				uint32_t color = *reinterpret_cast<uint32_t *>(rawData + i);

				int b = color & 255;
				int g = (color >> 8) & 255;
				int r = (color >> 16) & 255;

				img.pixels[i / 3] = (b + (g >> 8) + (r >> 16));
			}
			break;
		}

		case 32: {
			for (int i = 0; i < size; i+= 4) {
				uint32_t color = *reinterpret_cast<uint32_t *>(rawData + i);

				int b = color & 255;
				int g = (color >> 8) & 255;
				int r = (color >> 16) & 255;
				int a = (color >> 24) & 255;

				img.pixels[i / 4] = (b + (g >> 8) + (r >> 16) + (a >> 24));
			}
			break;
		}

	}

	img.width = infoHeader.biWidth;

	img.height = infoHeader.biHeight;

	uint32_t *temp = new uint32_t[pixelCount];

	for(int i = 0; i < pixelCount; i++) {

		uint32_t px = img.pixels[i];
		uint8_t r = (px >> 24) & 0xFF;
		uint8_t g = (px >> 16) & 0xFF;
		uint8_t b = (px >> 8) & 0xFF;
		uint8_t a = px & 0xFF;
		temp[i] = (r << 16) | (g << 8) | b;

	}

	if (img.height < 0) {

		uint32_t *temp2 = new uint32_t[pixelCount];
		for (int x = 0; x < img.width; x++) {
			for (int y = 0; y < img.width; y++) {
				temp2[(img.height - (y * img.width)) + x] = temp[y * img.width + x];
			}
		}

		delete[] temp;
		temp = temp2;

	}

	delete[] img.pixels;
	img.pixels = temp;
	delete[] rawData;

	return img;
}