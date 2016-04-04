#ifndef __TOUCHSCREENLISTENER_H__
#define __TOUCHSCREENLISTENER_H__

#include <string>
#include <linux/version.h>
#include <linux/input.h>
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace LinuxFB
		{

			class TouchscreenListener : public CST::Common::ThreadSet
			{
			public:
				enum MOUSE_EVENT_TYPE
				{
					MOUSE_DOWN = 1,
					MOUSE_UP
				};

				class Handler
				{
				public:
					virtual bool callback(int event_id, int x = 0, int y = 0, int z = 0) = 0;
				};

			public:
				TouchscreenListener(const char devicename[], Handler * handler = NULL);
				~TouchscreenListener();
			protected:
				static void thread_proc(TouchscreenListener* self);

			protected:
				std::string _devicename;
				Handler * _handler;
				int _fd;
			};

		}
	}
}

#endif // TOUCHSCREENLISTENER_H
