#ifndef __FONTSET_H__
#define __FONTSET_H__

#include "Font.h"

namespace Componentality
{
	namespace Graphics
	{

		class FontSet : public CST::Common::LibJSON::Object
		{
		public:
			class FontDescriptor
			{
			protected:
				std::string mFace;					// Font face
				int mSize;							// Font size
				Font::FontType mType;				// Font type
				mutable std::string mCache;			// Cache of string representation
			public:
				FontDescriptor() : mFace(""), mSize(0), mType(Font::FONT_NONE) {};
				FontDescriptor(const std::string face, const int size, const Font::FontType type) : mFace(face), mSize(size), mType(type) {}
				FontDescriptor(const std::string);
				FontDescriptor(const Font&);
				FontDescriptor(const FontDescriptor&);
				virtual ~FontDescriptor() {};
				virtual operator std::string() const;
				virtual FontDescriptor& operator=(const std::string);
				virtual FontDescriptor& operator=(const Font&);
				virtual FontDescriptor& operator=(const FontDescriptor&);
			public:
				virtual Font::FontType getType() const { return mType; }
				virtual size_t getSize() const { return mSize; }
				virtual const std::string& getFace() const { return mFace; }
				virtual void setType(const Font::FontType type) { mType = type; }
				virtual void setSize(const size_t size) { mSize = size; }
				virtual void setFace(const std::string& face) { mFace = face; }
			};
		protected:
			std::map<FontDescriptor, Font*> mFonts;
		public:
			FontSet();
			virtual ~FontSet();
			virtual std::string serialize();
			virtual bool deserialize(const std::string&);
			virtual Font* find(const FontDescriptor&);
			virtual bool append(Font&);
			virtual bool remove(Font&);
			virtual std::list<FontDescriptor> list() const;
		};

		bool operator==(const FontSet::FontDescriptor&, const FontSet::FontDescriptor&);
		bool operator!=(const FontSet::FontDescriptor&, const FontSet::FontDescriptor&);
		bool operator<(const FontSet::FontDescriptor&, const FontSet::FontDescriptor&);
		bool operator>(const FontSet::FontDescriptor&, const FontSet::FontDescriptor&);
		bool operator<=(const FontSet::FontDescriptor&, const FontSet::FontDescriptor&);
		bool operator>=(const FontSet::FontDescriptor&, const FontSet::FontDescriptor&);


		class LoadableFontSet : public FontSet
		{
		protected:
			std::string mFolder;								// Folder where font and character files to be stored
			std::map<FontDescriptor, std::ifstream*> mCharFiles;	// Streams for fonts' character files
		public:
			LoadableFontSet(const std::string folder);
			virtual ~LoadableFontSet();
			virtual std::string serialize();
			virtual std::string serialize(const FontDescriptor&);
			virtual bool deserialize(const std::string&);
			virtual bool deserialize();
			virtual Font* find(const FontDescriptor&);
			virtual bool unload(const FontDescriptor&);
			virtual void setFolder(const std::string& folder) { mFolder = folder; }
			virtual std::string getFolder() const { return mFolder; }
		protected:
			virtual IndexedFont* getFont(const FontDescriptor&);
			virtual std::pair<std::string, std::string> convertToFilenames(const FontDescriptor&); // Make font and character file names for given descriptor
		};

	} // namespace Graphics
} // namespace Componentality

#endif