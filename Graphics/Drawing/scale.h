/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/
/* Definitions of the surface which upscales or downscales image being output   */
/* to it. Lets simply scale image without changing of the painting algorithms   */
/* and approaches.                                                              */
/********************************************************************************/

#ifndef __SCALE_H__
#define __SCALE_H__

#include "../Surface/surface.h"

namespace Componentality
{
	namespace Graphics
	{

		class ScaledSurface : public ISurface
		{
		protected:
			double mXScaleFactor;
			double mYScaleFactor;
			ISurface& mMasterSurface;
		protected:
			ScaledSurface() : mMasterSurface(*(ISurface*)NULL) {};
		public:
			// Create surface with given dimensions
			ScaledSurface(ISurface& master, const double scale_factor_x = 1.0, const double scale_factor_y = 1.0) : 
				mMasterSurface(master), 
				mXScaleFactor(scale_factor_x),
				mYScaleFactor(scale_factor_y) 
			{};
			virtual ~ScaledSurface() {};
		public:
			// Set individual pixel's color
			virtual void plot(const size_t x, const size_t y, const Color&);
			// Get individual pixel's color
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const;
			// Get height
			virtual size_t getHeight() const;
		};

	} // namespace Graphics
} // namespace Componentality

#endif