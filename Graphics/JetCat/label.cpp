#include "label.h"

using namespace Componentality::Graphics::JetCat;
using namespace Componentality::Graphics;

Label::Label()
{
	mAlignments.first = ITextOutput::H_LEFT;
	mAlignments.second = ITextOutput::V_CENTER;
	mFont = System::getSystemProperties().getFont(System::SYSFONT_WINDOW_CAPTION);
}

Label::Label(const std::string text)
{
	std::string _text = text;
	mText = stringToList(_text);
	mAlignments.first = ITextOutput::H_LEFT;
	mAlignments.second = ITextOutput::V_CENTER;
	mFont = System::getSystemProperties().getFont(System::SYSFONT_WINDOW_CAPTION);
}

Label::Label(const std::string text, Componentality::Graphics::Color color, Margins margins)
	: mMargins(margins), mColor(color)
{
	std::string _text = text;
	mText = stringToList(_text);
	mAlignments.first = ITextOutput::H_LEFT;
	mAlignments.second = ITextOutput::V_CENTER;
	mFont = System::getSystemProperties().getFont(System::SYSFONT_WINDOW_CAPTION);
}

Label::Label(const std::list<int> text)
{
	mText = text;
	mAlignments.first = ITextOutput::H_LEFT;
	mAlignments.second = ITextOutput::V_CENTER;
	mFont = System::getSystemProperties().getFont(System::SYSFONT_WINDOW_CAPTION);
} 

Label::Label(const std::list<int> text, Componentality::Graphics::Color color, Margins margins)
	: mMargins(margins), mColor(color)
{
	mText = text;
	mAlignments.first = ITextOutput::H_LEFT;
	mAlignments.second = ITextOutput::V_CENTER;
	mFont = System::getSystemProperties().getFont(System::SYSFONT_WINDOW_CAPTION);
}


Label::~Label()
{
}

Margins Label::draw(ISurface& surface)
{
	CST::Common::scoped_lock lock(mLock);
	if (isHidden())
		return Margins();
	ITextOutput& text_output = getTextOutput(surface);
	text_output.setFont(mFont);
	text_output.setColor(mColor);
	text_output.setMargins(System::getSystemProperties().getMargins(System::SYSMARGINS_CAPTION_TEXT));
	text_output.setHAlignment(mAlignments.first);
	text_output.setVAlignment(mAlignments.second);
	text_output.print(mText);
	delete &text_output;

	return IElement::draw(surface);
}

std::list<int> Label::getText() const 
{ 
	CST::Common::scoped_lock lock(mLock);
	return mText;
}

void Label::setText(const std::string text) 
{ 
	CST::Common::scoped_lock lock(mLock);
	std::string _text = text; 
	mText = Componentality::Graphics::stringToList(_text);
}

void Label::setText(const std::list<int> text) 
{ 
	CST::Common::scoped_lock lock(mLock);
	mText = text;
}

Componentality::Graphics::FontSet::FontDescriptor Label::getFont() const 
{ 
	CST::Common::scoped_lock lock(mLock);
	return mFont;
}

void Label::setFont(const Componentality::Graphics::FontSet::FontDescriptor font) 
{ 
	CST::Common::scoped_lock lock(mLock);
	mFont = font;
}

Margins Label::getMargins() const 
{ 
	CST::Common::scoped_lock lock(mLock);
	return mMargins;
}

void Label::setMargins(const Margins margins) 
{ 
	CST::Common::scoped_lock lock(mLock);
	mMargins = margins;
}

Componentality::Graphics::Color Label::getColor() const 
{ 
	CST::Common::scoped_lock lock(mLock);
	return mColor;
}

void Label::setColor(const Componentality::Graphics::Color color) 
{ 
	CST::Common::scoped_lock lock(mLock);
	mColor = color;
}

IAlignmentDefinitions::Alignments Label::getAlignments() const 
{ 
	return mAlignments; 
	CST::Common::scoped_lock lock(mLock);
}

void Label::setAlignment(const IAlignmentDefinitions::Alignments align) 
{ 
	CST::Common::scoped_lock lock(mLock);
	mAlignments = align;
}
