/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/

#include "easy_draw.h"

using namespace Componentality::Graphics;

static inline long abs(long x)
{
	return x < 0 ? -x : x;
}

static inline int max(int x, int y)
{
	return x > y ? x : y;
}

static inline int min(int x, int y)
{
	return x < y ? x : y;
}

void Drawer::point(const Point pt, const Color& color)
{
	mSurface.plot(pt.x, pt.y, color);
}

void Drawer::line(const Point start, const Point end, const Color& color)
{
	int x1 = start.x, x2 = end.x, y1 = start.y, y2 = end.y;
	int dx = (x2 - x1 >= 0 ? 1 : -1);
	int dy = (y2 - y1 >= 0 ? 1 : -1);

	int lengthX = abs(x2 - x1);
	int lengthY = abs(y2 - y1);

	int length = max(lengthX, lengthY);

	if (length == 0)
	{
		mSurface.plot(x1, y1, color);
	}

	if (lengthY <= lengthX)
	{
		int x = x1;
		int y = y1;
		int d = -lengthX;

		length++;
		while (length--)
		{
			mSurface.plot(x, y, color);
			x += dx;
			d += 2 * lengthY;
			if (d > 0) {
				d -= 2 * lengthX;
				y += dy;
			}
		}
	}
	else
	{
		int x = x1;
		int y = y1;
		int d = -lengthY;

		length++;
		while (length--)
		{
			mSurface.plot(x, y, color);
			y += dy;
			d += 2 * lengthX;
			if (d > 0) {
				d -= 2 * lengthY;
				x += dx;
			}
		}
	}
}

void Drawer::rectangle(const Point topleft, const Point bottomright, const Color& color)
{
	int x0 = min(topleft.x, bottomright.x);
	int x1 = max(topleft.x, bottomright.x);
	int y0 = min(topleft.y, bottomright.y);
	int y1 = max(topleft.y, bottomright.y);
	int i;

	size_t width = mSurface.getWidth();
	size_t height = mSurface.getHeight();

	for (i = x0; i <= min(x1, width); i++)
	{
		mSurface.plot(i, y0, color);
		mSurface.plot(i, y1, color);
	}

	for (i = y0; i <= min(y1, height); i++)
	{
		mSurface.plot(x0, i, color);
		mSurface.plot(x1, i, color);
	}
}

void Drawer::filled_rectangle(const Point topleft, const Point bottomright, const Color& color)
{
	int x0 = min(topleft.x, bottomright.x);
	int x1 = max(topleft.x, bottomright.x);
	int y0 = min(topleft.y, bottomright.y);
	int y1 = max(topleft.y, bottomright.y);
	int i, j;

	size_t width = mSurface.getWidth();
	size_t height = mSurface.getHeight();

	for (j = y0; j <= min(y1, height); j++)
		for (i = x0; i <= min(x1, width); i++)
		{
			mSurface.plot(i, j, color);
		}
}

void Drawer::circle(const Point pt, const size_t radius, const Color& color)
{
	int _x = pt.x, _y = pt.y;
	int x = 0, y = radius, gap = 0, delta = (2 - 2 * radius);
	while (y >= 0)
	{
		mSurface.plot(_x + x, _y + y, color);
		mSurface.plot(_x + x, _y - y, color);
		mSurface.plot(_x - x, _y - y, color);
		mSurface.plot(_x - x, _y + y, color);
		gap = 2 * (delta + y) - 1;
		if (delta < 0 && gap <= 0)
		{
			x++;
			delta += 2 * x + 1;
			continue;
		}
		if (delta > 0 && gap > 0)
		{
			y--;
			delta -= 2 * y + 1;
			continue;
		}
		x++;
		delta += 2 * (x - y);
		y--;
	}
}
void Drawer::filled_circle(const Point pt, const size_t radius, const Color& color)
{
	int _x = pt.x, _y = pt.y;
	int x = 0, y = radius, gap = 0, delta = (2 - 2 * radius);
	int i;
	while (y >= 0)
	{
		for (i = min(_y + y, _y - y); i <= max(_y + y, _y - y); i++)
		{
			mSurface.plot(_x + x, i, color);
			mSurface.plot(_x - x, i, color);
		}
		gap = 2 * (delta + y) - 1;
		if (delta < 0 && gap <= 0)
		{
			x++;
			delta += 2 * x + 1;
			continue;
		}
		if (delta > 0 && gap > 0)
		{
			y--;
			delta -= 2 * y + 1;
			continue;
		}
		x++;
		delta += 2 * (x - y);
		y--;
	}
}
