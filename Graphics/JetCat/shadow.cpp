#include "shadow.h"
#include "global.h"
#include "../Drawing/transparency.h"
#include "../Drawing/viewport.h"

using namespace Componentality::Graphics::JetCat;
using namespace Componentality::Graphics;

Margins Shadow::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();

	{
		Point lefttop(surface.getWidth() - mWidth, mWidth);
		Point rightbottom(surface.getWidth(), surface.getHeight() - mWidth - 1);
		ViewPort view(surface, lefttop, rightbottom);
		AdvancedSurface asurface(view);
		GradientBrush brush(255, 0, 255, 255);
		asurface.setBrush(&brush);
		Drawer drawer(asurface);
		Point _lefttop(0, 0), _rightbottom(asurface.getWidth() - 1, asurface.getHeight() - 1);
		drawer.filled_rectangle(_lefttop, _rightbottom, System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_SHADOW));
	}
	{
		Point lefttop(mWidth, surface.getHeight() - mWidth);
		Point rightbottom(surface.getWidth() - mWidth - 1, surface.getHeight());
		ViewPort view(surface, lefttop, rightbottom);
		AdvancedSurface asurface(view);
		GradientBrush brush(255, 255, 255, 0);
		asurface.setBrush(&brush);
		Drawer drawer(asurface);
		Point _lefttop(0, 0), _rightbottom(asurface.getWidth() - 1, asurface.getHeight() - 1);
		drawer.filled_rectangle(_lefttop, _rightbottom, Color(0, 0, 0));
	}
	{
		Point lefttop(surface.getWidth() - mWidth, surface.getHeight() - mWidth);
		Point rightbottom(surface.getWidth(), surface.getHeight());
		ViewPort view(surface, lefttop, rightbottom);
		AdvancedSurface asurface(view);
		GradientBrush brush(255, 0, 255, 0);
		asurface.setBrush(&brush);
		Drawer drawer(asurface);
		Point _lefttop(0, 0), _rightbottom(asurface.getWidth() - 1, asurface.getHeight() - 1);
		drawer.filled_rectangle(_lefttop, _rightbottom, Color(0, 0, 0));
	}
	Margins result;
	result.mLeft = result.mTop = 0;
	result.mBottom = result.mRight = mWidth;

	ViewPort port = getDrawArea(surface, result);
	IElement::draw(port);
	return result;
}
