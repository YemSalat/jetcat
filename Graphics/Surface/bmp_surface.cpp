/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/

#include "bmp_surface.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::Bitmap;

// Adjust bitmap line length to the BMP requirements
// Microsoft requires bitmap line to be aligned to 4 bytes
static inline size_t row_width(const size_t mWidth)
{
	return ((mWidth + 3) / 4) * 4;
}

// Bitmap surface constructor
BitmapSurface::BitmapSurface(const size_t width, const size_t height) : ISurface(width, height)
{
	mWidth = width;
	mHeight = height;
	// Allocates memory for the bitmap
	unsigned int size = row_width(width) * height * 3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	mBuffer = new char[size];
	memset(mBuffer, 0, size);
	// Fill bitmap header structures
	mFileHeader = (PBITMAPFILEHEADER)mBuffer;
	mInfoHeader = (PBITMAPINFOHEADER) (mBuffer + sizeof(BITMAPFILEHEADER));
	mFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	mFileHeader->bfSize = size;
	mFileHeader->bfType = 0x4D42;
	mInfoHeader->biBitCount = 24;
	mInfoHeader->biPlanes = 1;
	mInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	mInfoHeader->biCompression = 0;
	mInfoHeader->biSizeImage = row_width(width) * height * 3;
	mInfoHeader->biWidth = mWidth;
	mInfoHeader->biHeight = mHeight;
	mInfoHeader->biXPelsPerMeter = 2835;
	mInfoHeader->biYPelsPerMeter = 2835;
};

// Deallocate bitmap's memory
BitmapSurface::~BitmapSurface() 
{

	delete[] mBuffer;
};

// Set given pixel's color
void BitmapSurface::plot(const size_t x, const size_t y, const Color& color)
{
	if ((x >= mWidth) || (y >= mHeight))
	{
		onErrorBoundaries(x, y);
		return;
	}

	ColorRGB col = color;

	// Index of the bit in bitmap's array
	unsigned int index = (((mHeight - y - 1) * row_width(mWidth * 3)) + x * 3) + mFileHeader->bfOffBits;
	mBuffer[index + 0] = col.blue;
	mBuffer[index + 1] = col.green;
	mBuffer[index + 2] = col.red;
}

// Retrieve pixel's color
Color BitmapSurface::peek(const size_t x, const size_t y)
{
	if ((x >= mWidth) || (y >= mHeight))
	{
		onErrorBoundaries(x, y);
		return Color();
	}

	ColorRGB col;

	// Same index to used in plot
	unsigned int index = (((mHeight - y - 1) * row_width(mWidth * 3)) + x * 3) + mFileHeader->bfOffBits;
	col.blue  = mBuffer[index + 0];
	col.green = mBuffer[index + 1];
	col.red   = mBuffer[index + 2];

	return col;
}

// Use bitmap read method for getting bitmap from a file
bool BitmapSurface::read(const std::string& fname)
{
	CST::Common::scoped_lock lock(mLock);
	if (mBuffer)
		delete[] mBuffer;
	mBuffer = NULL;
	bool result = Componentality::Graphics::Bitmap::read(fname, mFileHeader, mInfoHeader, mBuffer);
	if (result)
	{
		mWidth = mInfoHeader->biWidth;
		mHeight = mInfoHeader->biHeight;
	}
	else
		mBuffer = NULL;
	return result;
}

// Write bitmap to a file using bitmap's method
bool BitmapSurface::write(const std::string& fname)
{
	CST::Common::scoped_lock lock(mLock);
	return Componentality::Graphics::Bitmap::write(fname, mFileHeader, mInfoHeader, mBuffer);
}
