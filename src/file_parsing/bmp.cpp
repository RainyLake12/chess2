#include "file_parsing/bmp.hpp"
#include <expected>
#include <string>
#include <fstream>

#define twelve 12f

using namespace std;
/*
Error Codes :
0 - Invalied BMP
*/
expected<image, int> parseImage(string data) {

	int pointer = 0;
	char buffer[sizeof(BMPFileHeader)];
	string temp = data.substr(pointer, sizeof(BMPFileHeader));
	pointer += sizeof(BMPFileHeader);
	memcpy(buffer, temp.c_str(), sizeof(BMPFileHeader));
	BMPFileHeader fileHeader = *reinterpret_cast<BMPFileHeader *>(buffer);

	if (fileHeader.bfType != 0x4D42)
		return unexpected(0);

	// TODO : Continue later

}

/*
Error Codes :
-1: Couldn't open file
other: refer to parseImage
*/
expected<image, int> parseImageFromFile(string file) {

	ifstream in(file, ios::binary);
	if (!in) return unexpected(-1);

	in.seekg(0, ios::end);
	int size = in.tellg();

	in.seekg(0);
	char *buffer = new char [size];

	string str(buffer, size);
	expected<image, int> expectedImg = parseImage(str);

	if (!expectedImg) {
		return unexpected(expectedImg.error());
	}

	delete[] buffer;

	return *expectedImg;
}