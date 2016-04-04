#include "window.h"
#include "global.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Window::Window(WindowManager& owner) 
{
	mPosition.x = mPosition.y = 0;
	mHeight = mWidth = (size_t)-1;
	std::string handle;
	void* _this = this;
	handle.assign((char*)&_this, sizeof(this));
	owner.add(handle, *this);
};

Window::~Window() 
{
	std::string handle;
	void* _this = this;
	handle.assign((char*)&_this, sizeof(this));
	mOwner->remove(handle);
};


Margins Window::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	if (mWidth == (size_t)-1)
		mWidth = surface.getWidth();
	if (mHeight == (size_t)-1)
		mHeight = surface.getHeight();
	Point bottomright = (mWidth != (size_t)-1) && (mHeight != (size_t)-1) ? Point(mPosition.x + mWidth - 1, mPosition.y + mHeight - 1) : IElement::getRightBottom();
	ViewPort vp(surface, mPosition, bottomright);
	mTopLeft.x += mPosition.x; mTopLeft.y += mPosition.y;
	mRightBottom.x = mTopLeft.x + mWidth - 1; mRightBottom.y = mTopLeft.y + mHeight - 1;
	return IElement::draw(vp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

WindowManager::WindowManager() : mSurface(NULL)
{
	subscribe(*this);
	run((CST::Common::thread::threadfunc) __process, this);
	run((CST::Common::thread::threadfunc) __timer, this);
}

WindowManager::WindowManager(ISurface& surface) : mSurface(&surface)
{
	subscribe(*this);
	run((CST::Common::thread::threadfunc) __process, this);
	run((CST::Common::thread::threadfunc) __timer, this);
}

WindowManager::~WindowManager()
{
	unsubscribe(*this);
	exit();
}

void WindowManager::draw()
{
	if (!mSurface)
		return;
	mLock.lock();
	setTopLeft(Point(0, 0));
	setRightBottom(Point(mSurface->getWidth() - 1, mSurface->getHeight() - 1));
	draw(*mSurface);
	mLock.unlock();
}
	
void WindowManager::__process(WindowManager* master)
{
	while (!master->getExit())
	{
		master->mLock.lock();
		if (!master->mQueue.empty())
		{
			master->dispose(master->processEvent());
			master->mLock.unlock();
		}
		else
		{
			master->mLock.unlock();
			CST::Common::sleep(10);
		}
	}
}

void WindowManager::__timer(WindowManager* master)
{
	while (!master->getExit())
	{
		CST::Common::sleep(100);
		master->sendEvent(*new TimerEvent());
		TimerEvent::appendGlobalCounter();
	}
}

void WindowManager::setSurface(ISurface& surface)
{
	mLock.lock();
	mSurface = &surface;
	mLock.unlock();
}

ISurface* WindowManager::getSurface() const
{
	mLock.lock();
	ISurface* result = mSurface;
	mLock.unlock();
	return result;
}

void WindowManager::reset()
{
	mLock.lock();
	ISurface* result = NULL;
	mLock.unlock();
}

bool WindowManager::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_KEY)														// Keyboard events are only sent to focus holder
		return sendEventToFocusHolder(event);
	if (event.getType() == EVENT_TYPE_DRAW_REQUEST)												// Draw event has a reduced path
	{
		DrawRequestEvent& dre = (DrawRequestEvent&)event;
		DrawRequestEvent::Initiator* initiator = dre.get();
		if (initiator && !initiator->mUpdateAll && mSurface)
		{
			IElement* to_draw = initiator->mInitiator;
			while (to_draw && to_draw->isMovable())												// Movable objects must be drawn with it's owner
				to_draw = to_draw->getOwner();
			ViewPort vp(*mSurface, to_draw->getTopLeft(), to_draw->getRightBottom());
			to_draw->draw(vp);
		}
		else 
			draw();
		event.setProcessed();
		return true;
	}
	else
		return IElement::onEvent(event);
}

//////////////////////////////////////////////////////////////////////////////////////////

bool ModalWindow::onEvent(CST::Common::Event& event)
{
	bool result = Window::onEvent(event);
	if (filter(event))
		if (((IInputEvent&)event).isNotToModal() && !isHidden())				// If message is to modal windows only
			((IInputEvent&)event).setProcessed();								// mark it as processed
	return result;
}

Margins ModalWindow::draw(ISurface& surface)
{
	if (mGreying && !isHidden())
	{
		for (size_t j = 0; j < surface.getHeight(); j++)
			for (size_t i = 0; i < surface.getWidth(); i++)
			{
				Color color = surface.peek(i, j);
				grey(i, j, color);
				surface.plot(i, j, color);
			}
	}
	return Window::draw(surface);
}

void ModalWindow::grey(const size_t x, const size_t y, Color& color)
{
	ColorRGB rgb = color;
	rgb.blue = (char)____min(255, ((int)rgb.blue + 60) * 2);
	rgb.red = (char)____min(255, ((int)rgb.red + 60) * 2);
	rgb.green = (char)____min(255, ((int)rgb.green + 60) * 2);
	color = rgb;
}