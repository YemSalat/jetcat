#ifndef __SDL_SURFACE_H__
#define __SDL_SURFACE_H__

#include "SDL.h"
#include "../Surface/surface.h"
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{
		class SDLFrameBuffer : public ISurface, public CST::Common::ThreadSet
		{
		protected:
			SDL_Surface* mSurface;
			bool mExited;
			bool mStopUpdate;
			bool mChanged;
			size_t mRefreshPeriod;
		public:
			// Create surface with given dimensions
			SDLFrameBuffer(const size_t period = 25);
			virtual ~SDLFrameBuffer();
		public:
			// Set individual pixel's color
			virtual void plot(const size_t x, const size_t y, const Color&);
			// Get individual pixel's color
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const;
			// Get height
			virtual size_t getHeight() const;
		protected:
			static void update(SDLFrameBuffer* master);
			static void init(SDLFrameBuffer* master);
		};

	} // namespace Graphics
} // namespace Componentality

#endif
