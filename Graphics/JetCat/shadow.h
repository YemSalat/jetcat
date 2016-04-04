#ifndef __SHADOW_H__
#define __SHADOW_H__

#include "drawable.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class Shadow : public IElement
			{
			protected:
				size_t mWidth;
			public:
				Shadow(const size_t width = 3) : mWidth(width) {};
				virtual ~Shadow() {};
				virtual Margins draw(ISurface&);
				virtual size_t getWidth() const { return mWidth; }
				virtual void setWidth(const size_t width) { mWidth = width; }
			};
		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif