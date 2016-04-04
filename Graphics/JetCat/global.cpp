#include "global.h"

#ifdef WIN32
#define FONTS_FOLDER "c:\\Componentality\\Fonts"
#else
#define FONTS_FOLDER "/var/componentality/fonts"
#endif


using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

IColorDefinitions::IColorDefinitions()
{
	mColors[SYSCOLOR_WINDOW_BACKGROUND] = Color(0, 0, 0);
	mColors[SYSCOLOR_WINDOW_CAPTION_LINE] = Color(80, 80, 80);
	mColors[SYSCOLOR_WINDOW_CAPTION_BACKGROUND] = Color(60, 60, 60);
	mColors[SYSCOLOR_WINDOW_CAPTION_TEXT] = Color(255, 255, 255);
	mColors[SYSCOLOR_WINDOW_FRAME] = Color(80, 80, 80);
	mColors[SYSCOLOR_WINDOW_SHADOW] = Color(0, 0, 0);
	mColors[SYSCOLOR_WINDOW_STATUS_BACKGROUND] = Color(90, 90, 90);
	mColors[SYSCOLOR_WINDOW_STATUS_LINE] = mColors[SYSCOLOR_WINDOW_CAPTION_LINE];
	mColors[SYSCOLOR_WINDOW_STATUS_TEXT] = Color(0, 0, 0);
	mColors[SYSCOLOR_EDITBOX_FONT] = Color(0, 0, 0);
	mColors[SYSCOLOR_EDITBOX_CURSOR] = Color(255, 255, 255);
	mColors[SYSCOLOR_EDITBOX_BACKGROUND] = Color(80, 80, 80);
	mColors[SYSCOLOR_EDITBOX_FRAME] = Color(255, 255, 255);
	mColors[SYSCOLOR_MESSAGEBOX_TEXT] = Color(255, 255, 255);
}

Componentality::Graphics::Color IColorDefinitions::getColor(const SystemColorType type)
{
	return mColors[type];
}

void IColorDefinitions::setColor(const SystemColorType type, const Componentality::Graphics::Color color)
{
	mColors[type] = color;
}

System* System::mInstance = NULL;

System& System::getSystemProperties()
{
	if (!mInstance)
		mInstance = new System();
	return *mInstance;
}

IFontDefinitions::IFontDefinitions()
{
	mFontSet = new LoadableFontSet(FONTS_FOLDER);
	((LoadableFontSet*)mFontSet)->deserialize();
	mFonts[SYSFONT_WINDOW_CAPTION] = FontSet::FontDescriptor("Arial", 16, Font::FONT_NORMAL);
	mFonts[SYSFONT_WINDOW_STATUS] = FontSet::FontDescriptor("Arial", 16, Font::FONT_NORMAL);
	mFonts[SYSFONT_KEYPAD] = FontSet::FontDescriptor("Arial", 16, Font::FONT_BOLD);
	mFonts[SYSFONT_EDITBOX] = FontSet::FontDescriptor("Arial", 16, Font::FONT_NORMAL);
	mFonts[SYSFONT_BUTTON] = FontSet::FontDescriptor("Arial", 16, Font::FONT_BOLD);
	mFonts[SYSFONT_MESSAGE_BOX] = FontSet::FontDescriptor("Arial", 32, Font::FONT_NORMAL);
}

IFontDefinitions::~IFontDefinitions()
{
	if (!mFontSet)
		delete mFontSet;
}

FontSet::FontDescriptor IFontDefinitions::getFont(const SystemFontType fontindex)
{
	return mFonts[fontindex];
}

void IFontDefinitions::setFont(const SystemFontType fontindex, FontSet::FontDescriptor font)
{
	mFonts[fontindex] = font;
}

FontSet& IFontDefinitions::getFontSet()
{
	return *mFontSet;
}

void IFontDefinitions::setFontSet(FontSet& fset)
{
	if (mFontSet)
		delete mFontSet;
	mFontSet = &fset;
}

IMarginDefinitions::IMarginDefinitions()
{
	mMargins[SYSMARGINS_CAPTION_TEXT] = Margins(5, 5, 5, 5);
	mMargins[SYSMARGINS_STATUS_TEXT] = Margins(5, 2, 5, 1);
	mMargins[SYSMARGINS_EDITBOX] = Margins(2, 2, 2, 2);
}

IMarginDefinitions::~IMarginDefinitions()
{
}

Margins IMarginDefinitions::getMargins(const SystemMarginsType margin)
{
	return mMargins[margin];
}

void IMarginDefinitions::setMargins(const SystemMarginsType margin, const Margins value)
{
	mMargins[margin] = value;
}

IAlignmentDefinitions::IAlignmentDefinitions()
{
	mAlignments[SYSALIGN_CAPTION_TEXT].first = ITextOutput::H_LEFT;
	mAlignments[SYSALIGN_CAPTION_TEXT].second = ITextOutput::V_CENTER;
	mAlignments[SYSALIGN_STATUS_TEXT].first = ITextOutput::H_LEFT;
	mAlignments[SYSALIGN_STATUS_TEXT].second = ITextOutput::V_CENTER;
}

ISizeDefinitions::ISizeDefinitions()
{
	mSizes[SYSSIZE_CAPTION_HEIGHT] = 32;
	mSizes[SYSSIZE_STATUS_HEIGHT] = 24;
	mSizes[SYSSIZE_BUTTON_WIDTH] = 100;
	mSizes[SYSSIZE_BUTTON_HEIGHT] = 30;
	mSizes[SYSSIZE_MESSAGEBOX_WIDTH] = 500;
	mSizes[SYSSIZE_MESSAGEBOX_HEIGHT] = 200;
}

int ISizeDefinitions::getSize(const SystemSizeType size)
{
	return mSizes[size];
}

void ISizeDefinitions::setSize(const SystemSizeType size, int value)
{
	mSizes[size] = value;
}

////////////////////////////////////////////////////////////////////////

ICursorDefinitions::ICursorDefinitions()
{
	mType = SYSCURSOR_VERTICAL;
	mSize = 1;
	mIndent = 3;
}

ICursorDefinitions::~ICursorDefinitions()
{
}

ICursorDefinitions::CursorType ICursorDefinitions::getCursorType()
{
	return mType;
}

void ICursorDefinitions::setCursorType(const ICursorDefinitions::CursorType type)
{
	mType = type;
}

size_t ICursorDefinitions::getCursorSize()
{
	return mSize;
}

void ICursorDefinitions::setCursorSize(const size_t size)
{
	mSize = size;
}
