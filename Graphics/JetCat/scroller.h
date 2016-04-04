#ifndef __SCROLLER_H__
#define __SCROLLER_H__

#include "drawable.h"
#include "placement.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class Scroller : public Placement
			{
			protected:
				int mXOffset;
				int mYOffset;
			public:
				Scroller(const int h = 0, const int v = 0);
				virtual ~Scroller();
				virtual Margins draw(ISurface&);
				virtual void scroll(const int h, const int v);
				virtual std::pair<int, int> getScroll() const;
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
