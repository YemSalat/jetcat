#ifndef __FONT_H__
#define __FONT_H__

#include "character.h"
#include "FIndex.h"
#include "../Drawing/brush.h"
#include "../Drawing/viewport.h"
#include <list>
#include <map>

namespace Componentality
{
	namespace Graphics
	{
		class FontSet;
		class IndexedFont;

		std::list<int> stringToList(const std::string&);
		std::string listToString(std::list<int>&);

		class Font : public CST::Common::LibJSON::Object
		{
			friend class FontSet;
			friend class IndexedFont;
		public:
			enum FontType
			{
				FONT_NORMAL		 = 0x00,
				FONT_BOLD		 = 0x01,
				FONT_ITALIC		 = 0x02,
				FONT_BOLD_ITALIC = 0x03,

				FONT_NONE		 = 0xFF
			};
			class PrintCharacterCallback
			{
				friend class Font;
			public:
				PrintCharacterCallback() {};
				virtual ~PrintCharacterCallback() {};
			protected:
				virtual void onCharacterPrinting(const size_t index, const int character, const Point& start_location) = 0;
				virtual void onCharacterPrinted(const size_t index, const int character, const Point& end_location) = 0;
			};
		protected:
			std::map<int, Character*> mChars;				// List of characters
			std::map<long long, int> mKerning;				// Kerning sheet
			FontType mType;
			std::string mFontFace;
			int mSize;										// Size of the font
			int mBase;										// Font baseline location
			int mHeight;									// Font line height
			Color mForeground;
		public:
			Font() { mType = FONT_NONE; };
			Font(const std::string& json);
			virtual ~Font();
			virtual bool readFNT(const std::string& fnt, ISurface& texture);
			virtual std::string serialize(const bool no_chars = false);
			virtual bool deserialize(const std::string&);
			virtual Point print(ISurface&, const Point, const std::string&, PrintCharacterCallback* = NULL);
			virtual Point print(ISurface&, const Point, const std::list<int>&, PrintCharacterCallback* = NULL);
			virtual std::pair<size_t, size_t> measure(const std::string&);
			virtual std::pair<size_t, size_t> measure(const std::list<int>&);
		public:
			FontType getType() const { return mType; }
			std::string getFace() const { return mFontFace; }
			int getSize() const { return mSize; }
			int getBaseline() const { return mBase; }
			int getHeight() const { return mHeight; }
		public:
			virtual void setForeground(const Color& fg) { mForeground = fg; }
		protected:
			virtual Point print(ISurface&, const Point, const int, const int = -1);
			virtual std::pair<size_t, size_t> Parse(const std::string&, const size_t, const size_t);
			virtual std::string Serialize(const CST::Common::LibJSON::OPTIONS options = CST::Common::LibJSON::OPTIONS());
			virtual bool parse(CST::Common::LibJSON::Object&);
			virtual Character* loadCharacter(const int id) { return mChars[id]; }
		};

		class IndexedFont : public Font
		{
		protected:
			LoadTable mIndex;
			std::ifstream* mCharFile;
		public:
			IndexedFont();
			IndexedFont(Font&);
			IndexedFont(std::ifstream&);
			virtual ~IndexedFont();
			virtual std::string serialize(std::ofstream& charfile);
			virtual bool deserialize(const std::string&);
			virtual std::ifstream* getCharFile() { return mCharFile; }
			virtual void setCharFile(std::ifstream& file) { mCharFile = &file; }
		protected:
			virtual std::string serialize(const bool no_chars = false) { return Font::serialize(no_chars); };
			virtual Character* loadCharacter(const int id);
		};
	} // namespace Graphics
} // namespace Componentality

#endif