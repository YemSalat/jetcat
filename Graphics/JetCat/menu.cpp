#include "menu.h"
#include "background.h"
#include "label.h"
#include "inputs.h"

using namespace Componentality::Graphics::JetCat;
using namespace Componentality::Graphics;

IMenu::IMenu()
{
	mSelectionStrategy = SINGLE;
	setEventFilter(&mEventsFilter);
}

IMenu::~IMenu()
{
}

Margins IMenu::draw(ISurface& owner)
{
	size(owner.getWidth() - mXOffset, owner.getHeight() - mYOffset);
	Margins margins = Scroller::draw(owner);
	size(owner.getWidth(), owner.getHeight());
	return margins;
}

void IMenu::add(const Tag tag, IElement& unselected, IElement& selected)
{
	CST::Common::scoped_lock lock(mLock);
	if (get(tag))
		return;
	mUnselectedItems[tag] = &unselected;
	mSelectedItems[tag] = &selected;
	IElement::add(tag, unselected);
}

void IMenu::remove(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	if (mSelectedItems.find(tag) != mSelectedItems.end())
	{
		mSelectedItems.erase(mSelectedItems.find(tag));
	}
	if (mUnselectedItems.find(tag) != mUnselectedItems.end())
	{
		mUnselectedItems.erase(mUnselectedItems.find(tag));
	}
	if (mSelected.find(tag) != mSelected.end())
	{
		mSelected.erase(mSelected.find(tag));
	}
	IElement::remove(tag);
}

void IMenu::select(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	if (mSelectionStrategy == SINGLE)
	{
		std::list<Tag> selected = listSelected();
		for (std::list<Tag> ::iterator i = selected.begin(); i != selected.end(); i++)
			unselect(*i);
	}
	if (get(tag))
		if (mSelectedItems.find(tag) != mSelectedItems.end())
		{
			IElement* item = mSelectedItems[tag];
			mSelected[tag] = item;
			IElement::replace(tag, *item);
		}
}

void IMenu::unselect(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	if (get(tag))
		if (mUnselectedItems.find(tag) != mUnselectedItems.end())
		{
			if (mSelected.find(tag) != mSelected.end())
				mSelected.erase(mSelected.find(tag));
			IElement::replace(tag, *mUnselectedItems[tag]);
		}
}

bool IMenu::isSelected(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	return mSelected.find(tag) != mSelected.end();
}

std::list<Tag> IMenu::listSelected()
{
	CST::Common::scoped_lock lock(mLock);
	std::list<Tag> result;
	for (std::map<Tag, IElement*>::iterator i = mSelected.begin(); i != mSelected.end(); i++)
		result.push_back(i->first);
	return result;
}

bool IMenu::onEvent(CST::Common::Event& event)
{
	CST::Common::scoped_lock lock(mLock);
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_MENU)
	{
		MenuEvent& menu_event = (MenuEvent&)event;
		IElement* originator = menu_event.getItem();
		Tag originators_tag = has(*originator);
		if (originators_tag.empty())
			return false;
		switch (mSelectionStrategy)
		{
		case SINGLE:
			select(originators_tag);
			break;
		case MULTIPLE:
			if (isSelected(originators_tag))
				unselect(originators_tag);
			else
				select(originators_tag);
			break;
		}
		event.setProcessed();
		raise(*new DrawRequestEvent(this));
		return true;
	}
	if (event.getType() == EVENT_TYPE_SET_DEFAULT)
	{
		for (std::map<Tag, IElement*>::iterator i = mSelectedItems.begin(); i != mSelectedItems.end(); i++)
			i->second->send(event);
		for (std::map<Tag, IElement*>::iterator i = mUnselectedItems.begin(); i != mUnselectedItems.end(); i++)
			i->second->send(event);
	}
	return IElement::onEvent(event);
}

size_t IMenu::getPosition(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	size_t result = 0;
	DrawOrder order = getOrder();
	for (DrawOrder::iterator i = order.begin(); i != order.end(); i++, result++)
		if (*i == tag)
			return result;
	return (size_t)-1;
}

