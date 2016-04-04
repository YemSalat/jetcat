#ifdef WIN32
#error "FB_SURFACE can be only used in Linux"
#endif

#include "fb_surface.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::LinuxFB;

const char* FRAMEBUFFER_DEVICE = "/dev/fb0";

struct ScreenDescription
{
	int fbfd;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize;
	char *fbp;
};

// Create surface with given dimensions
FrameBuffer::FrameBuffer()
{
	ScreenDescription& dsc = *new ScreenDescription;
	dsc.fbfd = 0;
	dsc.screensize = 0;
	dsc.fbp = 0;

	// Open the file for reading and writing
	dsc.fbfd = open(FRAMEBUFFER_DEVICE, O_RDWR);
	if (dsc.fbfd == -1) {
		printf("Error: cannot open framebuffer device");
	}

	// Get fixed screen information
	if (ioctl(dsc.fbfd, FBIOGET_FSCREENINFO, &dsc.finfo) == -1) {
		printf("Error reading fixed information");
	}

	// Get variable screen information
	if (ioctl(dsc.fbfd, FBIOGET_VSCREENINFO, &dsc.vinfo) == -1) {
		printf("Error reading variable information");
	}

	// Figure out the size of the screen in bytes
	dsc.screensize = dsc.vinfo.xres * dsc.vinfo.yres * dsc.vinfo.bits_per_pixel / 8;

	mScreenDescriptor = &dsc;

	// Map the device to memory
	dsc.fbp = (char *)mmap(0, dsc.screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
		dsc.fbfd, 0);
	if (dsc.fbp == (char*)-1) {
		printf("Error: failed to map framebuffer device to memory");
	}
}

FrameBuffer::~FrameBuffer()
{
	ScreenDescription& dsc = *(ScreenDescription*)mScreenDescriptor;
	munmap(dsc.fbp, dsc.screensize);
	close(dsc.fbfd);
	delete &dsc;
}

void FrameBuffer::plot(const size_t x, const size_t y, const Color& color)
{
	ScreenDescription& dsc = *(ScreenDescription*)mScreenDescriptor;
	if ((x >= dsc.vinfo.xres) ||
		(y >= dsc.vinfo.yres))
	{
		onErrorBoundaries(x, y);
		return;
	}

	long location = (x + dsc.vinfo.xoffset) * (dsc.vinfo.bits_per_pixel / 8) +
		(y + dsc.vinfo.yoffset) * dsc.finfo.line_length;

	ColorRGB c = color;

	if (dsc.vinfo.bits_per_pixel == 32) {
		*(dsc.fbp + location) = c.blue;        
		*(dsc.fbp + location + 1) = c.green;    
		*(dsc.fbp + location + 2) = c.red;
		*(dsc.fbp + location + 3) = 0;
	}
	else  { //assume 16bpp
		unsigned short int t = ((c.red >> 3) & 0x1F) << 11 | ((c.green >> 3) & 0x1F) << 5 | ((c.blue >> 3) & 0x1F);
		*((unsigned short int*)(dsc.fbp + location)) = t;
	}

}

Color FrameBuffer::peek(const size_t x, const size_t y)
{
	ScreenDescription& dsc = *(ScreenDescription*)mScreenDescriptor;
	if ((x >= dsc.vinfo.xres) ||
		(y >= dsc.vinfo.yres))
	{
		onErrorBoundaries(x, y);
		return Color(0, 0, 0);
	}
	long location = (x + dsc.vinfo.xoffset) * (dsc.vinfo.bits_per_pixel / 8) +
		(y + dsc.vinfo.yoffset) * dsc.finfo.line_length;
	ColorRGB c;
	if (dsc.vinfo.bits_per_pixel == 32) {
		c.blue = *(dsc.fbp + location);
		c.green = *(dsc.fbp + location + 1);
		c.red = *(dsc.fbp + location + 2);
	}
	else  { //assume 16bpp
		unsigned short int t = *((unsigned short int*)(dsc.fbp + location));
		c.red = ((t >> 11) & (0x1F)) << 3;
		c.green = ((t >> 5) & (0x1F)) << 3; 
		c.blue = (t & (0x1F)) << 3;
	}
	return c;
}

size_t FrameBuffer::getWidth() const
{
	ScreenDescription& dsc = *(ScreenDescription*)mScreenDescriptor;
	return dsc.vinfo.xres;
}

size_t FrameBuffer::getHeight() const
{
	ScreenDescription& dsc = *(ScreenDescription*)mScreenDescriptor;
	return dsc.vinfo.yres;
}
