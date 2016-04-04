#include "placement.h"
#include "../Drawing/viewport.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Placement::Placement()
{
	mWidth = mHeight = (size_t)-1;
};

Placement::Placement(const size_t x, const size_t y, const size_t width, const size_t height)
{
	move(x, y);
	size(width, height);
}

Placement::~Placement()
{
}

Margins Placement::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	if ((mWidth != (size_t)-1) && (mHeight != (size_t)-1))
	{
		Point bottomright = (mWidth != (size_t)-1) && (mHeight != (size_t)-1) ? Point(mPosition.x + mWidth - 1, mPosition.y + mHeight - 1) : IElement::getRightBottom();
		ViewPort vp(surface, mPosition, bottomright);
		mTopLeft.x += mPosition.x; mTopLeft.y += mPosition.y;
		mRightBottom.x = mTopLeft.x + mWidth - 1; mRightBottom.y = mTopLeft.y + mHeight - 1;
		IElement::draw(vp);
	}
	else
	{
		MovingPort mp(surface, mPosition);
		mTopLeft.x += mPosition.x; mTopLeft.y += mPosition.y;
		mRightBottom = Point(surface.getWidth(), surface.getHeight()) - mPosition + mTopLeft;
		IElement::draw(mp);
	}
	return Margins();
}

///////////////////////////////////////////////////////////////////////////////////////////

FastPlacement::FastPlacement()
{
}

FastPlacement::~FastPlacement()
{
}

Margins FastPlacement::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();

	ISurface* dst;
	if ((mWidth != (size_t)-1) && (mHeight != (size_t)-1))
	{
		Point bottomright = (mWidth != (size_t)-1) && (mHeight != (size_t)-1) ? Point(mPosition.x + mWidth - 1, mPosition.y + mHeight - 1) : IElement::getRightBottom();
		dst = new ViewPort(surface, mPosition, bottomright);
	}
	else
		dst = new MovingPort(surface, mPosition);

	if (Restorable::mStorage)
	{
		Restorable::restore(*dst);
	}
	else
	{
		mMargins = Placement::draw(surface);
		Restorable::backup(*dst);
	}
	delete dst;
	return mMargins;
}

void FastPlacement::raise(CST::Common::Event& event)
{
	if (event.getType() == EVENT_TYPE_DRAW_REQUEST)
	{
		Restorable::reset();
	}
	Placement::raise(event);
}

bool FastPlacement::onEvent(CST::Common::Event& event)
{
	if (event.getType() == EVENT_TYPE_SET_DEFAULT)
		reset();
	return Placement::onEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

Margins Substrate::draw(ISurface& surface)
{
	if (!mStorage)
		backup(surface);
	else
		restore(surface);
	return IElement::draw(surface);
}

bool Substrate::onEvent(CST::Common::Event& event)
{
	if (event.getType() == EVENT_TYPE_SET_DEFAULT)
		reset();
	return IElement::onEvent(event);
}
