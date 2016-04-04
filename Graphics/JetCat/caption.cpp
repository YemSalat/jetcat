#include "caption.h"
#include "textout.h"
#include "../Drawing/viewport.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

Caption::Caption() : 
	mHeight(System::getSystemProperties().getSize(System::SYSSIZE_CAPTION_HEIGHT))
	, mLineColor(System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_CAPTION_LINE))
	, mColor(System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_CAPTION_BACKGROUND)) 
	, mFontColor(System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_CAPTION_TEXT))
	, mFont(System::getSystemProperties().getFont(System::SYSFONT_WINDOW_CAPTION))
{
	mMargins = System::getSystemProperties().getMargins(System::SYSMARGINS_CAPTION_TEXT);
};


Margins Caption::draw(ISurface& surface)
{
	Point topleft(0, 0), rightbottom(surface.getWidth(), mHeight);
	ViewPort vp(surface, topleft, rightbottom);
	Drawer drawer(vp);
	Point _topleft(0, 0), _rightbottom(vp.getWidth() - 1, vp.getHeight() - 1);
	drawer.filled_rectangle(_topleft, _rightbottom, mColor);
	Drawer linedrawer(surface);
	topleft.y = mHeight;
	linedrawer.line(topleft, rightbottom, mLineColor);

	{
		ITextOutput* text_output = provideTextOutput(vp);
		text_output->setFont(mFont);
		text_output->setColor(mFontColor);
		text_output->setMargins(mMargins);
		System::Alignments align = System::getSystemProperties().getAlignments(System::SYSALIGN_CAPTION_TEXT);
		text_output->setHAlignment(align.first);
		text_output->setVAlignment(align.second);
		text_output->print(mTitle);
		delete text_output;
	}

	Margins results;
	results.mBottom = results.mLeft = results.mRight = 0;
	results.mTop = mHeight + 1;

	IElement::draw(vp);

	return results;
}

