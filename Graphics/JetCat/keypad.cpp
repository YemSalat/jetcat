#include "keypad.h"
#include "placement.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

const std::string Componentality::Graphics::JetCat::EVENT_TYPE_KEY_MODE = ".evt.kmode";
const size_t Componentality::Graphics::JetCat::KEY_MODE_REGULAR	= 0x0000;
const size_t Componentality::Graphics::JetCat::KEY_MODE_SHIFT	= 0x0001;
const size_t Componentality::Graphics::JetCat::KEY_MODE_ALT		= 0x0002;
const size_t Componentality::Graphics::JetCat::KEY_MODE_CTRL	= 0x0004;
const size_t Componentality::Graphics::JetCat::KEY_MODE_FN		= 0x0008;
const size_t Componentality::Graphics::JetCat::KEY_MODE_NUMERIC	= 0x0010;

KeyButton::KeyButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const std::list<int>& text)
	: BitmapButton(image, image_pressed, image_passive)
	, mLabelColor(label_color)
	, mLabelColorPressed(label_pressed)
	, mLabelColorPassive(label_passive)
{
	mLabel = new Label();
	mLabel->setText(text);
	mType = KeyEvent::ALPHABETH;
	init();
}

KeyButton::KeyButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const std::string& text)
	: BitmapButton(image, image_pressed, image_passive)
	, mLabelColor(label_color)
	, mLabelColorPressed(label_pressed)
	, mLabelColorPassive(label_passive)
{
	mLabel = new Label();
	mLabel->setText(text);
	mType = KeyEvent::ALPHABETH;
	init();
}

KeyButton::KeyButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const int _text)
	: BitmapButton(image, image_pressed, image_passive)
	, mLabelColor(label_color)
	, mLabelColorPressed(label_pressed)
	, mLabelColorPassive(label_passive)
{
	std::list<int> text;
	text.push_back(_text);
	mLabel = new Label();
	mLabel->setText(text);
	mType = KeyEvent::ALPHABETH;
	init();
}

KeyButton::KeyButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const KeyEvent::KEY_TYPE type,
	const int value,
	const std::string _text)
	: BitmapButton(image, image_pressed, image_passive)
	, mLabelColor(label_color)
	, mLabelColorPressed(label_pressed)
	, mLabelColorPassive(label_passive)
{
	mLabel = new Label();
	mLabel->setText(_text);
	mType = type;
	mValue = value;
	init();
}

void KeyButton::onPress()
{
	mLabel->setColor(mLabelColorPressed);
	BitmapButton::onPress();
	std::list<int> text = mLabel->getText();
	if (mType == KeyEvent::SPECIAL)
		raise(*new KeyEvent(this, mType, KeyEvent::DOWN, mValue));
	else if (!text.empty())
		raise(*new KeyEvent(this, mType, KeyEvent::DOWN, text.front()));
}

void KeyButton::onDepress()
{
	mLabel->setColor(mLabelColor);
	BitmapButton::onDepress();
	std::list<int> text = mLabel->getText();
	if (mType == KeyEvent::SPECIAL)
		raise(*new KeyEvent(this, mType, KeyEvent::UP, mValue));
	else if (!text.empty())
		raise(*new KeyEvent(this, mType, KeyEvent::UP, text.front()));
}

void KeyButton::init()
{
	mLabel->setMargins(Margins());
	mLabel->setColor(mLabelColor);
	mLabel->setFont(System::getSystemProperties().getFont(System::SYSFONT_KEYPAD));
	mLabel->setAlignment(std::pair<ITextOutput::HorizontalAlignment, ITextOutput::VerticalAlignment>(ITextOutput::H_CENTER, ITextOutput::V_CENTER));
	BitmapSurface bitmap(1, 1);
	if (bitmap.read(mImage))
	{
		Placement* placement = new Placement();
		placement->size(bitmap.getWidth(), bitmap.getHeight());
		placement->add("label", *mLabel);
		this->add("placement", *placement);
	}
}

