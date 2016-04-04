#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "drawable.h"
#include "textout.h"
#include "../Fonts/FontSet.h"
#include "../../../common-libs/common/common_events.h"
#include <map>

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class IColorDefinitions
			{
			public:
				enum SystemColorType
				{
					SYSCOLOR_WINDOW_BACKGROUND,
					SYSCOLOR_WINDOW_FRAME,
					SYSCOLOR_WINDOW_SHADOW,
					SYSCOLOR_WINDOW_CAPTION_BACKGROUND,
					SYSCOLOR_WINDOW_CAPTION_TEXT,
					SYSCOLOR_WINDOW_CAPTION_LINE,
					SYSCOLOR_WINDOW_STATUS_BACKGROUND,
					SYSCOLOR_WINDOW_STATUS_TEXT,
					SYSCOLOR_WINDOW_STATUS_LINE,
					SYSCOLOR_EDITBOX_FONT,
					SYSCOLOR_EDITBOX_BACKGROUND,
					SYSCOLOR_EDITBOX_CURSOR,
					SYSCOLOR_EDITBOX_FRAME,
					SYSCOLOR_MESSAGEBOX_TEXT
				};
			protected:
				std::map<SystemColorType, Componentality::Graphics::Color> mColors;
			public:
				IColorDefinitions();
				virtual ~IColorDefinitions() {};
				virtual Componentality::Graphics::Color getColor(const SystemColorType type);
				virtual void setColor(const SystemColorType type, const Componentality::Graphics::Color);
			};

			class IFontDefinitions
			{
			public:
				enum SystemFontType
				{
					SYSFONT_WINDOW_CAPTION,
					SYSFONT_WINDOW_STATUS,
					SYSFONT_EDITBOX,
					SYSFONT_KEYPAD,
					SYSFONT_BUTTON,
					SYSFONT_MESSAGE_BOX
				};
			protected:
				std::map<SystemFontType, FontSet::FontDescriptor> mFonts;
				FontSet* mFontSet;
			public:
				IFontDefinitions();
				virtual ~IFontDefinitions();
				virtual FontSet::FontDescriptor getFont(const SystemFontType);
				virtual void setFont(const SystemFontType, FontSet::FontDescriptor);
				virtual FontSet& getFontSet();
				virtual void setFontSet(FontSet&);
			};

			class IMarginDefinitions
			{
			public:
				enum SystemMarginsType
				{
					SYSMARGINS_CAPTION_TEXT,
					SYSMARGINS_STATUS_TEXT,
					SYSMARGINS_EDITBOX
				};
			protected:
				std::map<SystemMarginsType, Margins> mMargins;
			public:
				IMarginDefinitions();
				virtual ~IMarginDefinitions();
				virtual Margins getMargins(const SystemMarginsType);
				virtual void setMargins(const SystemMarginsType, const Margins);
			};

			class IAlignmentDefinitions
			{
			public:
				typedef std::pair<ITextOutput::HorizontalAlignment, ITextOutput::VerticalAlignment> Alignments;
				enum SystemAlignmentsType
				{
					SYSALIGN_CAPTION_TEXT,
					SYSALIGN_STATUS_TEXT
				};
			protected:
				std::map<SystemAlignmentsType, Alignments> mAlignments;
			public:
				IAlignmentDefinitions();
				virtual ~IAlignmentDefinitions() {};
				virtual Alignments getAlignments(const SystemAlignmentsType align) { return mAlignments[align]; }
				virtual void setAlignments(const SystemAlignmentsType align, const Alignments value) { mAlignments[align] = value; }
			};

			class ISizeDefinitions
			{
			public:
				enum SystemSizeType
				{
					SYSSIZE_CAPTION_HEIGHT,
					SYSSIZE_STATUS_HEIGHT,
					SYSSIZE_BUTTON_WIDTH,
					SYSSIZE_BUTTON_HEIGHT,
					SYSSIZE_MESSAGEBOX_WIDTH,
					SYSSIZE_MESSAGEBOX_HEIGHT
				};
			protected:
				std::map<SystemSizeType, int> mSizes;
			public:
				ISizeDefinitions();
				virtual ~ISizeDefinitions() {};
				virtual int getSize(const SystemSizeType size);
				virtual void setSize(const SystemSizeType size, int value);
			};

			class ICursorDefinitions
			{
			public:
				enum CursorType
				{
					SYSCURSOR_HORIZONTAL,
					SYSCURSOR_VERTICAL
				};
			protected:
				CursorType mType;
				size_t mSize;
				size_t mIndent;
			public:
				ICursorDefinitions();
				virtual ~ICursorDefinitions();
				virtual CursorType getCursorType();
				virtual void setCursorType(const CursorType type);
				virtual size_t getCursorSize();
				virtual void setCursorSize(const size_t);
				virtual size_t getCursorIndent() { return mIndent; }
				virtual void setCursorIndent(const size_t indent) { mIndent = indent; }
			};

			class System : 
				public IColorDefinitions, 
				public IFontDefinitions, 
				public IMarginDefinitions, 
				public IAlignmentDefinitions,
				public ISizeDefinitions,
				public ICursorDefinitions
			{
			protected:
				static System* mInstance;
			protected:
				System() {};
			public:
				virtual ~System() {};
				static System& getSystemProperties();
			};


		}
	} // namespace Graphics
} // namespace Componentality

#endif
