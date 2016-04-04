#include "statusbar.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

StatusBar::StatusBar()
{
	mHeight = System::getSystemProperties().getSize(System::SYSSIZE_STATUS_HEIGHT);
	mLineColor = System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_STATUS_LINE);
	mColor = System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_STATUS_BACKGROUND);
	mFontColor = System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_STATUS_TEXT);
	mFont = System::getSystemProperties().getFont(System::SYSFONT_WINDOW_STATUS);
}

StatusBar::~StatusBar()
{
}

Margins StatusBar::draw(ISurface& surface)
{
	Point topleft(0, surface.getHeight() - mHeight), rightbottom(surface.getWidth(), surface.getHeight());
	ViewPort vp(surface, topleft, rightbottom);
	Drawer drawer(vp);
	Point _topleft(0, 0), _rightbottom(vp.getWidth() - 1, vp.getHeight() - 1);
	drawer.filled_rectangle(_topleft, _rightbottom, mColor);
	Drawer linedrawer(surface);
	topleft.y = rightbottom.y = surface.getHeight() - mHeight - 1;
	linedrawer.line(topleft, rightbottom, mLineColor);

	{
		ITextOutput* text_output = provideTextOutput(vp);
		text_output->setFont(mFont);
		text_output->setColor(mFontColor);
		text_output->setMargins(System::getSystemProperties().getMargins(System::SYSMARGINS_STATUS_TEXT));
		System::Alignments align = System::getSystemProperties().getAlignments(System::SYSALIGN_STATUS_TEXT);
		text_output->setHAlignment(align.first);
		text_output->setVAlignment(align.second);
		text_output->print(mTitle);
		delete text_output;
	}

	Margins results;
	results.mTop = results.mLeft = results.mRight = 0;
	results.mBottom = mHeight + 1;

	ViewPort port = getDrawArea(surface, results);
	IElement::draw(port);

	return results;
}
