#include "tabview.h"

using namespace Componentality::Graphics::JetCat;
using namespace Componentality::Graphics;

TabView::TabView()
{
}

TabView::~TabView()
{
}

Margins TabView::draw(ISurface& surface)
{
	IElement* tab = get(mCurrentTab);
	for (std::map<std::string, IDrawable*>::iterator i = mSubItems.begin(); i != mSubItems.end(); i++)
		if (i->first != mCurrentTab)
			i->second->hide();
		else
			i->second->show();
	if (tab)
	{
		size_t width = tab->getRightBottom().x - tab->getTopLeft().x;
		size_t height = tab->getRightBottom().y - tab->getTopLeft().y;
		tab->setTopLeft(getTopLeft());
		tab->setRightBottom(getTopLeft() + Point(width, height));
		return tab->draw(surface);
	}
	else
		return Margins();
}

void TabView::add(const Tag tab, IElement& element)
{
	IElement::add(tab, element);
	if (mCurrentTab.empty())
		mCurrentTab = tab;
}

void TabView::remove(const Tag tab)
{
	IElement::remove(tab);
}

void TabView::select(const Tag tab)
{
	mCurrentTab = tab;
}

IElement* TabView::get(const Tag tab) const
{
	if (mSubItems.find(tab) != mSubItems.end())
		return (IElement*) mSubItems.find(tab)->second;
	else
		return NULL;
}

std::list<Tag> TabView::list()
{
	std::list<std::string> result;
	for (std::map<std::string, IDrawable*>::const_iterator i = mSubItems.begin(); i != mSubItems.end(); i++)
		result.push_back(i->first);
	return result;
}
