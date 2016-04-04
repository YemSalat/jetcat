#include "touch.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

extern const std::string Componentality::Graphics::JetCat::EVENT_TYPE_LONG_PRESS = ".evt.lpress";		// Event generated upon long press
extern const std::string Componentality::Graphics::JetCat::EVENT_TYPE_SHORT_PRESS = ".evt.spress";		// Event generated upon short press

#define CALIBRATION_GAP 30

bool TouchEventsProcessor::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_PRESS)
	{
		mTicksAfterPressed = 1;
		mPoint = ((PointerEvent&)event).get();
		if (mSourceEventTransfer)
		{
			return Container::onEvent(event);
		}
		else
		{
			event.setProcessed();
			return true;
		}
	}
	else if (event.getType() == EVENT_TYPE_DEPRESS)
	{
		bool short_press = false;
		if (mTicksAfterPressed - 1 <= mLongPressBound / 100)
		{
			short_press = true;
			ShortPressEvent& spevent = *new ShortPressEvent;
			spevent.set(((PointerEvent&)event).get());
			if (!mSourceEventTransfer)
			{
				PressEvent pevent;
				pevent.set(((PointerEvent&)event).get());
				send(pevent);
			}
			raise(spevent);
		}
		mTicksAfterPressed = 0;
		if (mSourceEventTransfer || short_press)
		{
			return Container::onEvent(event);
		}
		else
		{
			event.setProcessed();
			return true;
		}
	}
	else if (event.getType() == EVENT_TYPE_TIMER)
	{
		if (mTicksAfterPressed)
		{
			if (mTicksAfterPressed - 1 == mLongPressBound / 100)
			{
				LongPressEvent& lpevent = *new LongPressEvent;
				lpevent.set(mPoint);
				raise(lpevent);
			}
			mTicksAfterPressed += 1;
		}
	}
	return Container::onEvent(event);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TouchCalibrator::TouchCalibrator(const double kx, const double ky, const double bx, const double by)
{
	set(kx, ky, bx, by);
}

TouchCalibrator::TouchCalibrator(const TouchCalibrator::CALIBRATIONS& calibrations)
{
	operator=(calibrations);
}

TouchCalibrator::TouchCalibrator(const std::string& filename)
{
	if (!read(filename))
		set(1.0, 1.0, 0.0, 0.0);
}

TouchCalibrator::~TouchCalibrator()
{
}

TouchCalibrator& TouchCalibrator::operator=(const TouchCalibrator::CALIBRATIONS& calibrations)
{
	mCalibrations = calibrations;
	return *this;
}

void TouchCalibrator::set(const double kx, const double ky, const double bx, const double by)
{
	mCalibrations.mKX = kx;
	mCalibrations.mKY = ky;
	mCalibrations.mBX = bx;
	mCalibrations.mBY = by;
}

bool TouchCalibrator::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if ((event.getType() == EVENT_TYPE_DEPRESS) ||
		(event.getType() == EVENT_TYPE_LONG_PRESS) ||
		(event.getType() == EVENT_TYPE_POINTER) ||
		(event.getType() == EVENT_TYPE_PRESS) ||
		(event.getType() == EVENT_TYPE_SHORT_PRESS))
	{
		Point point = ((PointerEvent&)event).get();
		double x = (double) point.x - mCalibrations.mBX;
		double y = (double) point.y - mCalibrations.mBY;
		x /= mCalibrations.mKX;
		y /= mCalibrations.mKY;
		((PointerEvent&)event).set(Point((int) x, (int) y));
	}
	return Container::onEvent(event);
}

bool TouchCalibrator::write(const std::string& filename)
{
	CST::Common::blob to_write;
	to_write.mData = (char*)&mCalibrations;
	to_write.mSize = sizeof(mCalibrations);
	return CST::Common::fileWrite(filename, to_write);
}