Tag IMenu::getByPosition(const size_t index)
{
	CST::Common::scoped_lock lock(mLock);
	size_t idx = 0;
	DrawOrder order = getOrder();
	for (DrawOrder::iterator i = order.begin(); i != order.end(); i++, idx++)
		if (idx == index)
			return *i;
	return Tag();
}

DrawOrder IMenu::getOrder()
{
	DrawOrder source_order = IElement::getOrder();
	DrawOrder result;
	for (DrawOrder::iterator i = source_order.begin(); i != source_order.end(); i++)
		if (!isHidden(*i))
			result.push_back(*i);
	return result;
}


//////////////////////////////////////////////////////////////////////////////////////////////

void IPlaceableMenu::rearrange()
{
	DrawOrder draw_order = getOrder();
	initLocation();
	for (DrawOrder::iterator i = draw_order.begin(); i != draw_order.end(); i++)
	{
		if ((mSelectedItems.find(*i) != mSelectedItems.end()) && (mSelected.find(*i) != mSelected.end()))
		{
			Point location = getLocation(*i);
			std::pair<size_t, size_t> size = getSize(*i);
			Placement* selected_placement = mPlacements[*i].second;
			selected_placement->move(location.x, location.y);
			selected_placement->size(size.first, size.second);
		}
		else if (mUnselectedItems.find(*i) != mUnselectedItems.end())
		{
			Point location = getLocation(*i);
			std::pair<size_t, size_t> size = getSize(*i);
			Placement* unselected_placement = mPlacements[*i].first;
			unselected_placement->move(location.x, location.y);
			unselected_placement->size(size.first, size.second);
		}
	}
}

void IPlaceableMenu::add(const Tag& tag, IElement& unselected, IElement& selected)
{
	CST::Common::scoped_lock lock(mLock);
	Placement* unselected_placement = new Placement, *selected_placement = new Placement;
	unselected_placement->add(tag, unselected);
	selected_placement->add(tag, selected);
	mPlacements[tag] = std::pair<Placement*, Placement*>(unselected_placement, selected_placement);
	mSelectedIndex[tag] = &selected;
	mUnselectedIndex[tag] = &unselected;
	mReverseIndex[&selected] = tag;
	mReverseIndex[&unselected] = tag;
	IMenu::add(tag, *unselected_placement, *selected_placement);
}

void IPlaceableMenu::remove(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	IMenu::remove(tag);
	if (mPlacements.find(tag) != mPlacements.end())
	{
		std::pair<Placement*, Placement*> item = mPlacements[tag];
		delete item.first;
		delete item.second;
		mPlacements.erase(mPlacements.find(tag));
	}
	if (mSelectedIndex.find(tag) != mSelectedIndex.end())
	{
		mReverseIndex.erase(mReverseIndex.find(mSelectedIndex[tag]));
		mSelectedIndex.erase(mSelectedIndex.find(tag));
	}
	if (mUnselectedIndex.find(tag) != mUnselectedIndex.end())
	{
		mReverseIndex.erase(mReverseIndex.find(mUnselectedIndex[tag]));
		mUnselectedIndex.erase(mUnselectedIndex.find(tag));
	}
}

Tag IPlaceableMenu::has(IElement& item)
{
	CST::Common::scoped_lock lock(mLock);
	if (mReverseIndex.find(&item) != mReverseIndex.end())
		return mReverseIndex[&item];
	else
		return Tag();
}

Margins IPlaceableMenu::draw(ISurface& surface)
{
	rearrange();
	return IMenu::draw(surface);
}


///////////////////////////////////////////////////////////////////////////////

IVerticalMenu::IVerticalMenu(const size_t hsize) : mHorizontalSize(hsize)
{
	mVerticalSize = 0;
}

IVerticalMenu::~IVerticalMenu()
{
}

Point IVerticalMenu::getLocation(const Tag& tag)
{
	mVerticalLocation[tag] = mVerticalSize;
	mVerticalSize += mItemsVerticalSize[tag];
	return Point(0, mVerticalLocation[tag]);
};

