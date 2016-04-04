/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Definitions of abstract brush interface and the most important standard      */
/* brushes. Brush is a way to redefine pixel plotting to allow using standard   */
/* painting appoach modifying painted results on the fly.                       */
/********************************************************************************/

#ifndef __BRUSH_H__
#define __BRUSH_H__

#include "../Surface/easy_draw.h"
#include "../Surface/surface.h"

namespace Componentality
{
	namespace Graphics
	{

		class IBrush
		{
		public:
			IBrush() {};
			virtual ~IBrush() {};
			virtual void plot(ISurface&, const size_t x, const size_t y, const Color&) = 0;
		};

		class AdvancedSurface : public ISurface
		{
		protected:
			ISurface& mMasterSurface;
			IBrush*   mBrush;
		protected:
			AdvancedSurface(const size_t width, const size_t height) : mMasterSurface(*(ISurface*) 0L) {};
		public:
			AdvancedSurface(ISurface&);
			virtual ~AdvancedSurface();
			virtual void setBrush(IBrush* = 0L);
		public:
			virtual void plot(const size_t x, const size_t y, const Color&);
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const { return mMasterSurface.getWidth(); };
			// Get height
			virtual size_t getHeight() const { return mMasterSurface.getHeight(); };
		};

		class NullBrush : public IBrush
		{
		public:
			NullBrush() {};
			virtual ~NullBrush() {};
			virtual void plot(ISurface&, const size_t x, const size_t y, const Color&) {};
		};

		class PointBrush : public IBrush
		{
		public:
			PointBrush() {};
			virtual ~PointBrush() {};
			virtual void plot(ISurface& surface, const size_t x, const size_t y, const Color& color) { surface.plot(x, y, color); };
		};

		class CircleBrush : public IBrush
		{
		protected:
			size_t mSize;
		public:
			CircleBrush(const size_t _size) : mSize(_size) {};
			virtual ~CircleBrush() {};
			virtual void plot(ISurface& surface, const size_t x, const size_t y, const Color& color);
		};

		class SquareBrush : public IBrush
		{
		protected:
			size_t mSize;
		public:
			SquareBrush(const size_t _size) : mSize(_size) {};
			virtual ~SquareBrush() {};
			virtual void plot(ISurface& surface, const size_t x, const size_t y, const Color& color);
		};

	} // namespace Graphics
} // namespace Componentality

#endif