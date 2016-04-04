/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Drawer is a toolset for plotting graphic primitives over a surface.          */
/* This file contains simplest drawer which draws these primitives using just a */
/* regular pixel-based drawing. However, re-definition of the surface plot lets */
/* drawer do more complicated printing using brushes etc.                       */
/********************************************************************************/

#ifndef __EASY_DRAW_H__
#define __EASY_DRAW_H__

#include "surface.h"

namespace Componentality
{
	namespace Graphics
	{
		// Simple representation of 2D point. Just to reduce the number of function parameters
		struct Point
		{
			int x;
			int y;
			Point(const size_t _x = 0, const size_t _y = 0) : x(_x), y(_y) {};
			virtual ~Point() {};
		};

		inline Point operator+(const Point a, const Point b)
		{
			return Point(a.x + b.x, a.y + b.y);
		}

		inline Point operator-(const Point a, const Point b)
		{
			return Point(a.x - b.x, a.y - b.y);
		}

		// Drawer defines basic algorithms for graphic primitives like lines, rectangles and circles
		class Drawer
		{
		protected:
			ISurface& mSurface;						// Surface used for plotting
		public:
			// Making drawer over given surface
			Drawer(ISurface& surf) : mSurface(surf) {};
			virtual ~Drawer() {};
		public:
			// Draw a single point
			virtual void point(const Point, const Color&);
			// Draw a line from start to end
			virtual void line(const Point start, const Point end, const Color&);
			// Draw rectangle (only four lines)
			virtual void rectangle(const Point topleft, const Point bottomright, const Color&);
			// Draw filled rectangle
			virtual void filled_rectangle(const Point topleft, const Point bottomright, const Color&);
			// Draw circle
			virtual void circle(const Point, const size_t, const Color&);
			// Draw filled circle
			virtual void filled_circle(const Point, const size_t, const Color&);
		};

	} // namespace Graphics
} // namespace Componentality

#endif
