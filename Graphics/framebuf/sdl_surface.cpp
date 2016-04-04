#include "sdl_surface.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace Componentality::Graphics;

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

// Create surface with given dimensions
SDLFrameBuffer::SDLFrameBuffer(const size_t period) : mRefreshPeriod(period)
{
	run((CST::Common::thread::threadfunc) init, this);
	int result = SDL_Init(SDL_INIT_VIDEO);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	mSurface = SDL_SetVideoMode(info->current_w, info->current_h, 24, SDL_SWSURFACE);
	SDL_ShowCursor(0);
	mExited = false;
	mStopUpdate = false;
	mChanged = true;
	run((CST::Common::thread::threadfunc) update, this);
}

SDLFrameBuffer::~SDLFrameBuffer()
{
	exit();
	while (!mExited)
		CST::Common::sleep(10);
	SDL_Quit();
}

void SDLFrameBuffer::plot(const size_t x, const size_t y, const Color& color)
{
	if ((x >= getWidth()) || (y >= getHeight()))
		return;
 	ColorRGB _color = color;
	Uint32 col = SDL_MapRGB(mSurface->format, _color.red, _color.green, _color.blue);
	putpixel(mSurface, x, y, col);
	mStopUpdate = true;
	mChanged = true;
}

Color SDLFrameBuffer::peek(const size_t x, const size_t y)
{
	if ((x >= getWidth()) || (y >= getHeight()))
		return Color();
	Uint32 col = getpixel(mSurface, x, y);
	ColorRGB rgb;
	SDL_GetRGB(col, mSurface->format, &rgb.red, &rgb.green, &rgb.blue);
	return rgb;
}

size_t SDLFrameBuffer::getWidth() const
{
	return mSurface->w;
}

size_t SDLFrameBuffer::getHeight() const
{
	return mSurface->h;
}

void SDLFrameBuffer::update(SDLFrameBuffer* master)
{
	SDL_Rect rect;
	rect.x = 0, rect.y = 0, rect.w = (Uint16)master->getWidth(), rect.h = (Uint16)master->getHeight();
	while (!master->getExit())
	{
		CST::Common::sleep(master->mRefreshPeriod);
		if (!master->mStopUpdate)
		{
			if (master->mChanged)
			{
				master->mChanged = false;
				SDL_LockSurface(master->mSurface);
				SDL_UpdateRects(master->mSurface, 1, &rect);
				SDL_UnlockSurface(master->mSurface);
			}
		}
		else
			master->mStopUpdate = false;
	}
	master->mExited = true;
}

void SDLFrameBuffer::init(SDLFrameBuffer* master)
{
	CST::Common::sleep(100);
	// Hacking patch for SDL stuck in Ubuntu Nitrogen
#ifndef WIN32
	kill(getpid(), SIGINT);
#endif
}
