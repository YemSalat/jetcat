/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Support bitwise masks with and without alpha factor. Bit masks are sprites   */
/* with one (foreground) or two (+background) colors to apply single colored    */
/* or make monochrome images. Normally used for fonts drawing and similar tasks */
/********************************************************************************/

#ifndef __BITMASK_H__
#define __BITMASK_H__

#include "sprite.h"
extern "C"
{
#include "../../../common-libs/Scanback/bitmem.h"
}
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{
		class Bitmask : public ISprite
		{
		protected:
			size_t mWidth;
			size_t mHeight;
			Color  mForeground;
			Color  mBackground;
			CST::Common::blob mBitmem;
		protected:
			Bitmask() {};
		public:
			Bitmask(const size_t width, const size_t height);
			Bitmask(const size_t width, const size_t height, Color foreground);
			Bitmask(const size_t width, const size_t height, Color foreground, Color background);
			virtual ~Bitmask();
		public:
			virtual size_t getWidth() const;
			virtual size_t getHeight() const;
			virtual void apply(ISurface&, const size_t x, const size_t y);
		public:
			virtual void setForeground(const Color& color) { mForeground = color; }
			virtual void setBackground(const Color& color) { mBackground = color; }
			virtual void setBit(const size_t x, const size_t y, bit val);
			virtual bit getBit(const size_t x, const size_t y);
		public:
			virtual CST::Common::blob getMemory();
		};

		class AlphaMask : public ISprite
		{
		protected:
			size_t mWidth;
			size_t mHeight;
			Color  mForeground;
			CST::Common::blob mAlphamem;
		protected:
			AlphaMask() {};
		public:
			AlphaMask(const size_t width, const size_t height);
			AlphaMask(const size_t width, const size_t height, Color foreground);
			AlphaMask(const size_t width, const size_t height, Color foreground, Color background);
			virtual ~AlphaMask();
		public:
			virtual size_t getWidth() const;
			virtual size_t getHeight() const;
			virtual void apply(ISurface&, const size_t x, const size_t y);
		public:
			virtual void setForeground(const Color& color) { mForeground = color; }
			virtual void setAlpha(const size_t x, const size_t y, unsigned char val);
			virtual unsigned char getAlpha(const size_t x, const size_t y);
		public:
			virtual CST::Common::blob getMemory();
		};

	} // namespace Graphics
} // namespace Componentality

#endif