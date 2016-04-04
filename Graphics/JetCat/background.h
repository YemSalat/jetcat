#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "drawable.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class Background : public IElement
			{
			protected:
				Componentality::Graphics::Color mColor;
			public:
				Background();
				virtual ~Background();
				virtual Margins draw(ISurface&);
				virtual Componentality::Graphics::Color getColor() { return mColor; }
				virtual void setColor(const Componentality::Graphics::Color color) { mColor = color; }
			};
		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif