#include "editbox.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

void EditBox::init()
{
	mCursor = new Cursor;
	mCursor->setSize(System::getSystemProperties().getCursorSize(), System::getSystemProperties().getCursorSize());
	mCursor->hide();
	add(".cursor", *mCursor);
	mPosition = 0;
	mScrolling = 0;
	mSpace = 5;
	setMargins(System::getSystemProperties().getMargins(System::SYSMARGINS_EDITBOX));
	mColor = System::getSystemProperties().getColor(System::SYSCOLOR_EDITBOX_FONT);
	mCursorColor = System::getSystemProperties().getColor(System::SYSCOLOR_EDITBOX_CURSOR);
	mBackgroundColor = System::getSystemProperties().getColor(System::SYSCOLOR_EDITBOX_BACKGROUND);
	mFrameColor = System::getSystemProperties().getColor(System::SYSCOLOR_EDITBOX_FRAME);
	mFont = System::getSystemProperties().getFont(System::SYSFONT_EDITBOX);
	setEventFilter(&mEventsFilter);
}

EditBox::EditBox(IFocusController& controller) : IFocusable(controller)
{
	init();
}

EditBox::EditBox(IFocusController& controller, const std::string text) : Label(text), IFocusable(controller)
{
	init();
}

EditBox::EditBox(IFocusController& controller, const std::string text, Componentality::Graphics::Color color, Margins margins)
	: Label(text, color, margins), IFocusable(controller)
{
	init();
}

EditBox::EditBox(IFocusController& controller, const std::list<int> text) : Label(text), IFocusable(controller)
{
	init();
}

EditBox::EditBox(IFocusController& controller, const std::list<int> text, Componentality::Graphics::Color color, Margins margins)
	: Label(text, color, margins), IFocusable(controller)
{
	init();
}

EditBox::~EditBox()
{
	if (mCursor)
		delete mCursor;
}

Margins EditBox::draw(ISurface& master)
{
	if (isHidden())
		return Margins();
	CST::Common::scoped_lock lock(mLock);
	Margins margins = getMargins();
	if (!mFrameColor.isUndefined())
	{
		Drawer drawer(master);
		drawer.rectangle(Point(0, 0), Point(master.getWidth() - 1, master.getHeight() - 1), mFrameColor);
		margins += Margins(1, 1, 1, 1);
	}
	if (!mBackgroundColor.isUndefined())
	{
		Drawer drawer(master);
		drawer.filled_rectangle(Point(1, 1), Point(master.getWidth() - 2, master.getHeight() - 2), mBackgroundColor);
	}
	ViewPort basic_surface(master, margins.getTopLeft(master), margins.getRightBottom(master));
	if (mCursor)
	{
		if (mCursorColor.isUndefined())
		{
			Color color = getColor();
			Componentality::Graphics::Color cursor_color(255 - color.operator Componentality::Graphics::ColorRGB().red,
				255 - color.operator Componentality::Graphics::ColorRGB().green,
				255 - color.operator Componentality::Graphics::ColorRGB().blue);
			mCursor->setColor(cursor_color);
		}
		else
			mCursor->setColor(mCursorColor);
		std::pair<Point, std::pair<size_t, size_t> > cursor_defs = positionCursor(basic_surface);
		mCursor->setLocation(cursor_defs.first + margins.getTopLeft(master));
		mCursor->setSize(cursor_defs.second.first, cursor_defs.second.second);
		mCursor->reset();
	}
	Font* font = System::getSystemProperties().getFontSet().find(this->getFont());
	MovingPort surface(basic_surface, -(int) mScrolling, 0);
	if (font)
	{
		std::list<int> text = getText();
		font->setForeground(mColor);
		font->print(surface, Point(1, 0), text);
	}
	IElement::draw(surface);
	return margins;
}

class Measurer : public Font::PrintCharacterCallback
{
public:
	size_t mBegin;
	size_t mEnd;
	size_t mIndex;
	size_t mTotal;
public:
	Measurer(const size_t index) : mBegin((size_t)-1), mEnd((size_t)-1), mTotal(0), mIndex(index) {};
	virtual ~Measurer() {};
protected:
	void onCharacterPrinting(const size_t index, const int character, const Point& start_location)
	{
		if (index == mIndex)
			mBegin = start_location.x;
	};
	void onCharacterPrinted(const size_t index, const int character, const Point& end_location)
	{
		if (index == mIndex)
			mEnd = end_location.x;
		mTotal = end_location.x;
	};
};

