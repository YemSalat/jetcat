#ifndef __EVTFILTER_H__
#define __EVTFILTER_H__

#include "../../../common-libs/common/common_events.h"
#include <map>
#include <string>

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class IElement;

			class IEventFilter
			{
			public:
				IEventFilter() {};
				virtual ~IEventFilter() {};
				virtual bool filter(CST::Common::Event&, IElement&) = 0;
			};

			class PointerEventStdFilter : public IEventFilter
			{
			public:
				PointerEventStdFilter() {};
				virtual ~PointerEventStdFilter() {};
				virtual bool filter(CST::Common::Event& event, IElement& element);
			};

			class IEventFilterSet : public IEventFilter
			{
			protected:
				std::map<std::string, IEventFilter*> mFilters;
			public:
				IEventFilterSet() {};
				virtual ~IEventFilterSet() {};
				virtual bool filter(CST::Common::Event&, IElement&);
			public:
				virtual void add(const std::string, IEventFilter*);
				virtual void remove(const std::string);
			};

			class StdEventFilter : public IEventFilterSet
			{
			protected:
				PointerEventStdFilter mPointerFilter;
			public:
				StdEventFilter();
				virtual ~StdEventFilter() {};
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif