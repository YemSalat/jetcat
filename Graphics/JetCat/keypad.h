#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "drawable.h"
#include "inputs.h"
#include "buttons.h"
#include "label.h"
#include "../Fonts/FontSet.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class KeyButton : public BitmapButton
			{
			protected:
				KeyEvent::KEY_TYPE mType;
				int mValue;
				Label* mLabel;
				Color  mLabelColor;
				Color  mLabelColorPressed;
				Color  mLabelColorPassive;
			public:
				KeyButton(const std::string& image,
						const std::string& image_pressed,
						const std::string& image_passive,
						const Color label_color,
						const Color label_pressed,
						const Color label_passive,
						const std::list<int>& text);
				KeyButton(const std::string& image,
						const std::string& image_pressed,
						const std::string& image_passive,
						const Color label_color,
						const Color label_pressed,
						const Color label_passive,
						const std::string& text = "");
				KeyButton(const std::string& image,
						const std::string& image_pressed,
						const std::string& image_passive,
						const Color label_color,
						const Color label_pressed,
						const Color label_passive,
						const int text);
				KeyButton(const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const KeyEvent::KEY_TYPE,
					const int value,
					const std::string text);
				virtual ~KeyButton() {}
				virtual void setFont(const FontSet::FontDescriptor font) { mLabel->setFont(font); }
				virtual FontSet::FontDescriptor getFont() const { return mLabel->getFont(); }
			protected:
				virtual void init();
				virtual void onPress();
				virtual void onDepress();
				virtual Margins draw(ISurface&);
			};

			extern const std::string EVENT_TYPE_KEY_MODE;
			extern const size_t KEY_MODE_REGULAR;
			extern const size_t KEY_MODE_SHIFT;
			extern const size_t KEY_MODE_ALT;
			extern const size_t KEY_MODE_CTRL;
			extern const size_t KEY_MODE_FN;
			extern const size_t KEY_MODE_NUMERIC;

			class KeyModeEvent : public IInputEvent
			{
			public:
				enum Operation
				{
					SET_MODE,						// Set new mode
					CLEAR_MODE,						// Reset mode
					CHANGE_MODE						// Change entire mode
				};
			protected:
				struct Payload : public CST::Common::Event::Payload
				{
					IElement* mOriginator;
					Operation mOp;
					size_t mMode;
					Payload(IElement* sender, const Operation op, const size_t mode) :
						mOriginator(sender), mOp(op), mMode(mode) {}
				};
			public:
				KeyModeEvent(IElement* sender, const Operation op, const size_t mode) : IInputEvent(EVENT_TYPE_KEY_MODE)
				{
					setPayload(*new Payload(sender, op, mode));
				}
				virtual ~KeyModeEvent() { if (mPayload) delete mPayload; mPayload = NULL; };
				virtual IElement* getOriginator() const { if (mPayload) return ((Payload*)mPayload)->mOriginator; else return NULL; }
				virtual Operation getOperation() const { return ((Payload*)mPayload)->mOp; }
				virtual size_t getMode() const { return ((Payload*)mPayload)->mMode; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) { return true; };
			};

			class ModifiableKeyButton : public KeyButton
			{
			public:
				struct Mode
				{
					size_t mMode;
					KeyEvent::KEY_TYPE mType;
					int mValue;
					std::string mText;
				public:
					Mode()
					{
						mMode = 0;
						mValue = 0;
						mType = KeyEvent::SPECIAL;
					}
					Mode(const size_t mode, const int value, const KeyEvent::KEY_TYPE type = KeyEvent::ALPHABETH)
					{
						mMode = mode;
						mValue = value;
						mType = type;
					}
					Mode(const size_t mode, const int value, const std::string text, const KeyEvent::KEY_TYPE type = KeyEvent::SPECIAL)
					{
						mMode = mode;
						mValue = value;
						mType = type;
						mText = text;
					}
				};
			protected:
				mutable CST::Common::mutex mLock;
				std::map<size_t, Mode> mButtonModes;
				size_t mCurrentMode;
			public:
				ModifiableKeyButton(const std::string image,
					const std::string image_pressed,
					const std::string image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					std::list<ModifiableKeyButton::Mode>);
				virtual ~ModifiableKeyButton();
				virtual size_t getMode() const;
				virtual void setMode(const size_t);
				virtual void addMode(const Mode);
				virtual void removeMode(const size_t);
				virtual Mode getMode(const size_t);
				virtual Margins draw(ISurface&);
			protected:
				virtual bool onEvent(CST::Common::Event&);
			protected:
				ModifiableKeyButton(const std::string image,
					const std::string image_pressed,
					const std::string image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const std::list<int>& text) : KeyButton(image, image_pressed, image_passive, label_color, label_pressed, label_passive, text) {};
				ModifiableKeyButton(const std::string image,
					const std::string image_pressed,
					const std::string image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const std::string text = "") : KeyButton(image, image_pressed, image_passive, label_color, label_pressed, label_passive, text) {};
				ModifiableKeyButton(const std::string image,
					const std::string image_pressed,
					const std::string image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const int text) : KeyButton(image, image_pressed, image_passive, label_color, label_pressed, label_passive, text) {};
			};

			class KeypadLine : public IElement
			{
			protected:
				std::list<ModifiableKeyButton*> mButtons;
				FontSet::FontDescriptor mFont;
				const size_t mStep;
			public:
				KeypadLine(const std::string& regular, const std::string& caps,
					const std::string& image,
					const Color color_regular,
					const Color color_pressed,
					const size_t step);
				KeypadLine(const std::string& regular, const std::string& caps,
					const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const Color color_regular,
					const Color color_pressed,
					const Color color_passive,
					const size_t step);
				virtual ~KeypadLine();
				virtual void setFont(const FontSet::FontDescriptor font);
				virtual FontSet::FontDescriptor getFont() const;
				virtual ModifiableKeyButton* findKey(const size_t, const KeyEvent::KEY_TYPE, const int value);
				virtual size_t getStep() const { return mStep; };
				virtual size_t getNumberOfButtons() const { return mButtons.size(); }
			protected:
				virtual void init(const std::string& regular, const std::string& caps,
					const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const Color color_regular,
					const Color color_pressed,
					const Color color_passive,
					const size_t step);
				virtual bool onEvent(CST::Common::Event& event);
			};

			class AlphabeticKeypad : public IElement
			{
			protected:
				CST::Common::mutex mLock;
				const size_t mVerticalStep;
				const int mHorizontalShift;
				std::vector<KeypadLine*> mLines;
			public:
				AlphabeticKeypad(std::list<KeypadLine*>, const size_t vertical_step, const int horizontal_shift);
				virtual ~AlphabeticKeypad();
				virtual KeypadLine* getLine(const size_t index);
				virtual size_t getVerticalStep() const { return mVerticalStep; }
				virtual int getHorizontalShift() const { return mHorizontalShift; }
			protected:
				virtual void init(std::list<KeypadLine*>& lines, const size_t vertical_step, const int horizontal_shift);
				virtual void onStartLine(int& x, size_t& y, KeypadLine& kpl) {};
				virtual void onEndLine(const int x, const size_t y, KeypadLine& kpl) {};
			};

			class ModeChangeButton : public KeyButton
			{
			protected:
				CST::Common::mutex mLock;
				size_t mValue;
				bool mActive;
			protected:
				ModeChangeButton(const std::string image,
					const std::string image_pressed,
					const std::string image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const std::list<int>& text);
				ModeChangeButton(const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const std::string& text = "");
				ModeChangeButton(const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const int text);
			public:
				ModeChangeButton(const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const Color label_color,
					const Color label_pressed,
					const Color label_passive,
					const size_t value,
					const std::string text);
				virtual ~ModeChangeButton() {}
			protected:
				virtual void onPress();
				virtual void onDepress();
			};

			class StandardKeypad : public AlphabeticKeypad
			{
			protected:
				KeyButton* mSpace;
				KeyButton* mBackspace;
				KeyButton* mShift;
				KeyButton* mDelete;
			protected:
				StandardKeypad(std::list<KeypadLine*>& line, const size_t vertical_step, const int horizontal_shift)
					: AlphabeticKeypad(line, vertical_step, horizontal_shift) {};
			public:
				StandardKeypad(const size_t h_step, const size_t v_step, const int h_shift,
					const std::string& image,
					const std::string& image_pressed,
					const std::string& image_passive,
					const std::string& space_image,
					const std::string& space_image_pressed,
					const std::string& space_image_passive,
					const std::string& special_image,
					const std::string& special_image_pressed,
					const std::string& special_image_passive,
					const Color color_regular,
					const Color color_pressed,
					const Color color_passive
					);
				virtual ~StandardKeypad();
				virtual KeyButton* getSpaceButton() const { return mSpace; }
				virtual KeyButton* getBackspaceButton() const { return mBackspace; }
				virtual KeyButton* getDeleteButton() const { return mDelete; }
				virtual KeyButton* getShiftButton() const { return mShift; }
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
