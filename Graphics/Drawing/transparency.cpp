/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Alfa and gradient transparency support                                       */
/********************************************************************************/
#include "transparency.h"
#include "sprite.h"

using namespace Componentality::Graphics;

void AlphaBrush::plot(ISurface& surface, const size_t x, const size_t y, const Color& color)
{
	ColorRGB original_color = surface.peek(x, y);
	ColorRGB new_color = color;
	new_color.blue = ____applyAlpha(original_color.blue, new_color.blue, mAlpha);
	new_color.green = ____applyAlpha(original_color.green, new_color.green, mAlpha);
	new_color.red = ____applyAlpha(original_color.red, new_color.red, mAlpha);
	surface.plot(x, y, new_color);
}

void GradientBrush::plot(ISurface& surface, const size_t x, const size_t y, const Color& color)
{
	long long x_scale = 10000 * x / surface.getWidth();
	long long y_scale = 10000 * y / surface.getHeight();
	x_scale = (mRight * x_scale) + (mLeft * (10000 - x_scale));
	y_scale = (mBottom * y_scale) + (mTop * (10000 - y_scale));
	long long alpha = (x_scale + y_scale) / 20000;

	ColorRGB original_color = surface.peek(x, y);
	ColorRGB new_color = color;
	new_color.blue = ____applyAlpha(original_color.blue, new_color.blue, (unsigned char) alpha);
	new_color.green = ____applyAlpha(original_color.green, new_color.green, (unsigned char)alpha);
	new_color.red = ____applyAlpha(original_color.red, new_color.red, (unsigned char)alpha);
	surface.plot(x, y, new_color);
}