void IVerticalMenu::add(const Tag& tag, IElement& unselected, IElement& selected, const size_t row_size)
{
	CST::Common::scoped_lock lock(mLock);
	mItemsVerticalSize[tag] = row_size;
	IPlaceableMenu::add(tag, unselected, selected);
}

void IVerticalMenu::remove(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	std::map<Tag, size_t>::iterator found_vl = mVerticalLocation.find(tag);
	if (found_vl != mVerticalLocation.end())
		mVerticalLocation.erase(found_vl);
	found_vl = mItemsVerticalSize.find(tag);
	if (found_vl != mItemsVerticalSize.end())
		mItemsVerticalSize.erase(found_vl);
	IPlaceableMenu::remove(tag);
}

///////////////////////////////////////////////////////////////////////////////

IHorizontalMenu::IHorizontalMenu(const size_t vsize) : mVerticalSize(vsize)
{
	mHorizontalSize = 0;
}

IHorizontalMenu::~IHorizontalMenu()
{
}

Point IHorizontalMenu::getLocation(const Tag& tag)
{
	mHorizontalLocation[tag] = mHorizontalSize;
	mHorizontalSize += mItemsHorizontalSize[tag];
	return Point(mHorizontalLocation[tag], 0);
}

void IHorizontalMenu::add(const Tag& tag, IElement& unselected, IElement& selected, const size_t col_size)
{
	CST::Common::scoped_lock lock(mLock);
	mItemsHorizontalSize[tag] = col_size;
	IPlaceableMenu::add(tag, unselected, selected);
}

void IHorizontalMenu::remove(const Tag& tag)
{
	CST::Common::scoped_lock lock(mLock);
	std::map<Tag, size_t>::iterator found_vl = mHorizontalLocation.find(tag);
	if (found_vl != mHorizontalLocation.end())
		mHorizontalLocation.erase(found_vl);
	found_vl = mItemsHorizontalSize.find(tag);
	if (found_vl != mItemsHorizontalSize.end())
		mItemsHorizontalSize.erase(found_vl);
	IPlaceableMenu::remove(tag);
}

////////////////////////////////////////////////////////////////////////////////////////////

TextMenuItem::TextMenuItem(IMenu& menu, const Color foreground, const Color background, const std::string text)
	: IMenuItem(menu)
{
	mGround = new Background;
	((Background*)mGround)->setColor(background);
	mText = new EasyLabel(text, foreground);
	this->add("background", *mGround);
	this->add("text", *mText);
	std::string id;
	IElement* self = this;
	id.assign((char*)&self, sizeof(IElement*));
}

TextMenuItem::TextMenuItem(IMenu& menu, const Color foreground, const Color background, const std::list<int> text)
	: IMenuItem(menu)
{
	mGround = new Background;
	((Background*)mGround)->setColor(background);
	mText = new EasyLabel(text, foreground);
	this->add("background", *mGround);
	this->add("text", *mText);
	std::string id;
	IElement* self = this;
	id.assign((char*)&self, sizeof(IElement*));
}

TextMenuItem::~TextMenuItem()
{
	this->remove("background");
	this->remove("text");
	delete mGround;
	delete mText;
}

std::pair<Color, Color> TextMenuItem::getColor() const
{
	Background* background = (Background*)mGround;
	Label* text = (Label*)mText;
	return std::pair<Color, Color>(text->getColor(), background->getColor());
}

std::list<int> TextMenuItem::getText() const
{
	Label* text = (Label*)mText;
	return text->getText();
}

bool TextMenuItem::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_PRESS)
	{
		raise(*new MenuEvent(this));
		event.setProcessed();
		return true;
	}
	return IElement::onEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

VerticalMenu::VerticalMenu(IFocusController& fc, const size_t hsize) : IVerticalMenu(hsize), IFocusable(fc)
{
}

VerticalMenu::~VerticalMenu()
{
}

