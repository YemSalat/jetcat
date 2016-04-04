#include "buttons.h"
#include "inputs.h"
#include "../Surface/bmp_surface.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

bool IButton::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_PRESS)
	{
		onPress();
		event.setProcessed();
		return true;
	}
	if (event.getType() == EVENT_TYPE_DEPRESS)
	{
		onDepress();
		event.setProcessed();
		return true;
	}
	if (event.getType() == EVENT_TYPE_TIMER)
		onTimer();
	return IElement::onEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

Margins BitmapButton::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	std::string filename;
	if (mPassive)
		filename = mImagePassive;
	else
		if (mPressed)
			filename = mImagePressed;
		else
			filename = mImage;
	if (mLoadedBitmapName != filename)
		mBitmap.read(mLoadedBitmapName = filename);
	surface.apply(mBitmap);
	Point rightbottom = getTopLeft();
	rightbottom.x += mBitmap.getWidth();
	rightbottom.y += mBitmap.getHeight();
	setRightBottom(rightbottom);
	return IElement::draw(surface);
}

void BitmapButton::onPress()
{
	if (mPassive)
		return;
	if (!mPressed)
	{
		mTimePressed = TimerEvent::getGlobalCounter();
		mPressed = true;
		raise(*new DrawRequestEvent(this));
	}
}

void BitmapButton::onDepress()
{
	if (mPassive)
		return;
	if (mPressed)
	{
		mPressed = false;
		raise(*new DrawRequestEvent(this));
	}
}

void BitmapButton::onTimer()
{
	if (mPassive || !mPressed)
		return;
	if (TimerEvent::getGlobalCounter() - mTimePressed > mDepressTimeout)
		onDepress();
}

//////////////////////////////////////////////////////////////////////////////

void SwitchButton::onPress()
{
	toggle();
}

void SwitchButton::onDepress()
{
}

void SwitchButton::onTimer()
{
}

void SwitchButton::redraw()
{
	raise(*new DrawRequestEvent(this));
}

Margins SwitchButton::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	std::string filename;
	if (mPassive)
		filename = mImagePassive;
	else
	if (mOff)
		filename = mImageOff;
	else
		filename = mImageOn;
	if (mLoadedBitmapName != filename)
	mBitmap.read(mLoadedBitmapName = filename);
	surface.apply(mBitmap);
	Point rightbottom = getTopLeft();
	rightbottom.x += mBitmap.getWidth();
	rightbottom.y += mBitmap.getHeight();
	setRightBottom(rightbottom);
	return IElement::draw(surface);
}

////////////////////////////////////////////////////////////////////////////

TextButton::TextButton(
	const std::string& text,
	Componentality::Graphics::Color text_color,
	Componentality::Graphics::Color back_color,
	Componentality::Graphics::Color line_color) : mTextColor(text_color), mLineColor(line_color), mBackgroundColor(back_color)
{
	setEventFilter(&mEventsFilter);
	mPressed = false;
	mTimePressed = 0;
	mDepressTimeout = 10;
	mBackground = new Background;
	mShadow = new Shadow;
	mLabel = new Label;
	mLabelPlacement = new Placement;
	mLine = new Frame;
	add("shadow", *mShadow);
	add("label", *mLabelPlacement);
	mLabelPlacement->add("background", *mBackground);
	mLabelPlacement->add("frame", *mLine);
	mLabelPlacement->add("label", *mLabel);
	mBackground->setColor(mBackgroundColor);
	mLabel->setColor(mTextColor);
	mLine->setColor(mLineColor);
	mLabel->setText(text);
	mLabel->setAlignment(std::pair<ITextOutput::HorizontalAlignment, ITextOutput::VerticalAlignment>(ITextOutput::H_CENTER, ITextOutput::V_CENTER));
	mLabel->setFont(System::getSystemProperties().getFont(System::SYSFONT_BUTTON));
};

Margins TextButton::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	if (mStorage)
		restore(surface);
	else
		backup(surface);
	if (mPressed)
	{
		mShadow->hide();
		mLabelPlacement->move(mShadow->getWidth(), mShadow->getWidth());
	}
	else
	{
		mShadow->show();
		mLabelPlacement->move(0, 0);
	}
	return IElement::draw(surface);
}

void TextButton::onPress()
{
	if (!mPressed)
	{
		mTimePressed = TimerEvent::getGlobalCounter();
		mPressed = true;
		raise(*new DrawRequestEvent(this));
	}
}

void TextButton::onDepress()
{
	if (mPressed)
	{
		mPressed = false;
		raise(*new DrawRequestEvent(this));
	}
}

void TextButton::onTimer()
{
	if (!mPressed)
		return;
	if (TimerEvent::getGlobalCounter() - mTimePressed > mDepressTimeout)
		onDepress();
}

