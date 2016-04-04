#include "bmp.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <inttypes.h>

using namespace std;
using namespace Componentality::Graphics::Bitmap;

#ifndef CHAR_BIT
#define CHAR_BIT (sizeof(char) * 8)
#define static_assert(x, y)
#endif

static bool isLittleEndian()
{
	volatile uint32_t i = 0x01234567;
	// return 0 for big endian, 1 for little endian.
	return (*((uint8_t*)(&i))) == 0x67;
}

template <typename T>
T swap_endian(T u)
{
	static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

	union
	{
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T)-k - 1];

	return dest.u;
}

static void swap_endian(BITMAPFILEHEADER& file_header, BITMAPINFOHEADER& info_header)
{
	if (!isLittleEndian())
	{
		swap_endian<WORD>(file_header.bfType);
		swap_endian<DWORD>(file_header.bfSize);
		swap_endian<WORD>(file_header.bfReserved1);
		swap_endian<WORD>(file_header.bfReserved2);
		swap_endian<DWORD>(file_header.bfOffBits);

		swap_endian<DWORD>(info_header.biSize);
		swap_endian<LONG>(info_header.biWidth);
		swap_endian<LONG>(info_header.biHeight);
		swap_endian<WORD>(info_header.biPlanes);
		swap_endian<WORD>(info_header.biBitCount);
		swap_endian<DWORD>(info_header.biCompression);
		swap_endian<DWORD>(info_header.biSizeImage);
		swap_endian<LONG>(info_header.biXPelsPerMeter);
		swap_endian<LONG>(info_header.biYPelsPerMeter);
		swap_endian<DWORD>(info_header.biClrUsed);
		swap_endian<DWORD>(info_header.biClrImportant);
	}
}

bool Componentality::Graphics::Bitmap::read(string filename, PBITMAPFILEHEADER& file_header, PBITMAPINFOHEADER& info_header, char*& buffer)
{
	std::ifstream file(filename.c_str(), std::ios::binary);

	if (file) {
		file.seekg(0, std::ios::end);
		streampos length = file.tellg();
		file.seekg(0, std::ios::beg);

		unsigned long size = (unsigned long) length;
		buffer = new char[size];
		size_t remain = (size_t) length;
		size_t index = 0;
		while (remain)
		{
			size_t toread = remain > 512000 ? 512000 : remain;
			file.read(buffer + index, toread);
			index += toread;
			remain -= toread;
		}
		file_header = (PBITMAPFILEHEADER)(&buffer[0]);
		info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
	}
	else
		return false;
	swap_endian(*file_header, *info_header);
	return true;
}

bool Componentality::Graphics::Bitmap::write(string filename, PBITMAPFILEHEADER& file_header, PBITMAPINFOHEADER& info_header, char*& buffer)
{
	ofstream file(filename.c_str(), std::ios::binary);

	if (file)
	{
		swap_endian(*file_header, *info_header);
		file.write(buffer, file_header->bfSize);
		swap_endian(*file_header, *info_header);
	}
	else
		return false;
	return true;
}