Margins VerticalMenu::draw(ISurface& surface)
{
	return IVerticalMenu::draw(surface);
}

bool VerticalMenu::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_PRESS)
	{
		requestFocus();
	}
	else if ((getSelectionStrategy() == SINGLE) && (event.getType() == EVENT_TYPE_KEY))
	{
		KeyEvent& kevent = (KeyEvent&)event;
		bool processed = false;
		if ((kevent.getKeyType() == KeyEvent::SPECIAL) && (kevent.getEventType() == KeyEvent::UP))
		{
			size_t selection_index = 0;
			DrawOrder order = getOrder();
			if (!mSelected.empty())
			{
				Tag selected = mSelected.begin()->first;
				selection_index = getPosition(selected);
			}
			switch (kevent.getKeyCode())
			{
			case KeyEvent::VKEY_UP:
				if (!selection_index)
					selection_index = order.size() - 1;
				else
					selection_index -= 1;
				break;
			case KeyEvent::VKEY_DOWN:
				selection_index = (selection_index + 1) % order.size();
				break;
			}
			select(getByPosition(selection_index));
			raise(*new DrawRequestEvent(this));
			event.setProcessed();
		}
	}
	return IVerticalMenu::onEvent(event);
}

void VerticalMenu::select(const Tag& selected)
{
	IVerticalMenu::select(selected);
	if (!mSelected.empty() && (getSelectionStrategy() == SINGLE))
	{
		Tag selected = mSelected.begin()->first;
		int pos = (int)mVerticalLocation[selected];
		int height = (int)mItemsVerticalSize[selected];
		if ((mHeight != (size_t) -1) && ((int) (pos + Scroller::mYOffset + height) > (int) mHeight))
		{
			scroll(0, (int) (mHeight - (int) pos - (int) height));
		}
		if (pos < -Scroller::mYOffset)
		{
			scroll(0, -pos);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

HorizontalMenu::HorizontalMenu(IFocusController& fc, const size_t vsize) : IHorizontalMenu(vsize), IFocusable(fc)
{
}

HorizontalMenu::~HorizontalMenu()
{
}

Margins HorizontalMenu::draw(ISurface& surface)
{
	return IHorizontalMenu::draw(surface);
}

bool HorizontalMenu::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (event.getType() == EVENT_TYPE_PRESS)
	{
		requestFocus();
	}
	else if ((getSelectionStrategy() == SINGLE) && (event.getType() == EVENT_TYPE_KEY))
	{
		KeyEvent& kevent = (KeyEvent&)event;
		bool processed = false;
		if ((kevent.getKeyType() == KeyEvent::SPECIAL) && (kevent.getEventType() == KeyEvent::UP))
		{
			size_t selection_index = 0;
			DrawOrder order = getOrder();
			if (!mSelected.empty())
			{
				Tag selected = mSelected.begin()->first;
				selection_index = getPosition(selected);
			}
			switch (kevent.getKeyCode())
			{
			case KeyEvent::VKEY_LEFT:
				if (!selection_index)
					selection_index = order.size() - 1;
				else
					selection_index -= 1;
				break;
			case KeyEvent::VKEY_RIGHT:
				selection_index = (selection_index + 1) % order.size();
				break;
			}
			select(getByPosition(selection_index));
			raise(*new DrawRequestEvent(this));
			event.setProcessed();
		}
	}
	return IHorizontalMenu::onEvent(event);
}

void HorizontalMenu::select(const Tag& selected)
{
	IHorizontalMenu::select(selected);
	if (!mSelected.empty() && (getSelectionStrategy() == SINGLE))
	{
		Tag selected = mSelected.begin()->first;
		int pos = (int)mHorizontalLocation[selected];
		int width = (int)mItemsHorizontalSize[selected];
		if ((mWidth != (size_t) -1) && ((int) (pos + Scroller::mXOffset + width) > (int) mWidth))
		{
			scroll((int)mWidth - pos - width, 0);
		}
		if (pos < -Scroller::mXOffset)
		{
			scroll(-pos, 0);
		}
	}
}
