#include "scroller.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Scroller::Scroller(const int h, const int v) : mXOffset(h), mYOffset(v)
{
}

Scroller::~Scroller()
{
}

Margins Scroller::draw(ISurface& surface)
{
	mTopLeft.x += mXOffset;
	mTopLeft.y += mYOffset;
	mRightBottom.x += mXOffset;
	mRightBottom.y += mYOffset;
	MovingPort mp(surface, mXOffset, mYOffset);
	return Placement::draw(mp);
}

void Scroller::scroll(const int h, const int v)
{
	mXOffset = h;
	mYOffset = v;
	SetDefaultEvent defevent;
	send(defevent);
}

std::pair<int, int> Scroller::getScroll() const
{
	return std::pair<int, int>(mXOffset, mYOffset);
}
