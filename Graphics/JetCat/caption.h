#ifndef __CAPTION_H__
#define __CAPTION_H__

#include "global.h"
#include "drawable.h"
#include "textout.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class Caption : public IElement
			{
			protected:
				size_t mHeight;
				Margins mMargins;
				Componentality::Graphics::Color mColor;
				Componentality::Graphics::Color mLineColor;
				Componentality::Graphics::Color mFontColor;
				Componentality::Graphics::FontSet::FontDescriptor mFont;
				std::string mTitle;
			public:
				Caption();
				virtual ~Caption() {};
				virtual Margins draw(ISurface&);
				virtual size_t getHeight() const { return mHeight; }
				virtual void setHeight(const size_t height) { mHeight = height; }
				virtual Componentality::Graphics::Color getColor() { return mColor; }
				virtual void setColor(const Componentality::Graphics::Color color) { mColor = color; }
				virtual Componentality::Graphics::Color getLineColor() { return mLineColor; }
				virtual void setLineColor(const Componentality::Graphics::Color color) { mLineColor = color; }
				virtual Componentality::Graphics::Color getFontColor() { return mFontColor; }
				virtual void setFontColor(const Componentality::Graphics::Color color) { mFontColor = color; }
				virtual std::string getTitle() const { return mTitle; }
				virtual void setTitle(const std::string title) { mTitle = title; }
				virtual Componentality::Graphics::FontSet::FontDescriptor getFont() const { return mFont; }
				virtual void setFont(Componentality::Graphics::FontSet::FontDescriptor font) { mFont = font; }
				virtual void setMargins(const Margins margins) { mMargins = margins; }
				virtual Margins getMargins() const { return mMargins; }
			protected:
				virtual ITextOutput* provideTextOutput(ISurface& surface) { return new StandardTextOutput(surface); }
			};
		}
	} // namespace Graphics
} // namespace Componentality

#endif