bool TouchCalibrator::read(const std::string& filename)
{
	CST::Common::blob read = CST::Common::fileRead(filename);
	if (read.mSize < sizeof(mCalibrations))
	{
		read.purge();
		return false;
	}
	mCalibrations = *(CALIBRATIONS*)read.mData;
	read.purge();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////

Calibrate::Calibrate(const std::string cal_file) : mCalibrationFile(cal_file)
{
	mLeftTop = mRightTop = mLeftBottom = mRightBottom = NULL;
	mWidth = 0;
	mHeight = 0;
	mCleaned = mCalibrated = read(mCalibrationFile);
}

Calibrate::~Calibrate()
{
	if (mLeftTop)
		delete mLeftTop;
	if (mRightTop)
		delete mRightTop;
	if (mLeftBottom)
		delete mLeftBottom;
	if (mRightBottom)
		delete mRightBottom;
}
	
Margins Calibrate::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	if (mCalibrated && mCleaned)
		return Container::draw(surface);
	if (!mWidth)
		mWidth = surface.getWidth();
	if (!mHeight)
		mHeight = surface.getHeight();
	if (!mLeftTop)
		drawTarget(surface, Point(CALIBRATION_GAP, CALIBRATION_GAP));
	else if (!mRightTop)
		drawTarget(surface, Point(mWidth - CALIBRATION_GAP, CALIBRATION_GAP));
	else if (!mLeftBottom)
		drawTarget(surface, Point(CALIBRATION_GAP, mHeight - CALIBRATION_GAP));
	else if (!mRightBottom)
		drawTarget(surface, Point(mWidth - CALIBRATION_GAP, mHeight - CALIBRATION_GAP));
	else if (!mCleaned)
		clear(surface);
	else
		return Container::draw(surface);
	return Margins();
}

bool Calibrate::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if ((event.getType() == EVENT_TYPE_PRESS) && (!mCalibrated))
	{
		PointerEvent& pevent = (PointerEvent&)event;
		if (!mLeftTop)
			mLeftTop = new Point(pevent.get());
		else if (!mRightTop)
			mRightTop = new Point(pevent.get());
		else if (!mLeftBottom)
			mLeftBottom = new Point(pevent.get());
		else if (!mRightBottom)
		{
			mRightBottom = new Point(pevent.get());
			calibrate(*mLeftTop, *mRightBottom, Point(CALIBRATION_GAP, CALIBRATION_GAP), Point(mWidth - CALIBRATION_GAP, mHeight - CALIBRATION_GAP));
			mCalibrated = write(mCalibrationFile);
		}
		raise(*new DrawRequestEvent(this));
	}
	return Container::onEvent(event);
}

void Calibrate::drawTarget(ISurface& surface, Point point)
{
	Drawer drawer(surface);
	drawer.filled_rectangle(Point(0, 0), Point(surface.getWidth(), surface.getHeight()), Color(0, 0, 0));
	drawer.circle(point, CALIBRATION_GAP / 2, Color(255, 255, 255));
	drawer.line(point - Point(CALIBRATION_GAP, 0), point + Point(CALIBRATION_GAP, 0), Color(255, 255, 255));
	drawer.line(point - Point(0, CALIBRATION_GAP), point + Point(0, CALIBRATION_GAP), Color(255, 255, 255));
}

void Calibrate::calibrate(double p1, double p2, double a1, double a2, double& k, double& b)
{
	k = (p2 - p1) / (a2 - a1);
	b = p2 - k * a2;
}

void Calibrate::calibrate(Point& pressed1, Point& pressed2, Point drawn1, Point drawn2)
{
	calibrate(mLeftTop->x, mRightBottom->x, drawn1.x, drawn2.x, mCalibrations.mKX, mCalibrations.mBX);
	calibrate(mLeftTop->y, mRightBottom->y, drawn1.y, drawn2.y, mCalibrations.mKY, mCalibrations.mBY);
}

void Calibrate::clear(ISurface& surface)
{
	Drawer drawer(surface);
	drawer.filled_rectangle(Point(0, 0), Point(surface.getWidth(), surface.getHeight()), Color(0, 0, 0));
	mCleaned = true;
}
