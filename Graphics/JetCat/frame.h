#ifndef __FRAME_H__
#define __FRAME_H__

#include "drawable.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class Frame : public IElement
			{
			protected:
				size_t mWidth;
				Componentality::Graphics::Color mColor;
			public:
				Frame();
				virtual ~Frame();
				virtual Margins draw(ISurface&);
				virtual size_t getWidth() const { return mWidth; }
				virtual void setWidth(const size_t width) { mWidth = width; }
				virtual Componentality::Graphics::Color getColor() { return mColor; }
				virtual void setColor(const Componentality::Graphics::Color color) { mColor = color; }
			};
		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
