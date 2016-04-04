/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Sprite and Bitmap Sprite definitions                                         */
/********************************************************************************/

#include "sprite.h"

using namespace Componentality::Graphics;

BitmapSprite::BitmapSprite() : BitmapSurface(1, 1)
{
	mAlpha = 0xFF;
}

BitmapSprite::BitmapSprite(const size_t width, const size_t height) : BitmapSurface(width, height)
{
	mAlpha = 0xFF;
}

BitmapSprite::BitmapSprite(const size_t width, const size_t height, const unsigned char alpha) : BitmapSurface(width, height)
{
	mAlpha = alpha;
}

BitmapSprite::~BitmapSprite()
{
}

size_t BitmapSprite::getWidth() const
{
	return mWidth;
}

size_t BitmapSprite::getHeight() const
{
	return mHeight;
}

void BitmapSprite::apply(ISurface& surface, const size_t x, const size_t y)
{
	for (size_t yi = 0; yi < mHeight; yi++)
		for (size_t xi = 0; xi < mWidth; xi++)
			{
				ColorRGB original_color = surface.peek(xi + x, yi + y);
				ColorRGB new_color = peek(xi, yi);
				new_color.blue = ____applyAlpha(original_color.blue, new_color.blue, mAlpha);
				new_color.green = ____applyAlpha(original_color.green, new_color.green, mAlpha);
				new_color.red = ____applyAlpha(original_color.red, new_color.red, mAlpha);
				surface.plot(xi + x, yi + y, new_color);
			}
}
