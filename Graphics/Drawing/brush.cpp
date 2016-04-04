/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Definitions of abstract brush interface and the most important standard      */
/* brushes. Brush is a way to redefine pixel plotting to allow using standard   */
/* painting appoach modifying painted results on the fly.                       */
/********************************************************************************/

#include "brush.h"

using namespace Componentality::Graphics;

AdvancedSurface::AdvancedSurface(ISurface& master) : mBrush(0L), mMasterSurface(master)
{
}

AdvancedSurface::~AdvancedSurface()
{
}

void AdvancedSurface::setBrush(IBrush* brush)
{
	mBrush = brush;
}

void AdvancedSurface::plot(const size_t x, const size_t y, const Color& color)
{
	if (!mBrush)
		mMasterSurface.plot(x, y, color);
	else
		mBrush->plot(mMasterSurface, x, y, color);
}

Color AdvancedSurface::peek(const size_t x, const size_t y)
{
	return mMasterSurface.peek(x, y);
}

void CircleBrush::plot(ISurface& surface, const size_t x, const size_t y, const Color& color)
{
	Drawer(surface).filled_circle(Point(x, y), mSize / 2, color);
}

void SquareBrush::plot(ISurface& surface, const size_t x, const size_t y, const Color& color)
{
	Drawer(surface).filled_rectangle(Point(x - mSize / 2, y - mSize / 2), Point(x + mSize / 2, y + mSize / 2), color);
}
