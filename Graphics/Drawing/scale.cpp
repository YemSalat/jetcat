/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Definitions of the surface which upscales or downscales image being output   */
/* to it. Lets simply scale image without changing of the painting algorithms   */
/* and approaches.                                                              */
/********************************************************************************/

#include "scale.h"

#define round_d(x) ((int) (x))
#define round_u(x) ((int) ((double) (x) + 0.9999))

using namespace Componentality::Graphics;

// Set individual pixel's color
void ScaledSurface::plot(const size_t x, const size_t y, const Color& color)
{
	int _x = round_d(mXScaleFactor * x);
	int _y = round_d(mYScaleFactor * y);
	
	for (int i = _x; (round_u(i / mXScaleFactor) == x) && (i >= 0); i--)
	{
		for (int j = _y; (round_u(j / mYScaleFactor) == y) && (j >= 0); j--)
		{
			mMasterSurface.plot(i, j, color);
		}
	}
}

// Get individual pixel's color
Color ScaledSurface::peek(const size_t x, const size_t y)
{
	double _x = mXScaleFactor * x;
	double _y = mYScaleFactor * y;
	return mMasterSurface.peek((size_t)_x, (size_t)_y);
}

// Get width
size_t ScaledSurface::getWidth() const
{
	return (size_t)(mMasterSurface.getWidth() * mXScaleFactor);
}

// Get height
size_t ScaledSurface::getHeight() const
{
	return (size_t)(mMasterSurface.getHeight() * mYScaleFactor);
}
