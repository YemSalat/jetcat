/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Bitmap surface is a custom surface representing 24-bit plain bitmap.         */
/* All palette-based bitmap types aren't supported by Bitmap surface            */
/********************************************************************************/

#ifndef __BMP_SURFACE_H__
#define __BMP_SURFACE_H__

#include <memory.h>
#include "surface.h"
#include "../bitmap/bmp.h"
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{

		// Custom type of the surface with bitmap image as a physical surface implementation
		class BitmapSurface : public ISurface
		{
			friend class BitmapImage;
		protected:
			CST::Common::mutex mLock;
			char* mBuffer;								// Memory buffer. Contains both headers and pixels data
			unsigned int mWidth;						// Surface width
			unsigned int mHeight;						// Surface height
			Bitmap::PBITMAPFILEHEADER mFileHeader;		// Bitmap file header (according to Microsoft specification)
			Bitmap::PBITMAPINFOHEADER mInfoHeader;		// Bitmap info header (according to Microsoft specification)
		public:
			BitmapSurface(const size_t width = 0, const size_t height = 0); // Makes bitmap of the given size
			virtual ~BitmapSurface();
		public:
			// Surface-based set and get pixel's color methods
			virtual void plot(const size_t x, const size_t y, const Color&);
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const { return mWidth; };
			// Get height
			virtual size_t getHeight() const { return mHeight; };
		public:
			// Reading of the bitmap from a file
			virtual bool read(const std::string&);
			// Writing of the bitmap to a file
			virtual bool write(const std::string&);
		public:
			// Retrieve raw memory
			virtual char* getMemory() const { return mBuffer; };
			// Retrieve data memory
			virtual char* getColorMemory() const { return mBuffer + mFileHeader->bfOffBits; }
		};



	} // namespace Graphics
} // namespace Componentality

#endif
