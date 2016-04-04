#ifndef __INTEGRATION_H__
#define __INTEGRATION_H__

#include "../JetCat/window.h"
#include "../JetCat-Modules/resize_wm.h"
#include "../Surface/bmp_surface.h"
#include <WindowsX.h>

namespace Componentality
{
	namespace Graphics
	{
		namespace Windows
		{
			class WindowManager : public Componentality::Graphics::JetCat::WindowManager
			{
			protected:
				HWND mWnd;
			public:
				WindowManager(ISurface&, HWND);
				virtual ~WindowManager();
				virtual void draw();
			protected:
				virtual bool onEvent(CST::Common::Event&);
			};

			extern Componentality::Graphics::JetCat::WindowManager* DEFAULT_WINDOW_MANAGER;
			void init(HWND wnd);
			void draw(HDC, ISurface&);
			void draw(HDC);
		} // namespace Windows
	} // namespace Graphics
} // namespace Componentality

#endif