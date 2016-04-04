/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Alfa and gradient transparency support                                       */
/********************************************************************************/

#ifndef __TRANSPARENCY_H__
#define __TRANSPARENCY_H__

#include "brush.h"

namespace Componentality
{
	namespace Graphics
	{
		class AlphaBrush : public IBrush
		{
		protected:
			unsigned char mAlpha;
		public:
			AlphaBrush(const unsigned char alpha = 0xFF) { mAlpha = alpha; };
			virtual ~AlphaBrush() {};
			virtual void plot(ISurface&, const size_t x, const size_t y, const Color&);
		public:
			void setAlpha(const unsigned char alpha) { mAlpha = alpha; }
			unsigned char getAlpha() const { return mAlpha; }
		};

		class GradientBrush : public IBrush
		{
		protected:
			unsigned char mLeft;
			unsigned char mRight;
			unsigned char mTop;
			unsigned char mBottom;
		protected:
			GradientBrush() {};
		public:
			GradientBrush(const unsigned char left, const unsigned char right, const unsigned char top, const unsigned char bottom)
				: mLeft(left), mTop(top), mRight(right), mBottom(bottom) {};
			virtual ~GradientBrush() {};
			virtual void plot(ISurface&, const size_t x, const size_t y, const Color&);
		};

	} // namespace Graphics
} // namespace Componentality

#endif