#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "drawable.h"
#include "background.h"
#include "shadow.h"
#include "label.h"
#include "placement.h"
#include "frame.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class IButton : public IElement
			{
			public:
				IButton() {}
				virtual ~IButton() {};
				virtual bool onEvent(CST::Common::Event&);
			protected:
				virtual void onPress() = 0;
				virtual void onDepress() = 0;
				virtual void onTimer() = 0;
			};

			class ActiveArea : public IButton
			{
			public:
				ActiveArea() {}
				virtual ~ActiveArea() {};
			protected:
				virtual void onPress() {};
				virtual void onDepress() {};
			};

			class BitmapButton : public IButton
			{
			protected:
				std::string mImage;
				std::string mImagePressed;
				std::string mImagePassive;
				bool mPressed;
				bool mPassive;
				StdEventFilter mEventsFilter;
				size_t mTimePressed;
				size_t mDepressTimeout;
				std::string mLoadedBitmapName;			// Name of currently loaded bitmap
				BitmapSurface mBitmap;					// Currently loaded bitmap
			public:
				BitmapButton(
							const std::string image,
							const std::string pressed,
							const std::string passive) : mImage(image), mImagePressed(pressed), mImagePassive(passive), mBitmap(0, 0)
				{
					setEventFilter(&mEventsFilter);
					mPressed = false;
					mPassive = false;
					mDepressTimeout = 10;
				};
				virtual ~BitmapButton() {};
				virtual Margins draw(ISurface&);
				virtual void activate() { mPassive = false; };
				virtual void passivate() { mPassive = true; };
				virtual void press() { onPress(); };
				virtual void depress() { onDepress(); };
				void setDepressTimeout(const size_t timeout) { mDepressTimeout = timeout; }
				size_t getDepressTimeout() const { return mDepressTimeout; };
			protected:
				virtual void onPress();
				virtual void onDepress();
				virtual void onTimer();
			};

			class SwitchButton : public IButton
			{
			protected:
				std::string mImageOn;
				std::string mImageOff;
				std::string mImagePassive;
				bool mOff;
				bool mPassive;
				StdEventFilter mEventsFilter;
				std::string mLoadedBitmapName;
				BitmapSurface mBitmap;
			public:
				SwitchButton(
					const std::string imageOn,
					const std::string imageOff,
					const std::string imagePassive) : mImageOn(imageOn), mImageOff(imageOff), mImagePassive(imagePassive), mBitmap(0, 0)
				{
					setEventFilter(&mEventsFilter);
					mOff = false;
					mPassive = false;
				};
				virtual ~SwitchButton() {};
				virtual Margins draw(ISurface&);
				virtual void set(const bool off) { mOff = off; redraw(); };
				virtual void toggle() { mOff = !mOff; redraw(); };
				virtual void press() { onPress(); };
				virtual void depress() { onDepress(); };
				virtual void setPassive(const bool passive) { mPassive = passive; }
				virtual bool getPassive() const { return mPassive; }
			protected:
				virtual void onPress();
				virtual void onDepress();
				virtual void onTimer();
				virtual void redraw();
			};

			class TextButton : public IButton, public Restorable
			{
			protected:
				Componentality::Graphics::Color mTextColor;
				Componentality::Graphics::Color mLineColor;
				Componentality::Graphics::Color mBackgroundColor;
				Placement*						mLabelPlacement;
				Label*							mLabel;
				Shadow*							mShadow;
				Background*						mBackground;
				Frame*							mLine;
				bool mPressed;
				StdEventFilter mEventsFilter;
				size_t mDepressTimeout;
				size_t mTimePressed;
			public:
				TextButton(
					const std::string& text,
					Componentality::Graphics::Color text_color,
					Componentality::Graphics::Color back_color,
					Componentality::Graphics::Color line_color);
				virtual ~TextButton() {};
				virtual Margins draw(ISurface&);
				virtual void press() { onPress(); };
				virtual void depress() { onDepress(); };
				virtual void setText(const std::string& text) { mLabel->setText(text); }
				virtual void setText(const std::list<int>& text) { mLabel->setText(text); }
				virtual std::list<int> getText() const { return mLabel->getText(); }
				virtual FontSet::FontDescriptor getFont() const { return mLabel->getFont(); }
				virtual void setFont(const FontSet::FontDescriptor font) { mLabel->setFont(font); }
				void setDepressTimeout(const size_t timeout) { mDepressTimeout = timeout; }
				size_t getDepressTimeout() const { return mDepressTimeout; };
			protected:
				virtual void onPress();
				virtual void onDepress();
				virtual void onTimer();
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif