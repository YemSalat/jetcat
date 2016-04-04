#include "textout.h"
#include "global.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

std::pair<size_t, size_t> ITextOutput::measure(const std::string& text)
{
	return measure(stringToList(text));
}

std::pair<size_t, size_t> ITextOutput::measure(const std::list<int>& text)
{
	Font* font = System::getSystemProperties().getFontSet().find(mFont);
	if (!font)
		return std::pair<size_t, size_t>(0, 0);
	return font->measure(text);
}

///////////////////////////////////////////////////////////////////////////////////////////

std::list<int> SingleLineCutter::operator()(ISurface& surface, const std::string text)
{
	std::list<int> list;
	for (size_t i = 0; i < text.size(); i++)
		list.push_back(text[i]);
	return operator()(surface, list);
}

std::list<int> SingleLineCutter::operator()(ISurface& surface, const std::list<int>& text)
{
	Font* font = mFontSet.find(mFont);
	if (!font)
		return text;
	std::list<int> cut_text = text;
	std::pair<size_t, size_t> textsize = font->measure(cut_text);
	size_t width = surface.getWidth();
	while (textsize.first > width)
	{
		cut_text.pop_back();
		textsize = font->measure(cut_text);
	}
	return cut_text;
}

std::list<int> SingleLineEllipsisCutter::operator()(ISurface& surface, const std::list<int>& text)
{
	Font* font = mFontSet.find(mFont);
	if (!font)
		return text;
	std::list<int> cut_text = text;
	std::pair<size_t, size_t> textsize = font->measure(cut_text);
	size_t width = surface.getWidth();
	std::list<int> cut_text_save = cut_text;
	while (textsize.first > width)
	{
		cut_text = cut_text_save;
		cut_text.pop_back();
		cut_text_save = cut_text;
		cut_text.push_back('.');
		cut_text.push_back('.');
		cut_text.push_back('.');
		textsize = font->measure(cut_text);
	}
	return cut_text;
}

ITextCutter* StandardTextOutput::getTextCutter(FontSet& fontset, FontSet::FontDescriptor& font)
{
	return new SingleLineEllipsisCutter(fontset, font);
}

StandardTextOutput::StandardTextOutput(ISurface& surface) : EasyTextOutput(surface)
{
}

StandardTextOutput::~StandardTextOutput()
{
}

void StandardTextOutput::print(const std::list<int>& _text)
{
	ViewPort _vp(mSurface, Point(mMargins.mLeft, mMargins.mTop), Point(mSurface.getWidth() - mMargins.mRight, mSurface.getHeight() - mMargins.mBottom));
	ITextCutter* cutter = getTextCutter(System::getSystemProperties().getFontSet(), mFont);
	std::list<int> text;
	if (cutter)
	{
		text = cutter->operator()(_vp, _text);
		delete cutter;
	}
	else
		text = _text;
	EasyTextOutput::print(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////

EasyTextOutput::EasyTextOutput(ISurface& surface) : ITextOutput(surface)
{
}

EasyTextOutput::~EasyTextOutput()
{
}

void EasyTextOutput::print(const std::string& text)
{
	print(stringToList(text));
}

void EasyTextOutput::print(const std::list<int>& text)
{
	ViewPort _vp(mSurface, Point(mMargins.mLeft, mMargins.mTop), Point(mSurface.getWidth() - mMargins.mRight, mSurface.getHeight() - mMargins.mBottom));

	Font* font = System::getSystemProperties().getFontSet().find(mFont);
	if (!font)
		return;
	std::pair<size_t, size_t> text_size = std::pair<size_t, size_t>(0, 0);

	Point topleft;
	Point bottomright;
	switch (mHAlign)
	{
	case H_JUSTIFY:
	case H_LEFT:
		topleft.x = mMargins.mLeft;
		bottomright.x = mSurface.getWidth() - mMargins.mRight;
		break;
	case H_RIGHT:
		text_size = font->measure(text);
		bottomright.x = mSurface.getWidth() - mMargins.mRight;
		if (bottomright.x > (int)(text_size.first + mMargins.mLeft))
			topleft.x = bottomright.x - text_size.first;
		else
			topleft.x = mMargins.mLeft;
		break;
	case H_CENTER:
		text_size = font->measure(text);
		if (mSurface.getWidth() - mMargins.mLeft - mMargins.mRight < text_size.first)
		{
			topleft.x = mMargins.mLeft;
			bottomright.x = mSurface.getWidth() - mMargins.mRight;
		}
		else
		{
			topleft.x = ((mSurface.getWidth() - mMargins.mLeft - mMargins.mRight) - text_size.first) / 2 + mMargins.mLeft;
			bottomright.x = mSurface.getWidth() - mMargins.mRight;
		}
		break;
	}

	switch (mVAlign)
	{
	case V_TOP:
		topleft.y = mMargins.mTop;
		bottomright.y = mSurface.getHeight() - mMargins.mBottom;
		break;
	case V_BOTTOM:
		if (!text_size.first)
			text_size = font->measure(text);
		bottomright.y = mSurface.getHeight() - mMargins.mBottom;
		if (mSurface.getHeight() - mMargins.mTop - mMargins.mBottom < text_size.second)
		{
			topleft.y = mMargins.mTop;
		}
		else
		{
			topleft.y = bottomright.y - text_size.second;
		}
		break;
	case V_CENTER:
		if (!text_size.first)
			text_size = font->measure(text);
		if (mSurface.getHeight() - mMargins.mTop - mMargins.mBottom < text_size.second)
		{
			topleft.y = mMargins.mTop;
			bottomright.y = mSurface.getHeight() - mMargins.mBottom;
		}
		else
		{
			topleft.y = ((mSurface.getHeight() - mMargins.mTop - mMargins.mBottom) - text_size.second) / 2 + mMargins.mTop;
			bottomright.y = topleft.y + text_size.second;
		}
	}
	ViewPort vp(mSurface, topleft, bottomright);
	font->setForeground(mColor);
	font->print(vp, Point(0, 0), text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

MultilineTextOutput::MultilineTextOutput(ISurface& surface) : EasyTextOutput(surface)
{
	mInterlinePadding = 0;
}

MultilineTextOutput::~MultilineTextOutput()
{

}

void MultilineTextOutput::print(const std::list<int>& _text)
{
	std::list<int>& text = (std::list<int>&) _text;
	std::list<int> string;
	VerticalAlignment valign = getVAlignment();

	size_t line_count = 0;
	Font* font = System::getSystemProperties().getFontSet().find(mFont);
	if (!font)
		return;
	size_t height = font->getHeight();
	Margins save_margins = mMargins;
	size_t vshift = 0;

	if (valign != V_TOP)
	{
		size_t vsize = measureVertical(text);
		size_t vspace = mSurface.getHeight() - mMargins.mTop - mMargins.mBottom;
		if (vsize > vspace)
			vsize = vspace;
		if (valign == V_BOTTOM)
		{
			vshift = vspace - vsize;
		}
		else
		{
			vshift = (vspace - vsize) / 2;
		}
	}

	EasyTextOutput::setVAlignment(V_TOP);
	for (std::list<int>::iterator i = text.begin(); i != text.end(); i++)
	{
		if (*i != '\n')
			string.push_back(*i);
		else
		{
			size_t vpos = line_count * (height + mInterlinePadding);
			mMargins.mTop = vshift + save_margins.mTop + vpos;
			EasyTextOutput::print(string);
			string.clear();
			line_count += 1;
		}
	}
	if (!string.empty())
	{
		mMargins.mTop = vshift + save_margins.mTop + line_count * (height + mInterlinePadding);;
		EasyTextOutput::print(string);
	}
	mMargins = save_margins;
	setVAlignment(valign);
}

std::pair<size_t, size_t> MultilineTextOutput::measure(const std::list<int>& _text)
{
	size_t xmax = 0; size_t ymax = 0;

	std::list<int>& text = (std::list<int>&) _text;
	std::list<int> string;

	Font* font = System::getSystemProperties().getFontSet().find(mFont);
	if (!font)
		return std::pair<size_t, size_t>(0, 0);
	size_t height = font->getHeight();

	for (std::list<int>::iterator i = text.begin(); i != text.end(); i++)
	{
		if (*i != '\n')
			string.push_back(*i);
		else
		{
			xmax = ____max(xmax, font->measure(string).first);
			ymax += height + mInterlinePadding;
			string.clear();
		}
	}
	if (!string.empty())
	{
		xmax = ____max(xmax, font->measure(string).first);
		ymax += height + mInterlinePadding;
	}
	return std::pair<size_t, size_t>(xmax, ymax);
}

size_t MultilineTextOutput::measureVertical(const std::list<int>& _text)
{
	size_t result = 0;

	std::list<int>& text = (std::list<int>&) _text;

	Font* font = System::getSystemProperties().getFontSet().find(mFont);
	if (!font)
		return 0;
	size_t height = font->getHeight();
	bool string = false;

	for (std::list<int>::iterator i = text.begin(); i != text.end(); i++)
	{
		if (*i != '\n')
			string = true;
		else
		{
			string = false;
			result += height + mInterlinePadding;
		}
	}
	if (string)
	{
		result += height + mInterlinePadding;
	}
	return result;
}