std::pair<Point, std::pair<size_t, size_t> > EditBox::positionCursor(ISurface& surface)
{
	CST::Common::scoped_lock lock(mLock);
	std::pair<Point, std::pair<size_t, size_t> > result(Point(0, 0), std::pair<size_t, size_t>(0, 0));
	NullSurface null_surface((size_t)-1, (size_t)-1);
	Font* font = System::getSystemProperties().getFontSet().find(this->getFont());
	if (!font)
		return result;
	Measurer measurer(this->mPosition);
	std::list<int> string = getText();
	font->print(null_surface, Point(0, 0), string, &measurer);
	if (System::getSystemProperties().getCursorType() == System::SYSCURSOR_HORIZONTAL)
	{
		result.first.x = mPosition < string.size() ? measurer.mBegin : measurer.mTotal;
		result.second.first = (measurer.mBegin != (size_t)-1) && (measurer.mEnd != (size_t)-1) ? measurer.mEnd - measurer.mBegin : font->getHeight() / 2;
		result.first.y = font->getBaseline() + System::getSystemProperties().getCursorIndent();
		result.second.second = mCursor->getSize().second;
	}
	else if (System::getSystemProperties().getCursorType() == System::SYSCURSOR_VERTICAL)
	{
		result.first.x = mPosition < string.size() ? measurer.mBegin : measurer.mTotal;
		result.second.first = mCursor->getSize().first;
		result.first.y = 0;
		result.second.second = font->getHeight() - result.first.y;
	}
	while (result.first.x + mSpace >= mScrolling + (int) surface.getWidth())
	{
		mScrolling += mSpace;
	}
	while (result.first.x < mScrolling)
	{
		mScrolling -= ____min(mSpace, mScrolling);
	}
	result.first.x -= mScrolling;
	return result;
}

void EditBox::setPosition(const size_t pos)
{
	CST::Common::scoped_lock lock(mLock);
	mPosition = ____min(pos, getText().size());
}

bool EditBox::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	CST::Common::scoped_lock lock(mLock);
	if (event.getType() == EVENT_TYPE_PRESS)
	{
		requestFocus();
	}
	else if (event.getType() == EVENT_TYPE_KEY)
	{
		KeyEvent& kevent = (KeyEvent&)event;
		bool processed = false;
		if ((kevent.getKeyType() == KeyEvent::SPECIAL) && (kevent.getEventType() == KeyEvent::UP))
		{
			switch (kevent.getKeyCode())
			{
			case KeyEvent::VKEY_LEFT:
				if (mPosition)
					mPosition -= 1;
				break;
			case KeyEvent::VKEY_RIGHT:
				if (mPosition <= getText().size())
					mPosition += 1;
				break;
			case KeyEvent::VKEY_DELETE:
				remove();
				break;
			case KeyEvent::VKEY_BACKSPACE:
				if (mPosition)
				{
					mPosition -= 1;
					remove();
				}
				break;
			}
			raise(*new DrawRequestEvent(this));
			event.setProcessed();
		}
		if ((kevent.getKeyType() == KeyEvent::ALPHABETH) && (kevent.getEventType() == KeyEvent::DOWN) && isAlphabetic(kevent.getKeyCode()))
		{
			insert(kevent.getKeyCode());
			raise(*new DrawRequestEvent(this));
			event.setProcessed();
		}
	}
	return IElement::onEvent(event);
}

static std::string __alphabeth = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-={}[]:';\",./?\\|~` ";

bool EditBox::isAlphabetic(const int keycode)
{
	return __alphabeth.find((char)keycode) != (size_t)-1;
}

void EditBox::insert(const int key)
{
	CST::Common::scoped_lock lock(mLock);
	std::list<int> text = getText();
	if (mPosition >= text.size())
	{
		text.push_back(key);
		mPosition++;
		setText(text);
		return;
	}
	std::list<int> result;
	size_t counter = 0;
	for (std::list<int>::iterator i = text.begin(); i != text.end(); i++, counter++)
	{
		if (mPosition == counter)
			result.push_back(key);
		result.push_back(*i);
	}
	mPosition++;
	setText(result);
}

void EditBox::remove()
{
	CST::Common::scoped_lock lock(mLock);
	std::list<int> text = getText();
	if (mPosition < text.size())
	{
		std::list<int> result;
		size_t counter = 0;
		for (std::list<int>::iterator i = text.begin(); i != text.end(); i++, counter++)
		{
			if (mPosition != counter)
				result.push_back(*i);
		}
		setText(result);
	}
}

bool EditBox::onSetFocus() 
{ 
	CST::Common::scoped_lock lock(mLock);
	mCursor->show();
	raise(*new DrawRequestEvent(this));
	return true;
};

bool EditBox::onReleaseFocus() 
{
	CST::Common::scoped_lock lock(mLock);
	mCursor->hide();
	raise(*new DrawRequestEvent(this));
	return true;
}
