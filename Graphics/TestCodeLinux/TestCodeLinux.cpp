#include "fb_surface.h"

#include "../Surface/bmp_surface.h"
#include "../Surface/easy_draw.h"
#include "../Drawing/brush.h"
#include "../Drawing/viewport.h"
#include "../Drawing/sprite.h"
#include "../Fonts/character.h"
#include "../Fonts/FontSet.h"
#include "../Drawing/transparency.h"


using namespace Componentality::Graphics::LinuxFB;

int main(int argc, char* argv[])
{
	LinuxFrameBuffer(framebuffer);
	Componentality::Graphics::Drawer drawer(framebuffer);

        drawer.filled_rectangle(Componentality::Graphics::Point(0, 0),
                                Componentality::Graphics::Point(framebuffer.getWidth() - 1, framebuffer.getHeight() - 1),
				Componentality::Graphics::Color(0, 0, 0));
	drawer.point(Componentality::Graphics::Point(0, 0), Componentality::Graphics::Color(255, 0, 0));
	drawer.line(Componentality::Graphics::Point(50, 50), Componentality::Graphics::Point(250, 250), Componentality::Graphics::Color(255, 0, 0));
	drawer.rectangle(Componentality::Graphics::Point(50, 50), Componentality::Graphics::Point(250, 250), Componentality::Graphics::Color(0, 255, 0));
	drawer.filled_rectangle(Componentality::Graphics::Point(350, 350), Componentality::Graphics::Point(550, 550), Componentality::Graphics::Color(0, 0, 255));
	drawer.circle(Componentality::Graphics::Point(150, 150), 50, Componentality::Graphics::Color(0, 255, 0));
	drawer.filled_circle(Componentality::Graphics::Point(450, 450), 50, Componentality::Graphics::Color(255, 255, 255));

	return 0;
}
