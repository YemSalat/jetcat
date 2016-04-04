#include "touchscreenlistener.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace CST::Common;
using namespace Componentality::Graphics::LinuxFB;

TouchscreenListener::TouchscreenListener(const char devicename[], Handler* handler)
    : _devicename(devicename)
    , _handler(handler)
    , _fd(-1)
{
	run((thread::threadfunc)thread_proc, this);
}

TouchscreenListener::~TouchscreenListener()
{
    if (_fd >= 0)
        close(_fd);
	exit();
}

void TouchscreenListener::thread_proc(TouchscreenListener* self)
{
    struct input_event ev;
    struct timeval last_press_time, timeout;

	self->_fd = open(self->_devicename.c_str(), O_RDONLY | O_NONBLOCK);
	if (self->_fd < 0)
    {
        return;
    }

    fd_set rdfds, errfds;

    bool mouse_pressed = false;
    int x = -1, y = -1;

    while (!self->getExit()) {

        FD_ZERO(&rdfds);
        FD_ZERO(&errfds);
		FD_SET(self->_fd, &rdfds);
		FD_SET(self->_fd, &errfds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

		int retval = select(self->_fd + 1, &rdfds, 0, &errfds, &timeout);
        if (retval  == 0)
        {
            if (mouse_pressed)
            {
				if (self->_handler)
					self->_handler->callback(MOUSE_UP);
                mouse_pressed = false;
                x = -1;
                y = -1;
            }
            continue;
        }

        if (retval < 0)
        {
            switch (errno)
            {
            case EINTR:
                continue;
            }

            return;
        }

		if (!FD_ISSET(self->_fd, &rdfds))
            continue;
		if (FD_ISSET(self->_fd, &errfds))
        {
            return;
        }

        while (1)
        {
            int rd = read(self->_fd, &ev, sizeof(struct input_event));

            if (rd < 0)
            {
                int err = errno;

                if ( err == EINTR)
                    continue;
                if (err == EAGAIN)
                    break;

                return;
            }

            if (rd == 0)
                break;

            if (rd < (int) sizeof(struct input_event))
            {
                return;
            }

            if (mouse_pressed)
                continue;

            if (ev.type == EV_ABS)
            {
                if (x < 0 && ((ev.code == ABS_MT_POSITION_X) || (ev.code == ABS_X)))
                    x = ev.value;
				if (y < 0 && ((ev.code == ABS_MT_POSITION_Y) || (ev.code == ABS_Y)))
					y = ev.value;

                if (x >= 0 && y >= 0)
                {
                    mouse_pressed = true;
					if (self->_handler)
						self->_handler->callback(MOUSE_DOWN, x, y);
                }
            }
        }
    }
}
