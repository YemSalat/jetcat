#ifndef __FB_SURFACE_H__
#define __FB_SURFACE_H__

#include "../Surface/surface.h"
#include "../framebuf/framebuf.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace LinuxFB
		{

			class FrameBuffer : public ISurface
			{
			protected:
				void* mScreenDescriptor;
			public:
				// Create surface with given dimensions
				FrameBuffer();
				virtual ~FrameBuffer();
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

		} // namespace LinuxFB
	} // namespace Graphics
} // namespace Componentality

#ifndef USE_SDL_FRAMEBUF
#define LinuxFrameBuffer(buffer)                                                \
		Componentality::Graphics::CommonFrameBuffer<Componentality::Graphics::LinuxFB::FrameBuffer> buffer;
#else
#include "../framebuf/sdl_surface.h"
#define LinuxFrameBuffer(buffer) Componentality::Graphics::SDLFrameBuffer buffer
#endif

#endif
