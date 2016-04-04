#include "messagebox.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

MessageWindow::MessageWindow(WindowManager& owner, const bool greying) : ModalWindow(owner, greying), mButtonY((size_t)-1)
{
	mLabel = new MultilineLabel;
	mShadow = new Shadow;
	mFrame = new Frame;
	mBackground = new Background;
	mLabelPlacement = new Placement;
	mLabel->setColor(System::getSystemProperties().getColor(System::SYSCOLOR_MESSAGEBOX_TEXT));
	mBackground->setColor(System::getSystemProperties().getColor(System::SYSCOLOR_WINDOW_BACKGROUND));
	add("shadow", *mShadow);
	add("frame", *mFrame);
	add("background", *mBackground);
	add("label", *mLabelPlacement);
	mLabelPlacement->add("label", *mLabel);
	mLabel->setAlignment(std::pair<ITextOutput::HorizontalAlignment, ITextOutput::VerticalAlignment>(ITextOutput::H_CENTER, ITextOutput::V_CENTER));
	mLabel->setFont(System::getSystemProperties().getFont(System::SYSFONT_MESSAGE_BOX));
	size(System::getSystemProperties().getSize(System::SYSSIZE_MESSAGEBOX_WIDTH), System::getSystemProperties().getSize(System::SYSSIZE_MESSAGEBOX_HEIGHT));
}

MessageWindow::~MessageWindow()
{
}

Margins MessageWindow::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	move((surface.getWidth() - this->getSize().first) / 2, (surface.getHeight() - this->getSize().second) / 2);
	size_t width = this->getSize().first - mShadow->getWidth() - mFrame->getWidth() * 2;
	size_t buttons_width = 0;
	size_t buttons_height = 0;
	for (std::map<IDrawable*, Placement*>::iterator i = mPlacements.begin(); i != mPlacements.end(); i++)
	{
		buttons_width += i->second->getSize().first;
		buttons_height = ____max(i->second->getSize().second, buttons_height);
	}
	size_t gap = (width - buttons_width) / (mPlacements.size() + 1);
	size_t client_height = this->getSize().second - mShadow->getWidth() - mFrame->getWidth() * 2;
	size_t button_y = client_height - buttons_height * 2;
	if (mButtonY == (size_t)-1)
		mButtonY = button_y;
	else
		button_y = mButtonY;
	size_t button_x = gap;
	DrawOrder order = getOrder();
	for (DrawOrder::iterator i = order.begin(); i != order.end(); i++)
	{
		IDrawable* object = get(*i);
		if (mIndex.find(object) != mIndex.end())
		{
			mIndex[object]->move(button_x, button_y);
			button_x += mIndex[object]->getSize().first;
			button_x += gap;
		}
	}
	mLabelPlacement->move(0, 0);
	mLabelPlacement->size(-1, -1);
	return ModalWindow::draw(surface);
}

void MessageWindow::addButton(const Tag& button_name, IElement& button, size_t width, size_t height)
{
	if (!width)
		width = System::getSystemProperties().getSize(System::SYSSIZE_BUTTON_WIDTH);
	if (!height)
		height = System::getSystemProperties().getSize(System::SYSSIZE_BUTTON_HEIGHT);
	Placement* placement = new Placement;
	placement->size(width, height);
	placement->add("button", button);
	add(button_name, *placement);
	mPlacements[&button] = placement;
	mIndex[placement] = placement;
}

void MessageWindow::removeButton(const Tag& button_name)
{
	IDrawable* item = get(button_name);
	if (mPlacements.find(item) != mPlacements.end())
	{
		if (mIndex.find(mPlacements[item]) != mIndex.end())
			mIndex.erase(mIndex[item]);
		mPlacements.erase(mPlacements[item]);
	}
	remove(button_name);
}

