#include "inputs.h"
#include "drawable.h"
#include "../Drawing/viewport.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

inline Point sum(Point x, Point y)
{
	return Point(x.x + y.x, x.y + y.y);
}

bool IDrawable::filter(CST::Common::Event& event)
{
	if (((IInputEvent&)event).isNotToHidden() && isHidden()) 
		return false; 
	else 
		return true;
};

IElement::~IElement()
{
	for (DrawingList::iterator i = mSubItems.begin(); i != mSubItems.end(); i++)
	{
		delete i->second;
	}
}

Margins IElement::draw(ISurface& surface)
{
	Margins result;

	if (isHidden())
		return result;

	for (DrawOrder::iterator i = mOrder.begin(); i != mOrder.end(); i++)
	{
		DrawingList::iterator subitem = mSubItems.find(*i);
		if (subitem != mSubItems.end())
		{
			if (subitem->second->isHidden())
				continue;
			Point topleft(result.mLeft, result.mTop);
			Point bottomright(surface.getWidth() - result.mRight - 1, surface.getHeight() - result.mBottom - 1);
			ViewPort vp(surface, topleft, bottomright);
			subitem->second->setTopLeft(sum(getTopLeft(), topleft));
			subitem->second->setRightBottom(sum(getTopLeft(), bottomright));
			subitem->second->setZOrder(getZOrder() - 1);
			Margins submargins = subitem->second->draw(vp);
			if (!ignoreMargins())
			{
				result.mLeft += submargins.mLeft;
				result.mRight += submargins.mRight;
				result.mTop += submargins.mTop;
				result.mBottom += submargins.mBottom;
			}
		}
	}

	return result;
}

void IElement::add(const Tag tag, IElement& element)
{
	if (mSubItems.find(tag) == mSubItems.end())
		replace(tag, element);
}

void IElement::replace(const Tag tag, IElement& element)
{
	mSubItems[tag] = &element;
	element.mOwner = this;
	if (!isInOrder(tag))
		mOrder.push_back(tag);
}

void IElement::remove(const Tag tag)
{
	DrawingList::iterator ptr = mSubItems.find(tag);
	if (ptr != mSubItems.end())
	{
		((IElement*)ptr->second)->mOwner = NULL;
		mSubItems.erase(ptr);
	}
	for (DrawOrder::iterator i = mOrder.begin(); i != mOrder.end();)
	{
		if ((*i) == tag)
			i = mOrder.erase(i);
		else
			i++;
	}
}

std::list<Tag> IElement::list() const
{
	std::list<Tag> result;
	for (DrawingList::const_iterator i = mSubItems.begin(); i != mSubItems.end(); i++)
		result.push_back(i->first);
	return result;
}

bool IElement::isInOrder(Tag tag) const
{
	for (DrawOrder::const_iterator i = mOrder.begin(); i != mOrder.end(); i++)
		if (*i == tag)
			return true;
	return false;
}

ViewPort IElement::getDrawArea(ISurface& surface, const Margins& margins)
{
	Point topleft(margins.mLeft, margins.mTop);
	Point rightbottom(surface.getWidth() - margins.mLeft - margins.mRight, surface.getHeight() - margins.mTop - margins.mBottom);

	return ViewPort(surface, topleft, rightbottom);
}

bool IElement::onEvent(CST::Common::Event& event)
{
	if (!filter(event))
		return false;
	if (mOrder.empty())
		return false;
	bool result = false;
	DrawOrder::iterator i = mOrder.end();
	do
	{
		i--;
		result |= get(*i)->onEvent(event);
	} while (i != mOrder.begin() && !event.getProcessed());
	return result;
}

bool IElement::filter(CST::Common::Event& event)
{
	IEventFilter* filter = getEventFilter();
	if (!filter)
		return IDrawable::filter(event);
	else
		return IDrawable::filter(event) && filter->filter(event, *this);
}

IEventFilter* IElement::getEventFilter()
{
	if (mEvtFilter)
		return mEvtFilter;
	else
		if (mOwner)
			return mOwner->getEventFilter();
		else
			return NULL;
}

bool IElement::isHidden() const
{
	if (IDrawable::isHidden())
		return true;
	if (mOwner)
		return mOwner->isHidden();
	return false;
}

IDrawable* IElement::get(const Tag tag) 
{ 
	DrawingList::iterator ptr = mSubItems.find(tag);
	if (ptr != mSubItems.end())
		return ptr->second; 
	else 
		return NULL; 
}

///////////////////////////////////////////////////////////////////////////////////////

void Restorable::backup(ISurface& surface)
{
	if ((surface.getWidth() != (size_t)-1) &&
		(surface.getHeight() != (size_t)-1))
	{
		if (!mStorage)
			mStorage = new BitmapSurface(surface.getWidth(), surface.getHeight());
		mStorage->apply(surface);
	}
}

void Restorable::restore(ISurface& surface)
{
	if (mStorage)
		surface.apply(*mStorage);
}

