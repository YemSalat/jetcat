#include "cursor.h"
#include "inputs.h"

using namespace Componentality::Graphics::JetCat;
using namespace Componentality::Graphics;

Cursor::Cursor()
{
	mWidth = mHeight = 0;
	mPeriod = 5;
	mTimeSinceToggle = 0;
	mShown = false;
}

Cursor::Cursor(const Point location)
{
	mLocation = location;
	mWidth = mHeight = 0;
	mPeriod = 5;
	mTimeSinceToggle = 0;
	mShown = false;
}

Cursor::Cursor(const Point location, const size_t width, const size_t height)
{
	mLocation = location;
	mWidth = width;
	mHeight = height;
	mPeriod = 5;
	mTimeSinceToggle = 0;
	mShown = false;
}

Cursor::Cursor(const Point location, const size_t width, const size_t height, const Color color)
{
	mLocation = location;
	mWidth = width;
	mHeight = height;
	mColor = color;
	mPeriod = 5;
	mTimeSinceToggle = 0;
	mShown = false;
}

Cursor::~Cursor()
{
}

Margins Cursor::draw(ISurface& surface)
{
	if (!mWidth || !mHeight || isHidden())
		return Margins();
	CST::Common::scoped_lock lock(mLock);
	ViewPort vp(surface, mLocation, mLocation + Point(mWidth - 1, mHeight - 1));
	if (mShown)
	{
		Drawer drawer(vp);
		if (!mStorage)
			backup(vp);
		drawer.filled_rectangle(Point(0, 0), Point(mWidth - 1, mHeight - 1), mColor);
	}
	else
		restore(vp);
	return Margins();
}

void Cursor::setSize(const size_t width, const size_t height)
{
	mWidth = width;
	mHeight = height;
}

void Cursor::setLocation(const Point location)
{
	mLocation = location;
}

bool Cursor::onEvent(CST::Common::Event& event)
{
	if (event.getType() == EVENT_TYPE_TIMER)
	{
		if (isHidden())
		{
			reset();
			return true;
		}
		CST::Common::scoped_lock lock(mLock);
		if (!mTimeSinceToggle)
		{
			mTimeSinceToggle = mPeriod;
			mShown = !mShown;
			if (!isHidden())
				raise(*new DrawRequestEvent(this));
		}
		else
			mTimeSinceToggle -= 1;
		return true;
	}
	else
		return IElement::onEvent(event);
}

void Cursor::reset()
{
	CST::Common::scoped_lock lock(mLock);
	mTimeSinceToggle = 1;
	mShown = false;
	Restorable::reset();
	if (!isHidden())
		raise(*new DrawRequestEvent(this));
}
