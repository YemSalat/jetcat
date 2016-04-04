#include "frame.h"
#include "global.h"
#include "../Drawing/brush.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Frame::Frame() : mWidth(1), mColor(System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_FRAME))
{
}

Frame::~Frame()
{
}

Margins Frame::draw(ISurface& surface)
{
	Point topleft(mWidth / 2, mWidth / 2), bottomright(surface.getWidth() - mWidth / 2 - 1, surface.getHeight() - mWidth / 2 - 1);
	AdvancedSurface asurface(surface);
	SquareBrush brush(mWidth);
	asurface.setBrush(&brush);
	Drawer drawer(asurface);
	drawer.rectangle(topleft, bottomright, mColor);
	Margins result;
	result.mBottom = result.mLeft = result.mRight = result.mTop = mWidth;

	ViewPort port = getDrawArea(surface, result);
	IElement::draw(port);
	return result;
}
