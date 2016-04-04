/********************************************************************************/
/* Portable Graphics Library for Embedded Systems * (C) Componentality Oy, 2015 */
/* Initial design and development: Konstantin A. Khait                          */
/* Support, comments and questions: dev@componentality.com                      */
/********************************************************************************/

#include "surface.h"

using namespace Componentality::Graphics;

Color::Color()
{
	mType = COLOR_UNDEFINED;
}

Color::Color(const ColorRGB ColorRGB)
{
	operator=(ColorRGB);
}

Color::Color(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	ColorRGB ColorRGB;
	ColorRGB.blue = blue; ColorRGB.green = green; ColorRGB.red = red;
	operator=(ColorRGB);
}

Color::Color(const Greyscale intensity)
{
	operator=(intensity);
}

Color::Color(const unsigned char greyscale)
{
	Greyscale gs;
	gs.intensity = greyscale;
	operator=(gs);
}

Color::Color(const Color& source)
{
	mType = source.mType;
	if (mType == COLOR_ColorRGB)
		mColorRGB = source.mColorRGB;
	else
		mGreyscale = source.mGreyscale;
}

Color::~Color()
{

}

bool Color::operator==(const Color& src) const
{
	return (mColorRGB.red == src.mColorRGB.red) &&
		(mColorRGB.green == src.mColorRGB.green) &&
		(mColorRGB.blue == src.mColorRGB.blue);
}

bool Color::operator!=(const Color& src) const
{
	return !operator==(src);
}

Color& Color::operator=(const ColorRGB ColorRGB)
{
	mType = COLOR_ColorRGB;
	mColorRGB = ColorRGB;
	return *this;
}

Color& Color::operator=(const Greyscale intensity)
{
	mType = COLOR_GREYSCALE;
	mGreyscale = intensity;
	return *this;
}

Color& Color::operator=(const Color& source)
{
	mType = source.mType;
	if (mType == COLOR_ColorRGB)
		mColorRGB = source.mColorRGB;
	else
		mGreyscale = source.mGreyscale;
	return *this;;
}

Color::operator Greyscale() const
{
	if (mType == COLOR_GREYSCALE)
		return mGreyscale;
	if (mType == COLOR_ColorRGB)
	{
		// Recalculation of ColorRGB color to greyscale according to color's visibility coefficients
		double value = 0.2126 * (double)mColorRGB.red + 0.7152 * (double)mColorRGB.green + 0.0722 * (double)mColorRGB.blue;
		Greyscale result;
		result.intensity = (unsigned char)(int)value;
		return result;
 	}
	Greyscale result;
	result.intensity = 0;
	return result;
}

Color::operator ColorRGB() const
{
	if (mType == COLOR_ColorRGB)
		return mColorRGB;
	if (mType == COLOR_GREYSCALE)
	{
		ColorRGB result;
		result.red = result.green = result.blue = mGreyscale.intensity;
		return result;
	}
	ColorRGB result;
	result.red = result.green = result.blue = 0;
	return result;
}

void ISurface::apply(ISurface& source)
{
	onApply();
	source.onApply();
	for (size_t i = 0; i < ____min(source.getWidth(), getWidth()); i++)
		for (size_t j = 0; j < ____min(source.getHeight(), getHeight()); j++)
			plot(i, j, source.peek(i, j));
	source.onApplied();
	onApplied();
}
