#include "evtfilter.h"
#include "drawable.h"
#include "inputs.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

bool PointerEventStdFilter::filter(CST::Common::Event& event, IElement& element)
{
	PointerEvent* evt = (PointerEvent*)&event;
	return evt->filter(element.getTopLeft(), element.getRightBottom());
}

bool IEventFilterSet::filter(CST::Common::Event& evt, IElement& element)
{
	std::map<std::string, IEventFilter*>::iterator ptr = mFilters.find(evt.getType());
	if (ptr != mFilters.end())
		return ptr->second->filter(evt, element);
	return true;			// Filter must pass all the messages which it doesn't know how to filter
}

void IEventFilterSet::add(const std::string type, IEventFilter* filter)
{
	mFilters[type] = filter;
}

void IEventFilterSet::remove(const std::string type)
{
	if (mFilters.find(type) != mFilters.end())
		mFilters.erase(mFilters.find(type));
}

StdEventFilter::StdEventFilter()
{
	add(EVENT_TYPE_POINTER, &mPointerFilter);
	add(EVENT_TYPE_PRESS, &mPointerFilter);
	add(EVENT_TYPE_DEPRESS, &mPointerFilter);
}