Margins KeyButton::draw(ISurface& surface)
{
	return BitmapButton::draw(surface);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ModifiableKeyButton::ModifiableKeyButton(const std::string image,
	const std::string image_pressed,
	const std::string image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	std::list<ModifiableKeyButton::Mode> modes) : KeyButton(image, image_pressed, image_passive,
		label_color, label_pressed, label_passive)
{
	this->mCurrentMode = 0;
	for (std::list<ModifiableKeyButton::Mode>::iterator i = modes.begin(); i != modes.end(); i++)
		mButtonModes[i->mMode] = *i;
}

ModifiableKeyButton::~ModifiableKeyButton()
{
}

size_t ModifiableKeyButton::getMode() const
{
	CST::Common::scoped_lock lock(mLock);
	return mCurrentMode;
}

void ModifiableKeyButton::setMode(const size_t mode)
{
	CST::Common::scoped_lock lock(mLock);
	mCurrentMode = mode;
}

void ModifiableKeyButton::addMode(const Mode mode)
{
	CST::Common::scoped_lock lock(mLock);
	mButtonModes[mode.mMode] = mode;
}

void ModifiableKeyButton::removeMode(const size_t mode)
{
	CST::Common::scoped_lock lock(mLock);
	std::map<size_t, Mode>::iterator ptr = mButtonModes.find(mode);
	if (ptr != mButtonModes.end())
		mButtonModes.erase(ptr);
}

ModifiableKeyButton::Mode ModifiableKeyButton::getMode(const size_t mode)
{
	CST::Common::scoped_lock lock(mLock);
	std::map<size_t, Mode>::iterator ptr = mButtonModes.find(mode);
	if (ptr != mButtonModes.end())
		return ptr->second;
	return Mode();
}

bool ModifiableKeyButton::onEvent(CST::Common::Event& event)
{
	CST::Common::scoped_lock lock(mLock);
	if (event.getType() != EVENT_TYPE_KEY_MODE)
		return KeyButton::onEvent(event);
	KeyModeEvent& _event = (KeyModeEvent&)event;
	switch (_event.getOperation())
	{
	case KeyModeEvent::SET_MODE:
		setMode(getMode() | _event.getMode());
		break;
	case KeyModeEvent::CHANGE_MODE:
		setMode(_event.getMode());
		break;
	case KeyModeEvent::CLEAR_MODE:
		setMode(getMode() & ~_event.getMode());
		break;
	}
	return true;
}

Margins ModifiableKeyButton::draw(ISurface& master)
{
	CST::Common::scoped_lock lock(mLock);

	std::map<size_t, Mode>::iterator ptr = mButtonModes.find(mCurrentMode);
	if (ptr != mButtonModes.end())
	{
		Mode mode = ptr->second;
		if (mode.mText.empty())
		{
			std::list<int> text;
			text.push_back(mode.mValue);
			mLabel->setText(text);
		}
		else
			mLabel->setText(mode.mText);
	}
	return KeyButton::draw(master);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

KeypadLine::KeypadLine(const std::string& regular, const std::string& caps,
	const std::string& image,
	const Color color_regular,
	const Color color_pressed,
	const size_t gap)
	: mStep(gap)
{
	init(regular, caps, image, image, image, color_regular, color_pressed, color_regular, gap);
}

KeypadLine::KeypadLine(const std::string& regular, const std::string& caps,
	const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color color_regular,
	const Color color_pressed,
	const Color color_passive,
	const size_t step)
	: mStep(step)
{
	init(regular, caps, image, image_pressed, image_passive, color_regular, color_pressed, color_passive, step);
}

void KeypadLine::init(const std::string& regular, const std::string& caps,
	const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color color_regular,
	const Color color_pressed,
	const Color color_passive,
	const size_t step)
{
	mFont = System::getSystemProperties().getFont(System::SYSFONT_KEYPAD);
	for (size_t i = 0; i < regular.size(); i++)
	{
		ModifiableKeyButton::Mode mode_normal(KEY_MODE_REGULAR, (char)regular[i]);
		ModifiableKeyButton::Mode mode_caps(KEY_MODE_SHIFT, (char)caps[i]);
		ModifiableKeyButton& key =
			*new ModifiableKeyButton(image, image, image, color_regular, color_pressed, color_regular, std::list<ModifiableKeyButton::Mode>());
		key.addMode(mode_normal);
		key.addMode(mode_caps);
		Placement* placement = new Placement;
		placement->add("button", key);
		placement->move(i * step, 0);
		this->add(Tag() + (char)caps[i], *placement);
		mButtons.push_back(&key);
	}
}

KeypadLine::~KeypadLine()
{
}

void KeypadLine::setFont(const FontSet::FontDescriptor font)
{
	mFont = font;
	for (std::list<ModifiableKeyButton*>::iterator i = mButtons.begin(); i != mButtons.end(); i++)
	{
		(*i)->setFont(font);
	}
}

FontSet::FontDescriptor KeypadLine::getFont() const
{
	return mFont;
}

ModifiableKeyButton* KeypadLine::findKey(const size_t _mode, const KeyEvent::KEY_TYPE _type, const int _value)
{
	for (std::list<ModifiableKeyButton*>::iterator i = mButtons.begin(); i != mButtons.end(); i++)
	{
		ModifiableKeyButton::Mode mode = (*i)->getMode(_mode);
		if ((mode.mType == _type) && (mode.mValue == _value))
			return *i;
	}
	return NULL;
}

bool KeypadLine::onEvent(CST::Common::Event& event)
{
	if (event.getType() == EVENT_TYPE_KEY_MODE)
	{
		raise(*new DrawRequestEvent(this));
	}
	return IElement::onEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AlphabeticKeypad::AlphabeticKeypad(std::list<KeypadLine*> lines, const size_t vertical_step, const int horizontal_shift)
	: mVerticalStep(vertical_step), mHorizontalShift(horizontal_shift)
{
	init(lines, vertical_step, horizontal_shift);
}

AlphabeticKeypad::~AlphabeticKeypad()
{
}

void AlphabeticKeypad::init(std::list<KeypadLine*>& lines, const size_t vertical_step, const int horizontal_shift)
{
	CST::Common::scoped_lock lock(mLock);
	int x = 0;
	size_t y = 0;
	for (std::list<KeypadLine*>::iterator i = lines.begin(); i != lines.end(); i++)
	{
		mLines.push_back(*i);
		Placement* placement = new Placement;
		std::string key;
		KeypadLine* kpl = *i;
		key.assign((char*)&kpl, sizeof(KeypadLine*));
		placement->add("keyline", **i);
		placement->move(x, y);
		onStartLine(x, y, *kpl);
		onEndLine(x + kpl->getStep() * kpl->getNumberOfButtons(), y, *kpl);
		x += horizontal_shift;
		y += vertical_step;
		this->add(key, *placement);
	}
}

KeypadLine* AlphabeticKeypad::getLine(const size_t index)
{
	if (index >= mLines.size())
		return NULL;
	return mLines[index];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* line_1_mode_0 = "`1234567890-=";
const char* line_1_mode_1 = "~!@#$%^&*()_+";
const char* line_2_mode_0 = "qwertyuiop[]\\";
const char* line_2_mode_1 = "QWERTYUIOP{}|";
const char* line_3_mode_0 = "asdfghjkl;'";
const char* line_3_mode_1 = "ASDFGHJKL:\"";
const char* line_4_mode_0 = "zxcvbnm,./";
const char* line_4_mode_1 = "ZXCVBNM<>?";

StandardKeypad::StandardKeypad(const size_t h_step, const size_t v_step, const int h_shift,
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
	)
	: AlphabeticKeypad(std::list<KeypadLine*>(), v_step, h_shift)
{
	KeypadLine* kpl1 = new KeypadLine(
		line_1_mode_0,
		line_1_mode_1,
		image, image_pressed, image_passive,
		color_regular, color_pressed, color_passive,
		h_step
		);
	KeypadLine* kpl2 = new KeypadLine(
		line_2_mode_0,
		line_2_mode_1,
		image, image_pressed, image_passive,
		color_regular, color_pressed, color_passive,
		h_step
		);
	KeypadLine* kpl3 = new KeypadLine(
		line_3_mode_0,
		line_3_mode_1,
		image, image_pressed, image_passive,
		color_regular, color_pressed, color_passive,
		h_step
		);
	KeypadLine* kpl4 = new KeypadLine(
		line_4_mode_0,
		line_4_mode_1,
		image, image_pressed, image_passive,
		color_regular, color_pressed, color_passive,
		h_step
		);
	std::list<KeypadLine*> list;
	list.push_back(kpl1);
	list.push_back(kpl2);
	list.push_back(kpl3);
	list.push_back(kpl4);

	KeyButton* mSpace = new KeyButton(space_image, space_image_pressed, space_image_passive,
		color_regular, color_pressed, color_passive, " ");
	Placement* space_bar_placement = new Placement;
	space_bar_placement->move(4 * h_shift, 4 * v_step);
	space_bar_placement->add("space", *mSpace);
	this->add("space", *space_bar_placement);

	KeyButton* mBackspace = new KeyButton(special_image, special_image_pressed, special_image_passive,
		color_regular, color_pressed, color_passive, KeyEvent::SPECIAL, KeyEvent::VKEY_BACKSPACE, "Back");
	Placement* backspace_placement = new Placement;
	backspace_placement->move(kpl1->getNumberOfButtons() * h_step + 0 * h_shift, 0 * v_step);
	backspace_placement->add("backspace", *mBackspace);
	this->add("backspace", *backspace_placement);

	KeyButton* mDelete = new KeyButton(special_image, special_image_pressed, special_image_passive,
		color_regular, color_pressed, color_passive, KeyEvent::SPECIAL, KeyEvent::VKEY_DELETE, "Del");
	Placement* delete_placement = new Placement;
	delete_placement->move(kpl2->getNumberOfButtons() * h_step + 1 * h_shift, 1 * v_step);
	delete_placement->add("delete", *mDelete);
	this->add("delete", *delete_placement);

	KeyButton* mShift = new ModeChangeButton(special_image, special_image_pressed, special_image_passive,
		color_regular, color_pressed, color_passive, KEY_MODE_SHIFT, "Shift");
	Placement* shift_placement = new Placement;
	shift_placement->move(kpl4->getNumberOfButtons() * h_step + 3 * h_shift, 3 * v_step);
	shift_placement->add("shift", *mShift);
	this->add("shift", *shift_placement);

	init(list, v_step, h_shift);
}

StandardKeypad::~StandardKeypad()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ModeChangeButton::ModeChangeButton(const std::string image,
	const std::string image_pressed,
	const std::string image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const std::list<int>& text)
	: KeyButton(image, image_pressed, image_passive, label_color, label_pressed, label_passive, text) {}

ModeChangeButton::ModeChangeButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const std::string& text)
	: KeyButton(image, image_pressed, image_passive, label_color, label_pressed, label_passive, text) {}

ModeChangeButton::ModeChangeButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const int text)
	: KeyButton(image, image_pressed, image_passive, label_color, label_pressed, label_passive, text) {}

ModeChangeButton::ModeChangeButton(const std::string& image,
	const std::string& image_pressed,
	const std::string& image_passive,
	const Color label_color,
	const Color label_pressed,
	const Color label_passive,
	const size_t value,
	const std::string text)
	: KeyButton(image, image_pressed, image_pressed, label_color, label_pressed, label_passive, text)
{
	mValue = value;
	mActive = false;
}
	
void ModeChangeButton::onPress()
{
	CST::Common::scoped_lock lock(mLock);
	if (!mActive)
	{
		mLabel->setColor(mLabelColorPressed);
		raise(*new KeyModeEvent(this, KeyModeEvent::SET_MODE, mValue));
		mActive = true;
	}
	else
	{
		mLabel->setColor(mLabelColor);
		raise(*new KeyModeEvent(this, KeyModeEvent::CLEAR_MODE, mValue));
		mActive = false;
	}
	raise(*new DrawRequestEvent(this));
}

void ModeChangeButton::onDepress()
{
}
