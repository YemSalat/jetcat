#include "../../graphics/Graphics/LinuxFB/fb_surface.h"
#include "../../graphics/Graphics/Surface/bmp_surface.h"

int main(int argc, char* argv[])
{
	if (argc <= 1)
		return -1;
	LinuxFrameBuffer(framebuffer);
	Componentality::Graphics::BitmapSurface screenshot(framebuffer.getWidth(), framebuffer.getHeight());
	screenshot.apply(framebuffer);
	screenshot.write(argv[1]);
	return 0;
}
