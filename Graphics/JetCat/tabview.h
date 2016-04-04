#ifndef __TABVIEW_H__
#define __TABVIEW_H__

#include "drawable.h"
#include <map>
#include <list>

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class TabView : public IElement
			{
			protected:
				std::string mCurrentTab;
			public:
				TabView();
				virtual ~TabView();
				virtual Margins draw(ISurface&);
			public:
				virtual void add(const Tag, IElement&);
				virtual void remove(const Tag);
				virtual void select(const Tag);
				virtual IElement* get(const Tag) const;
				virtual std::list<Tag> list();
			};
		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
