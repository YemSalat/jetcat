/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Fragmented surface output (view ports, moving ports, limitators              */
/********************************************************************************/
#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "../Surface/surface.h"
#include "../Surface/easy_draw.h"

namespace Componentality
{
	namespace Graphics
	{
		class ViewPort : public ISurface
		{
		protected:
			ISurface& mSurface;
			size_t mXOffset;
			size_t mYOffset;
			size_t mWidth;
			size_t mHeight;
		public:
			ViewPort(ISurface& master, const Point topleft, const Point bottomright);
			virtual ~ViewPort();
		public:
			virtual void plot(const size_t x, const size_t y, const Color&);
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const { return mWidth; };
			// Get height
			virtual size_t getHeight() const { return mHeight; };
		};

		class LimitingPort : public ISurface
		{
		protected:
			ISurface& mSurface;
			size_t mXOffset;
			size_t mYOffset;
			size_t mWidth;
			size_t mHeight;
		public:
			LimitingPort(ISurface& master, const Point topleft, const Point bottomright);
			virtual ~LimitingPort();
		public:
			virtual void plot(const size_t x, const size_t y, const Color&);
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const { return mWidth; };
			// Get height
			virtual size_t getHeight() const { return mHeight; };
		};

		class MovingPort : public ISurface
		{
		protected:
			ISurface& mSurface;
			int mXOffset;
			int mYOffset;
		public:
			MovingPort(ISurface& master, const int xshift, const int yshift);
			MovingPort(ISurface& master, Point shift) : mSurface(master) { mXOffset = shift.x; mYOffset = shift.y; }
			virtual ~MovingPort();
		public:
			virtual void plot(const size_t x, const size_t y, const Color&);
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const { return mSurface.getWidth() - mXOffset; };
			// Get height
			virtual size_t getHeight() const { return mSurface.getHeight() - mYOffset; };
		};

		class NullSurface : public ISurface
		{
		protected:
			size_t mWidth;
			size_t mHeight;
		public:
			NullSurface(const size_t width, const size_t height) : mWidth(width), mHeight(height) {};
			virtual ~NullSurface() {};
		public:
			virtual void plot(const size_t x, const size_t y, const Color&) {};
			virtual Color peek(const size_t x, const size_t y) { return Color(); };
			// Get width
			virtual size_t getWidth() const { return mWidth; };
			// Get height
			virtual size_t getHeight() const { return mHeight; };
		};

		class MeasuringSurface : public NullSurface
		{
		protected:
			size_t mXMin;
			size_t mXMax;
			size_t mYMin;
			size_t mYMax;
		public:
			MeasuringSurface(const size_t width, const size_t height) : NullSurface(width, height) { reset(); };
			virtual ~MeasuringSurface() {};
		public:
			virtual void plot(const size_t x, const size_t y, const Color&);
			virtual Color peek(const size_t x, const size_t y);
		public:
			void reset();
			size_t getXMin() const { return mXMin; }
			size_t getXMax() const { return mXMax; }
			size_t getYMin() const { return mYMin; }
			size_t getYMax() const { return mYMax; }
		};

	} // namespace Graphics
} // namespace Componentality

#endif