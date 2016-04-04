#include "framebuf.h"

using namespace Componentality::Graphics;

IFrameBuffer::IFrameBuffer(ISurface& master,
	const size_t x_granularity, const size_t y_granularity) : ISurface(master.getWidth(), master.getHeight()),
	mMasterSurface(master)
{
	mXGranularity = x_granularity;
	mYGranularity = y_granularity;
	size_t xsegs = (getWidth() + x_granularity - 1) / x_granularity;
	size_t ysegs = (getHeight() + y_granularity - 1) / y_granularity;
	mChanged = new bool[xsegs * ysegs];
	for (size_t i = 0; i < xsegs * ysegs; i++)
		mChanged[i] = false;
};

IFrameBuffer::~IFrameBuffer()
{
	delete[] mChanged;
};


// Set individual pixel's color
void IFrameBuffer::plot(const size_t x, const size_t y, const Color& color)
{
	Color original_color = mMasterSurface.peek(x, y);
	changed(x, y) = (color != original_color);
	mMasterSurface.plot(x, y, color);
}

// Get individual pixel's color
Color IFrameBuffer::peek(const size_t x, const size_t y)
{
	return mMasterSurface.peek(x, y);
}

// Get width
size_t IFrameBuffer::getWidth() const
{
	return mMasterSurface.getWidth();
}

// Get height
size_t IFrameBuffer::getHeight() const
{
	return mMasterSurface.getHeight();
}

bool& IFrameBuffer::changed(const size_t x, const size_t y)
{
	size_t _x = x / mXGranularity;
	size_t _y = y / mYGranularity;
	size_t index = _y * ((getWidth() + mYGranularity - 1) / mYGranularity) + _x;
	return mChanged[index];
}

void IFrameBuffer::reset()
{
	size_t xsegs = (getWidth() + mXGranularity - 1) / mXGranularity;
	size_t ysegs = (getHeight() + mYGranularity - 1) / mYGranularity;
	mChanged = new bool[xsegs * ysegs];
	for (size_t i = 0; i < xsegs * ysegs; i++)
		mChanged[i] = false;
}

//////////////////////////////////////////////////////////////////////////////////////

GenericFrameBuffer::GenericFrameBuffer(ISurface& master, const size_t update_period) : mMasterSurface(master), mUpdatePeriod(update_period)
{
	mCache = new BitmapSurface(master.getWidth(), master.getHeight());
	mCache->apply(master);
	mExited = false;
	mStopUpdate = false;
	mChanged = true;
	run((CST::Common::thread::threadfunc)__process, this);
}

GenericFrameBuffer::~GenericFrameBuffer()
{
	exit();
	while (!mExited)
		CST::Common::sleep(10);
	delete mCache;
}

// Set individual pixel's color
void GenericFrameBuffer::plot(const size_t x, const size_t y, const Color& color)
{
	mCache->plot(x, y, color);
	mStopUpdate = true;
	mChanged = true;
}

// Get individual pixel's color
Color GenericFrameBuffer::peek(const size_t x, const size_t y)
{
	return mCache->peek(x, y);
}

// Get width
size_t GenericFrameBuffer::getWidth() const
{
	return mCache->getWidth();
}

// Get height
size_t GenericFrameBuffer::getHeight() const
{
	return mCache->getHeight();
}

void GenericFrameBuffer::__process(GenericFrameBuffer* master)
{
	while (!master->getExit())
	{
		CST::Common::sleep(master->mUpdatePeriod);
		if (!master->mStopUpdate)
		{
			if (master->mChanged)
			{
				master->mChanged = false;
				master->mMasterSurface.apply(*master->mCache);
			}
		}
		else
			master->mStopUpdate = false;
	}
	master->mExited = true;
}
