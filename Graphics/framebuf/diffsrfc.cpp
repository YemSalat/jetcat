#include "diffsrfc.h"

using namespace Componentality::Graphics;

void Componentality::Graphics::operator-=(ISurface& dst, ISurface& src)
{
	for (size_t i = 0; i < ____min(dst.getWidth(), src.getWidth()); i++)
		for (size_t j = 0; j < ____min(dst.getHeight(), src.getHeight()); j++)
		{
			ColorRGB dst_color = dst.peek(i, j);
			ColorRGB src_color = src.peek(i, j);
			dst_color.red -= src_color.red;
			dst_color.green -= src_color.green;
			dst_color.blue -= src_color.blue;
			dst.plot(i, j, dst_color);
		}
}

void Componentality::Graphics::operator+=(ISurface& dst, ISurface& src)
{
	for (size_t i = 0; i < ____min(dst.getWidth(), src.getWidth()); i++)
		for (size_t j = 0; j < ____min(dst.getHeight(), src.getHeight()); j++)
		{
			ColorRGB dst_color = dst.peek(i, j);
			ColorRGB src_color = src.peek(i, j);
			dst_color.red += src_color.red;
			dst_color.green += src_color.green;
			dst_color.blue += src_color.blue;
			dst.plot(i, j, dst_color);
		}
}
