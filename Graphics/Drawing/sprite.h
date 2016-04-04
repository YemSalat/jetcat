/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Sprite and Bitmap Sprite definitions                                         */
/********************************************************************************/

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "../Surface/bmp_surface.h"

namespace Componentality
{
	namespace Graphics
	{

		inline unsigned char ____applyAlpha(const unsigned long original, const unsigned long newval, const unsigned long alpha_new)
		{
			unsigned long result = (original * (255 - alpha_new )) + newval * alpha_new;
			result /= 255;
			return (unsigned char) (result & 0xFF);
		}

		class ISprite
		{
		public:
			ISprite() {};
			virtual ~ISprite() {};
		public:
			virtual size_t getWidth() const = 0;
			virtual size_t getHeight() const = 0;
			virtual void apply(ISurface&, const size_t x, const size_t y) = 0;
		};

		class BitmapSprite : public ISprite, public BitmapSurface
		{
		protected:
			unsigned char mAlpha;
		public:
			BitmapSprite();
			BitmapSprite(const size_t width, const size_t height);
			BitmapSprite(const size_t width, const size_t height, const unsigned char alpha);
			virtual ~BitmapSprite();
		public:
			virtual size_t getWidth() const;
			virtual size_t getHeight() const;
			virtual void apply(ISurface&, const size_t x, const size_t y);
			virtual void setAlpha(const unsigned char alpha) { mAlpha = alpha; }
			virtual unsigned char getAlpha() const { return mAlpha; }
		};

	} // namespace Graphics
} // namespace Componentality

#endif