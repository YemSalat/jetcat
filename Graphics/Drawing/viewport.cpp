/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Fragmented surface output (view ports, moving ports, limitators              */
/********************************************************************************/
#include "viewport.h"

using namespace Componentality::Graphics;

ViewPort::ViewPort(ISurface& master, const Point topleft, const Point bottomright) : mSurface(master)
{
	mXOffset = topleft.x;
	mYOffset = topleft.y;
	mWidth = bottomright.x - topleft.x + 1;
	mHeight = bottomright.y - topleft.y + 1;
}

ViewPort::~ViewPort()
{
}

void ViewPort::plot(const size_t x, const size_t y, const Color& color)
{
	if ((x >= mWidth) || (y >= mHeight))
		onErrorBoundaries(x, y);
	else
		mSurface.plot(x + mXOffset, y + mYOffset, color);
}

Color ViewPort::peek(const size_t x, const size_t y)
{
	if ((x >= mWidth) || (y >= mHeight))
	{
		onErrorBoundaries(x, y);
		return Color();
	}
	else
		return mSurface.peek(x + mXOffset, y + mYOffset);
}

////////////////////////////////////////////////////////////////////

LimitingPort::LimitingPort(ISurface& master, const Point topleft, const Point bottomright) : mSurface(master)
{
	mXOffset = topleft.x;
	mYOffset = topleft.y;
	mWidth = bottomright.x - topleft.x;
	mHeight = bottomright.y - topleft.y;
}

LimitingPort::~LimitingPort()
{
}

void LimitingPort::plot(const size_t x, const size_t y, const Color& color)
{
	if ((x >= mXOffset + mWidth) || (y >= mYOffset + mHeight) || (x < mXOffset) || (y < mYOffset))
		onErrorBoundaries(x, y);
	else
		mSurface.plot(x, y, color);
}

Color LimitingPort::peek(const size_t x, const size_t y)
{
	if ((x >= mXOffset + mWidth) || (y >= mYOffset + mHeight) || (x < mXOffset) || (y < mYOffset))
	{
		onErrorBoundaries(x, y);
		return Color();
	}
	else
		return mSurface.peek(x, y);
}

////////////////////////////////////////////////////////////////////

MovingPort::MovingPort(ISurface& master, const int xshift, const int yshift) : mSurface(master)
{
	mXOffset = xshift;
	mYOffset = yshift;
}

MovingPort::~MovingPort()
{
}

void MovingPort::plot(const size_t x, const size_t y, const Color& color)
{
	int xc = (int)x + mXOffset;
	int yc = (int)y + mYOffset;
	if ((xc >= 0) && (yc >= 0))
		mSurface.plot(xc, yc, color);
}

Color MovingPort::peek(const size_t x, const size_t y)
{
	int xc = (int)x + mXOffset;
	int yc = (int)y + mYOffset;
	return mSurface.peek(xc, yc);
}

////////////////////////////////////////////////////////////////////////

void MeasuringSurface::plot(const size_t x, const size_t y, const Color&)
{
	if (x < mXMin)
		mXMin = x;
	if (y < mYMin)
		mYMin = y;
	if (x > mXMax)
		mXMax = x;
	if (y > mYMax)
		mYMax = y;
}

Color MeasuringSurface::peek(const size_t x, const size_t y)
{
	if (x < mXMin)
		mXMin = x;
	if (y < mYMin)
		mYMin = y;
	if (x > mXMax)
		mXMax = x;
	if (y > mYMax)
		mYMax = y;
	return NullSurface::peek(x, y);
}

void MeasuringSurface::reset()
{
	mXMax = mYMax = 0;
	mXMin = mYMin = (size_t)-1;
}
