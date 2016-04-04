#include "../../graphics/Graphics/LinuxFB/fb_surface.h"
#include "../../graphics/Graphics/LinuxFB/touchscreenlistener.h"
#include "../../graphics/Graphics/JetCat/window.h"
#include "../../graphics/Graphics/JetCat/inputs.h"
#include "../../graphics/Graphics/JetCat/messagebox.h"
#include "../../graphics/Graphics/JetCat/caption.h"
#include "../../graphics/Graphics/JetCat/background.h"
#include "../../graphics/Graphics/JetCat-Modules/touch.h"
#include <stdio.h>

using namespace Componentality::Graphics::LinuxFB;
using namespace Componentality::Graphics::JetCat;


std::string global_message = "";
std::string global_title = "";

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
	if (argc > 1)
		global_message = argv[1];
	if (argc > 2)
		global_title = argv[2];

	DEFAULT_WINDOW_MANAGER = new ::WindowManager;

	Componentality::Graphics::JetCat::Window& win = *new Componentality::Graphics::JetCat::Window(*DEFAULT_WINDOW_MANAGER);
	Componentality::Graphics::JetCat::MessageWindow& mwin = *new Componentality::Graphics::JetCat::MessageWindow(*DEFAULT_WINDOW_MANAGER, false);
	Componentality::Graphics::JetCat::Caption& caption = *new Componentality::Graphics::JetCat::Caption;
	Componentality::Graphics::JetCat::Background& background = *new Componentality::Graphics::JetCat::Background;
	win.add("Background", background);
	mwin.setText(global_message);
	caption.setTitle(global_title);
        caption.setColor(Componentality::Graphics::Color(0, 0, 255));

	mwin.add("Title", caption);

	DEFAULT_WINDOW_MANAGER->draw();

	CST::Common::sleep(500);

	return 0;
}
