#include "../../graphics/Graphics/LinuxFB/fb_surface.h"
#include "../../graphics/Graphics/Surface/bmp_surface.h"

int main(int argc, char* argv[])
{
	if (argc <= 1)
		return -1;
	LinuxFrameBuffer(framebuffer);
	Componentality::Graphics::BitmapSurface bitmap;
	bitmap.read(argv[1]);
	framebuffer.apply(bitmap);
	return 0;
}
