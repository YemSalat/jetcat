#include "background.h"
#include "global.h"
#include "../Drawing/brush.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Background::Background() : mColor(System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_BACKGROUND))
{
}

Background::~Background()
{
}

Margins Background::draw(ISurface& surface)
{
	Point topleft(0, 0), bottomright(surface.getWidth() - 1, surface.getHeight() - 1);
	Drawer drawer(surface);
	drawer.filled_rectangle(topleft, bottomright, mColor);

	IElement::draw(surface);

	return Margins();
}
