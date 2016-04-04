/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Surface is a basis of entire library. It represents any entity, which is     */
/* capable to display anything. For example, frame buffer, image file etc. is   */
/* to be represented as a surface. Surface supports two operations: plot, i.e.  */
/* setting of the pixel color and peek, i.e. reading of the pixel color.        */
/* PGL-ES supports 24-bit color and 8-bit depth greyscale with automated        */
/* recalculation to it. 24-bit color is a default color representation.         */
/* Alpha channel is not supported by surface and implemented for other entities */
/* like sprite, separately.                                                     */
/********************************************************************************/

#ifndef __SURFACE_H___
#define __SURFACE_H___

#include <cstddef>

#ifndef ____min
#define ____min(x, y) ((x < y) ? (x) : (y))
#endif
#ifndef ____max
#define ____max(x, y) ((x > y) ? (x) : (y))
#endif

namespace Componentality
{
	namespace Graphics
	{
		//24-bit color structure
		struct ColorRGB
		{
			unsigned char red;
			unsigned char green;
			unsigned char blue;
		};

		// 8-bit greyscale
		struct Greyscale
		{
			unsigned char intensity;
		};

		// Unified color type
		class Color
		{
		protected:
			enum COLORTYPE
			{
				COLOR_ColorRGB,							// Color is natively 24-bit ColorRGB
				COLOR_GREYSCALE,					// Color is natively greyscale
				COLOR_UNDEFINED						// Color is undefined and not yet initialized
			};
			COLORTYPE mType;						// Color type used
			union
			{
				ColorRGB mColorRGB;							// ColorRGB color
				Greyscale mGreyscale;				// Greyscale color
			};
		public:
			// Making color from plain values
			Color();
			Color(const ColorRGB);
			Color(const unsigned char red, const unsigned char green, const unsigned char blue);
			Color(const Greyscale);
			Color(const unsigned char greyscale);
			Color(const Color&);
			virtual ~Color();
			Color& operator=(const ColorRGB);
			Color& operator=(const Greyscale);
			Color& operator=(const Color&);
			bool operator==(const Color& src) const;
			bool operator!=(const Color& src) const;
			operator Greyscale() const;
			operator ColorRGB() const;
			// Check if color is defined
			bool isUndefined() const { return mType == COLOR_UNDEFINED; }
		};

		// Surface interface. Represents any entity which allows plotting and getting pixels
		// Define your own surface to provide canvas for pixels-based output
		class ISurface
		{
		protected:
			ISurface() {};
		public:
			// Create surface with given dimensions
			ISurface(const size_t width, const size_t height) {};
			virtual ~ISurface() {};
		public:
			// Set individual pixel's color
			virtual void plot(const size_t x, const size_t y, const Color&) = 0;
			// Get individual pixel's color
			virtual Color peek(const size_t x, const size_t y) = 0;
			// Get width
			virtual size_t getWidth() const = 0;
			// Get height
			virtual size_t getHeight() const = 0;
			// Copy data from one surface to another
			virtual void apply(ISurface&);
		protected:
			// Event called when plot or peek tries accessing pixel out of the surface boundaries
			virtual void onErrorBoundaries(const size_t x, const size_t y) {};
			// Event called when apply starts processing surface for reading or writing (to lock it in memory)
			virtual void onApply() {};
			// Event called when apply stops processing surface for reading or writing (to unlock it in memory)
			virtual void onApplied() {};
		};

	} // namespace Graphics
} // namespace Componentality

#endif
