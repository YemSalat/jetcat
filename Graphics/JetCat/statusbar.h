#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

#include "global.h"
#include "drawable.h"
#include "caption.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class StatusBar : public Caption
			{
			public:
				StatusBar();
				virtual ~StatusBar();
				virtual Margins draw(ISurface&);
			};

		}  // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
