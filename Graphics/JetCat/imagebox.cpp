#include "imagebox.h"
#include "../Surface/bmp_surface.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Image::Image()
{
}

Image::~Image()
{
}

Margins Image::draw(ISurface& surface)
{
	if (!isHidden())
	{
		BitmapSurface picture;
		if (picture.read(mFilename))
			surface.apply(picture);
	}
	return Margins();
}
