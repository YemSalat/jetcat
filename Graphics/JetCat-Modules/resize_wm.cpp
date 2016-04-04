#include "resize_wm.h"
#include "touch.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

ScalingWindowManager::ScalingWindowManager(ISurface& surface, const double xscale, const double yscale)
	: WindowManager(*new ScaledSurface(surface, xscale, yscale))
{
	mXScaleFactor = xscale;
	mYScaleFactor = yscale;
}

ScalingWindowManager::~ScalingWindowManager()
{
	exit();
	while (!getExit())
		CST::Common::sleep(10);
	delete mSurface;
	mSurface = NULL;
}

bool ScalingWindowManager::onEvent(CST::Common::Event& event)
{
	if ((event.getType() == EVENT_TYPE_DEPRESS) ||
		(event.getType() == EVENT_TYPE_LONG_PRESS) ||
		(event.getType() == EVENT_TYPE_POINTER) ||
		(event.getType() == EVENT_TYPE_PRESS) ||
		(event.getType() == EVENT_TYPE_SHORT_PRESS))
	{
		Point point = ((PointerEvent&)event).get();
		double x = (double)point.x / mXScaleFactor;
		double y = (double)point.y / mYScaleFactor;
		((PointerEvent&)event).set(Point((int)x, (int)y));
	}
	return WindowManager::onEvent(event);
}

