#include "bitmask.h"

using namespace Componentality::Graphics;

CST::Common::blob ____initblob(CST::Common::blob blob)
{
	memset(blob.mData, 0, blob.mSize);
	return blob;
}

Bitmask::Bitmask(const size_t width, const size_t height)
{
	mWidth = width;
	mHeight = height;
	mBitmem = ____initblob(CST::Common::blob((width * height + 7) / 8));
}

Bitmask::Bitmask(const size_t width, const size_t height, Color foreground)
{
	mWidth = width;
	mHeight = height;
	mForeground = foreground;
	mBitmem = ____initblob(CST::Common::blob((width * height + 7) / 8));
}

Bitmask::Bitmask(const size_t width, const size_t height, Color foreground, Color background)
{
	mWidth = width;
	mHeight = height;
	mForeground = foreground;
	mBackground = background;
	mBitmem = ____initblob(CST::Common::blob((width * height + 7) / 8));
}

Bitmask::~Bitmask()
{
	mBitmem.purge();
}

size_t Bitmask::getWidth() const
{
	return mWidth;
}

size_t Bitmask::getHeight() const
{
	return mHeight;
}

void Bitmask::apply(ISurface& surface, const size_t x, const size_t y)
{
	if (mForeground.isUndefined() && mBackground.isUndefined())
		return;
	CST::Common::blob data = mBitmem;
	for (size_t yi = 0; yi < mHeight; yi++)
		for (size_t xi = 0; xi < mWidth; xi++)
		{
			bit bitval = bitmem_get1((unsigned char*)data.mData, yi * mWidth + xi);
			if (bitval && !mForeground.isUndefined())
				surface.plot(x + xi, y + yi, mForeground);
			if (!bitval && !mBackground.isUndefined())
				surface.plot(x + xi, y + yi, mBackground);
		}
}

void Bitmask::setBit(const size_t x, const size_t y, bit val)
{
	CST::Common::blob data = mBitmem;
	bitmem_put1((unsigned char*)data.mData, y * mWidth + x, val);
	mBitmem = data;
}

bit Bitmask::getBit(const size_t x, const size_t y)
{
	return bitmem_get1((unsigned char*)mBitmem.mData, y * mWidth + x);
}

CST::Common::blob Bitmask::getMemory()
{
	return mBitmem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

AlphaMask::AlphaMask(const size_t width, const size_t height)
{
	mWidth = width;
	mHeight = height;
	mAlphamem = ____initblob(CST::Common::blob(width * height));
}

AlphaMask::AlphaMask(const size_t width, const size_t height, Color foreground)
{
	mWidth = width;
	mHeight = height;
	mForeground = foreground;
	mAlphamem = ____initblob(CST::Common::blob(width * height));
}

AlphaMask::AlphaMask(const size_t width, const size_t height, Color foreground, Color background)
{
	mWidth = width;
	mHeight = height;
	mForeground = foreground;
	mAlphamem = ____initblob(CST::Common::blob(width * height));
}

AlphaMask::~AlphaMask()
{
	mAlphamem.purge();
}

size_t AlphaMask::getWidth() const
{
	return mWidth;
}

size_t AlphaMask::getHeight() const
{
	return mHeight;
}

void AlphaMask::apply(ISurface& surface, const size_t x, const size_t y)
{
	if (mForeground.isUndefined())
		return;
	CST::Common::blob data = mAlphamem;
	for (size_t xi = 0; xi < mWidth; xi++)
		for (size_t yi = 0; yi < mHeight; yi++)
		{
			ColorRGB original_color = surface.peek(xi + x, yi + y);
			ColorRGB new_color = mForeground;
			unsigned char alpha = (unsigned char) data.mData[yi * mWidth + xi];
			new_color.blue = ____applyAlpha(original_color.blue, new_color.blue, alpha);
			new_color.green = ____applyAlpha(original_color.green, new_color.green, alpha);
			new_color.red = ____applyAlpha(original_color.red, new_color.red, alpha);
			surface.plot(xi + x, yi + y, new_color);
		}
}

void AlphaMask::setAlpha(const size_t x, const size_t y, unsigned char val)
{
	mAlphamem.mData[y * mWidth + x] = (char) val;
}

unsigned char AlphaMask::getAlpha(const size_t x, const size_t y)
{
	return (unsigned char) mAlphamem.mData[y * mWidth + x];
}

CST::Common::blob AlphaMask::getMemory()
{
	return mAlphamem;
}
