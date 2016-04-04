#ifndef __BMP_H__
#define __BMP_H__

#include <string>

namespace Componentality
{
	namespace Graphics
	{
		namespace Bitmap
		{
			typedef int LONG;
			typedef unsigned short WORD;
			typedef unsigned int DWORD;

			#pragma pack(push)
			#pragma pack(1)

			typedef struct tagBITMAPFILEHEADER {
				WORD  bfType;
				DWORD bfSize;
				WORD  bfReserved1;
				WORD  bfReserved2;
				DWORD bfOffBits;
			} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

			typedef struct tagBITMAPINFOHEADER {
				DWORD biSize;
				LONG  biWidth;
				LONG  biHeight;
				WORD  biPlanes;
				WORD  biBitCount;
				DWORD biCompression;
				DWORD biSizeImage;
				LONG  biXPelsPerMeter;
				LONG  biYPelsPerMeter;
				DWORD biClrUsed;
				DWORD biClrImportant;
			} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

			#pragma pack(pop)

			extern bool read(std::string filename, PBITMAPFILEHEADER& file_header, PBITMAPINFOHEADER& info_header, char*& buffer);
			extern bool write(std::string filename, PBITMAPFILEHEADER& file_header, PBITMAPINFOHEADER& info_header, char*& buffer);

		} // namespace Bitmap
	} // namespace Graphics
} // namespace Componentality

#endif
