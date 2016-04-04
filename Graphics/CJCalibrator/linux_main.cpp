#include "../../graphics/Graphics/LinuxFB/fb_surface.h"
#include "../../graphics/Graphics/LinuxFB/touchscreenlistener.h"
#include "../../graphics/Graphics/JetCat/window.h"
#include "../../graphics/Graphics/JetCat/inputs.h"
#include "../../graphics/Graphics/JetCat-Modules/touch.h"
#include <stdio.h>

using namespace Componentality::Graphics::LinuxFB;
using namespace Componentality::Graphics::JetCat;

class WindowManager : public Componentality::Graphics::JetCat::WindowManager,
	public Componentality::Graphics::LinuxFB::TouchscreenListener,
	public Componentality::Graphics::LinuxFB::TouchscreenListener::Handler
{
protected:
	LinuxFrameBuffer(framebuffer);
public:
	WindowManager() : Componentality::Graphics::LinuxFB::TouchscreenListener("/dev/input/event0")
	{
		setSurface(framebuffer);
		_handler = this;
	};
	virtual ~WindowManager() 
	{
	};
protected:
	virtual bool callback(int event_id, int x = 0, int y = 0, int z = 0)
	{
		printf("Event %d, X=%d, Y=%d, Z=%d\n", event_id, x, y, z);
		if (event_id == MOUSE_UP)
			return true;
		Componentality::Graphics::JetCat::PointerEvent* event = new Componentality::Graphics::JetCat::PressEvent(
			Componentality::Graphics::Point(x, y));
		sendEvent(*event);
		return true;
	};
};


Componentality::Graphics::JetCat::WindowManager* DEFAULT_WINDOW_MANAGER = NULL;

int main(int argc, char* argv[])
{
	DEFAULT_WINDOW_MANAGER = new ::WindowManager;

	Componentality::Graphics::JetCat::Window& painter = *new Componentality::Graphics::JetCat::Window(*DEFAULT_WINDOW_MANAGER);
	Componentality::Graphics::JetCat::TouchCalibrator& tcal = *new Componentality::Graphics::JetCat::Calibrate;

	painter.add("tcal", tcal);

	DEFAULT_WINDOW_MANAGER->draw();

	// Event waiting loop
	while (true)
	{
		CST::Common::sleep(25);
	}

	return 0;
}
