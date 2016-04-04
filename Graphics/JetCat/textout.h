#ifndef __TEXTOUT_H__
#define __TEXTOUT_H__

#include "drawable.h"
#include "../Fonts/FontSet.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class ITextOutput
			{
			public:
				enum HorizontalAlignment
				{
					H_LEFT,
					H_RIGHT,
					H_CENTER,
					H_JUSTIFY
				};
				enum VerticalAlignment
				{
					V_TOP,
					V_BOTTOM,
					V_CENTER
				};
			protected:
				ISurface& mSurface;
				FontSet::FontDescriptor mFont;
				Margins mMargins;
				HorizontalAlignment mHAlign;
				VerticalAlignment mVAlign;
				Color mColor;
			public:
				ITextOutput(ISurface& surface) : mSurface(surface) { mHAlign = H_CENTER; mVAlign = V_CENTER; };
				virtual ~ITextOutput() {};
				virtual void print(const std::string&) = 0;
				virtual void print(const std::list<int>&) = 0;
				virtual FontSet::FontDescriptor getFont() const { return mFont; }
				virtual void setFont(const FontSet::FontDescriptor font) { mFont = font; }
				virtual Margins getMargins() const { return mMargins; }
				virtual void setMargins(const Margins margins) { mMargins = margins; }
				virtual Color getColor() const { return mColor; }
				virtual void setColor(const Color color) { mColor = color; }
				virtual HorizontalAlignment getHAlignment() const { return mHAlign; }
				virtual void setHAlignment(const HorizontalAlignment halign) { mHAlign = halign; }
				virtual VerticalAlignment getVAlignment() const { return mVAlign; }
				virtual void setVAlignment(const VerticalAlignment valign) { mVAlign = valign; };
				virtual std::pair<size_t, size_t> measure(const std::string&);
				virtual std::pair<size_t, size_t> measure(const std::list<int>&);
			};

			class ITextCutter
			{
			protected:
				FontSet& mFontSet;
				FontSet::FontDescriptor mFont;
			public:
				ITextCutter(FontSet& fontset, FontSet::FontDescriptor font) : mFontSet(fontset), mFont(font) {};
				virtual ~ITextCutter() {}
				virtual std::list<int> operator()(ISurface&, const std::string) = 0;
				virtual std::list<int> operator()(ISurface&, const std::list<int>&) = 0;
			};

			class SingleLineCutter : public ITextCutter
			{
			public:
				SingleLineCutter(FontSet& fontset, FontSet::FontDescriptor font) : ITextCutter(fontset, font) {};
				virtual ~SingleLineCutter() {};
				virtual std::list<int> operator()(ISurface&, const std::string);
				virtual std::list<int> operator()(ISurface&, const std::list<int>&);
			};

			class SingleLineEllipsisCutter : public SingleLineCutter
			{
			public:
				SingleLineEllipsisCutter(FontSet& fontset, FontSet::FontDescriptor font) : SingleLineCutter(fontset, font) {};
				virtual ~SingleLineEllipsisCutter() {};
				virtual std::list<int> operator()(ISurface&, const std::list<int>&);
			};

			class EasyTextOutput : public ITextOutput
			{
			public:
				EasyTextOutput(ISurface&);
				virtual ~EasyTextOutput();
				virtual void print(const std::string&);
				virtual void print(const std::list<int>&);
			};

			class StandardTextOutput : public EasyTextOutput
			{
			protected:
				virtual ITextCutter* getTextCutter(FontSet&, FontSet::FontDescriptor&);
			public:
				StandardTextOutput(ISurface&);
				virtual ~StandardTextOutput();
				virtual void print(const std::list<int>&);
			};

			class MultilineTextOutput : public EasyTextOutput
			{
			protected:
				size_t mInterlinePadding;
			public:
				MultilineTextOutput(ISurface&);
				virtual ~MultilineTextOutput();
				virtual void print(const std::list<int>&);
				virtual void setInterlineSpace(const size_t padding) { mInterlinePadding = padding; }
				virtual size_t getInterlineSpace() const { return mInterlinePadding; }
				virtual std::pair<size_t, size_t> measure(const std::list<int>&);
			protected:
				size_t measureVertical(const std::list<int>&);
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
