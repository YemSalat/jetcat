#ifndef __LABEL_H__
#define __LABEL_H__

#include "global.h"
#include "drawable.h"
#include "textout.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class Label : public IElement
			{
			protected:
				mutable CST::Common::mutex mLock;
				std::list<int> mText;
				Componentality::Graphics::FontSet::FontDescriptor mFont;
				Margins mMargins;
				Componentality::Graphics::Color mColor;
				IAlignmentDefinitions::Alignments mAlignments;
			public:
				Label();
				Label(const std::string text);
				Label(const std::string text, Componentality::Graphics::Color color, Margins margins = Margins());
				Label(const std::list<int> text);
				Label(const std::list<int> text, Componentality::Graphics::Color color, Margins margins = Margins());
				virtual ~Label();
				virtual Margins draw(ISurface& surface);
				virtual std::list<int> getText() const;
				virtual void setText(const std::string text);
				virtual void setText(const std::list<int> text);
				virtual Componentality::Graphics::FontSet::FontDescriptor getFont() const;
				virtual void setFont(const Componentality::Graphics::FontSet::FontDescriptor font);
				virtual Margins getMargins() const;
				virtual void setMargins(const Margins margins = Margins());
				virtual Componentality::Graphics::Color getColor() const;
				virtual void setColor(const Componentality::Graphics::Color color);
				virtual IAlignmentDefinitions::Alignments getAlignments() const;
				virtual void setAlignment(const IAlignmentDefinitions::Alignments align);
			protected:
				virtual ITextOutput& getTextOutput(ISurface& surface) { return *new StandardTextOutput(surface); }
			};

			class EasyLabel : public Label
			{
			public:
				EasyLabel() {};
				EasyLabel(const std::string text) : Label(text) {};
				EasyLabel(const std::string text, Componentality::Graphics::Color color, Margins margins = Margins())
					: Label(text, color, margins) {}
				EasyLabel(const std::list<int> text) : Label(text) {};
				EasyLabel(const std::list<int> text, Componentality::Graphics::Color color, Margins margins = Margins())
					: Label(text, color, margins) {}
				virtual ~EasyLabel() {};
			protected:
				virtual ITextOutput& getTextOutput(ISurface& surface) { return *new EasyTextOutput(surface); }
			};

			class MultilineLabel : public Label
			{
			public:
				MultilineLabel() {};
				MultilineLabel(const std::string text) : Label(text) {};
				MultilineLabel(const std::string text, Componentality::Graphics::Color color, Margins margins = Margins())
					: Label(text, color, margins) {}
				MultilineLabel(const std::list<int> text) : Label(text) {};
				MultilineLabel(const std::list<int> text, Componentality::Graphics::Color color, Margins margins = Margins())
					: Label(text, color, margins) {}
				virtual ~MultilineLabel() {};
			protected:
				virtual ITextOutput& getTextOutput(ISurface& surface) { return *new MultilineTextOutput(surface); }
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality


#endif