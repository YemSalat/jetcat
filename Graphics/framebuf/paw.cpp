#include "paw.h"
#include "../../../common-libs/Scanback/sbapi.h"
#include "../../../common-libs/common/common_utilities.h"

using namespace Componentality::Graphics;

Paw::Paw()
{
}

Paw::Paw(ISurface& surface)
{
	capture(surface, 0, 0, surface.getWidth(), surface.getHeight());
}

Paw::Paw(ISurface& surface, const size_t x, const size_t y, const size_t width, const size_t height)
{
	capture(surface, x, y, width, height);
}

Paw::Paw(IFrameBuffer::Fragment fragment)
{
	capture(*fragment.surface, fragment.x, fragment.y, fragment.width, fragment.height);
}

Paw::~Paw()
{
}

size_t Paw::getWidth() const
{
	return operator IFrameBuffer::Fragment().width;
}
			
size_t Paw::getHeight() const
{
	return operator IFrameBuffer::Fragment().height;
}

void Paw::capture(ISurface& surface, const size_t x, const size_t y, const size_t width, const size_t height)
{
	CST::Common::blob to_compress_red(width * height);
	CST::Common::blob to_compress_green(width * height);
	CST::Common::blob to_compress_blue(width * height);
	for (size_t j = 0; j < height; j++)
		for (size_t i = 0; i < width; i++)
		{
			ColorRGB color = surface.peek(i, j);
			to_compress_red.mData  [j * width + i] = color.red;
			to_compress_green.mData[j * width + i] = color.green;
			to_compress_blue.mData [j * width + i] = color.blue;
		}
	mData.clear();
	CST::Common::blob _x = CST::Common::numberToBlob<unsigned short>((unsigned short) x);
	CST::Common::blob _y = CST::Common::numberToBlob<unsigned short>((unsigned short)y);
	CST::Common::blob _w = CST::Common::numberToBlob<unsigned short>((unsigned short)width);
	CST::Common::blob _h = CST::Common::numberToBlob<unsigned short>((unsigned short)height);
	mData += CST::Common::blobToString(_x);
	mData += CST::Common::blobToString(_y);
	mData += CST::Common::blobToString(_w);
	mData += CST::Common::blobToString(_h);
	_x.purge(); _y.purge(); _w.purge(); _h.purge();
	Componentality::Common::RLESBCompressor compressor;
	CST::Common::blob red = compressor(to_compress_red);
	CST::Common::blob green = compressor(to_compress_green);
	CST::Common::blob blue = compressor(to_compress_blue);
	
	CST::Common::blob red_size = CST::Common::numberToBlob<unsigned long>((unsigned long)red.mSize);
	CST::Common::blob green_size = CST::Common::numberToBlob<unsigned long>((unsigned long)green.mSize);
	CST::Common::blob blue_size = CST::Common::numberToBlob<unsigned long>((unsigned long)blue.mSize);
	mData += CST::Common::blobToString(red_size);
	mData += CST::Common::blobToString(green_size);
	mData += CST::Common::blobToString(blue_size);
	red_size.purge(); green_size.purge(); blue_size.purge();

	mData += CST::Common::blobToString(red);
	mData += CST::Common::blobToString(green);
	mData += CST::Common::blobToString(blue);
	red.purge();
	green.purge();
	blue.purge();
}
	
void Paw::apply(ISurface& surface)
{
	IFrameBuffer::Fragment size_and_pos = operator IFrameBuffer::Fragment();
	if (!size_and_pos.width || !size_and_pos.height)
		return;
	size_t red_size = CST::Common::blobToNumber<unsigned long>(CST::Common::stringToBlob(mData.substr(8, 4)));
	size_t green_size = CST::Common::blobToNumber<unsigned long>(CST::Common::stringToBlob(mData.substr(12, 4)));
	size_t blue_size = CST::Common::blobToNumber<unsigned long>(CST::Common::stringToBlob(mData.substr(16, 4)));
	CST::Common::blob red_compressed, green_compressed, blue_compressed;
	red_compressed.mData = (char*)mData.c_str() + 20;
	red_compressed.mSize = red_size;
	green_compressed.mData = (char*)mData.c_str() + 20 + red_size;
	green_compressed.mSize = green_size;
	blue_compressed.mData = (char*)mData.c_str() + 20 + red_size + green_size;
	blue_compressed.mSize = blue_size;

	CST::Common::blob red, green, blue;
	Componentality::Common::RLESBDecompressor decompressor(size_and_pos.width * size_and_pos.height);
	red = decompressor(red_compressed);
	green = decompressor(green_compressed);
	blue = decompressor(blue_compressed);
	for (size_t i = 0; i < size_and_pos.width; i++)
		for (size_t j = 0; j < size_and_pos.height; j++)
		{
			Color color(red.mData[j * size_and_pos.width + i], green.mData[j * size_and_pos.width + i], blue.mData[j * size_and_pos.width + i]);
			surface.plot(i, j, color);
		}
	red.purge(); green.purge(); blue.purge();
}

Paw::operator IFrameBuffer::Fragment() const
{
	IFrameBuffer::Fragment result;
	result.x = result.y = result.height = result.width = 0;
	result.surface = NULL;
	if (mData.size() < 12)
		return result;
	std::string x_str = mData.substr(0, 2);
	std::string y_str = mData.substr(2, 2);
	std::string width_str = mData.substr(4, 2);
	std::string height_str = mData.substr(6, 2);
	result.x = CST::Common::blobToNumber<unsigned short>(CST::Common::stringToBlob(x_str));
	result.y = CST::Common::blobToNumber<unsigned short>(CST::Common::stringToBlob(y_str));
	result.width = CST::Common::blobToNumber<unsigned short>(CST::Common::stringToBlob(width_str));
	result.height = CST::Common::blobToNumber<unsigned short>(CST::Common::stringToBlob(height_str));
	return result;
}
