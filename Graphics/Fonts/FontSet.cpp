#include "FontSet.h"
#include "../../../common-libs/common/common_utilities.h"
#include "../../../common-libs/libXjson/xjson_conv.h"

#include <stdlib.h>

using namespace Componentality::Graphics;
using namespace CST::Common::LibJSON;

static const std::string font__fonts = "fonts";
static const std::string XJSON_LABEL = "xjsoncjf";
static const char FONTS_SEPARATOR = '\n';
static const std::string FONT_FILE_EXT = ".cjfont";
static const std::string CHAR_FILE_EXT = ".cjchar";

FontSet::FontDescriptor::FontDescriptor(const std::string src)
{
	operator=(src);
}

FontSet::FontDescriptor::FontDescriptor(const Font& font)
{
	operator=(font);
}

FontSet::FontDescriptor::FontDescriptor(const FontDescriptor& fd)
{
	operator=(fd);
}

FontSet::FontDescriptor::operator std::string() const
{
	if (mCache.empty())
	{
		char size[20], type[4];
		sprintf(size, "%d", mSize);
		sprintf(type, "%d", (int)mType);
		mCache = mFace + ":" + size + ":" + type;
	}
	return mCache;
}

bool Componentality::Graphics::operator==(const FontSet::FontDescriptor& self, const FontSet::FontDescriptor& fd)
{
	return (self.getFace() == fd.getFace()) && (self.getSize() == fd.getSize()) && (self.getType() == fd.getType());
}

bool Componentality::Graphics::operator!=(const FontSet::FontDescriptor& self, const FontSet::FontDescriptor& fd)
{
	return !(self == fd);
}

bool Componentality::Graphics::operator<(const FontSet::FontDescriptor& self, const FontSet::FontDescriptor& fd)
{
	if (self.getFace() < fd.getFace())
		return true;
	else
		if (self.getFace() == fd.getFace())
			if (self.getSize() < fd.getSize())
				return true;
			else
				if (self.getSize() == fd.getSize())
					return self.getType() < fd.getType();
	return false;
}

bool Componentality::Graphics::operator>(const FontSet::FontDescriptor& self, const FontSet::FontDescriptor& fd)
{
	if (self.getFace() > fd.getFace())
		return true;
	else
		if (self.getFace() == fd.getFace())
			if (self.getSize() > fd.getSize())
				return true;
			else
				if (self.getSize() == fd.getSize())
					return self.getType() > fd.getType();
	return false;
}

bool Componentality::Graphics::operator<=(const FontSet::FontDescriptor& self, const FontSet::FontDescriptor& fd)
{
	return operator<(self, fd) || operator==(self, fd);
}

bool Componentality::Graphics::operator>=(const FontSet::FontDescriptor& self, const FontSet::FontDescriptor& fd)
{
	return operator>(self, fd) || operator==(self, fd);
}

FontSet::FontDescriptor& FontSet::FontDescriptor::operator=(const std::string str)
{
	std::string face, size, type;
	size_t i;
	for (i = 0; i < str.size(); i++)
	{
		if (str[i] != ':')
			face += str[i];
		else
			break;
	}
	i += 1;
	for (; i < str.size(); i++)
	{
		if (str[i] != ':')
			size += str[i];
		else
			break;
	}
	i += 1;
	for (; i < str.size(); i++)
	{
		if (str[i] != ':')
			type += str[i];
		else
			break;
	}
	if (size.empty())
		size = "0";
	if (type.empty())
		type = "0";
	int _type, _size;
	sscanf(size.c_str(), "%d", &_size);
	sscanf(type.c_str(), "%d", &_type);
	mType = (Font::FontType) _type;
	mSize = _size;
	mFace = face;
	mCache.clear();
	return *this;
}

FontSet::FontDescriptor& FontSet::FontDescriptor::operator=(const Font& font)
{
	mFace = font.getFace();
	mType = font.getType();
	mSize = font.getSize();
	mCache.clear();
	return *this;
}

FontSet::FontDescriptor& FontSet::FontDescriptor::operator=(const FontDescriptor& fd)
{
	mFace = fd.mFace;
	mSize = fd.mSize;
	mType = fd.mType;
	mCache.clear();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

FontSet::FontSet()
{
}

FontSet::~FontSet()
{
}

std::string FontSet::serialize()
{
	Object self;
	Array fonts;

	for (std::map<FontDescriptor, Font*>::iterator i = mFonts.begin(); i != mFonts.end(); i++)
		fonts.Push(i->second);

	self.AddObject(font__fonts, &fonts);
	CST::Common::xJSON::JSONtoBIN j2b;
#ifdef FONTS_USE_XJSON
	std::string _serial = self.Serialize();
	size_t begin = 0, end = _serial.size();
	std::string serial = j2b(_serial, begin, end);
	std::string result = XJSON_LABEL + j2b.serialize() + serial;
#else
	std::string result = self.Serialize();
#endif
	self.RemoveObject(font__fonts);

	fonts.Clean();
	return result;
}

bool FontSet::deserialize(const std::string& _src)
{
	std::string src = _src;
	if (src.substr(0, XJSON_LABEL.size()) == XJSON_LABEL)
	{
		CST::Common::xJSON::BINtoJSON b2j;
		src.erase(0, XJSON_LABEL.size());
		src = b2j(src);
	}
	std::pair<size_t, size_t> result = Parse(src, 0, src.size());

	if (result == Parseable::ERROR_VAL)
		return false;

	JsonObject* obj = this->operator[](font__fonts);

	if (obj)
	{
		if (obj->GetObjectType() == ARRAY)
		{
			Array* array = (Array*) obj;
			for (size_t i = 0; i < array->Size(); i++)
			{
				JsonObject* obj = (*array)[i];
				if (obj->GetObjectType() == OBJECT)
				{
					Font* font = new Font;
					font->parse(*(Object*)obj);
					mFonts[FontDescriptor(*font)] = font;
				}
			}
		}
	}
	Reset();
	return true;
}

Font* FontSet::find(const FontDescriptor& fd)
{
	return mFonts[fd];
}

bool FontSet::append(Font& font)
{
	FontDescriptor fd(font);
	if (mFonts.find(fd) != mFonts.end())
		return false;
	mFonts[fd] = &font;
	return true;
}

bool FontSet::remove(Font& font)
{
	FontDescriptor fd(font);
	if (mFonts.find(fd) != mFonts.end())
	{
		delete mFonts.find(fd)->second;
		mFonts.erase(mFonts.find(fd));
		return true;
	}
	return false;
}

std::list<FontSet::FontDescriptor> FontSet::list() const
{
	std::list<FontDescriptor> result;
	for (std::map<FontDescriptor, Font*>::const_iterator i = mFonts.begin(); i != mFonts.end(); i++)
		result.push_back(i->first);
	return result;
}

////////////////////////////////////////////////////////////////////////////

LoadableFontSet::LoadableFontSet(const std::string folder) : mFolder(folder) 
{
}

LoadableFontSet::~LoadableFontSet()
{
	for (std::map<FontDescriptor, std::ifstream*>::iterator i = mCharFiles.begin(); i != mCharFiles.end(); i++)
		if (i->second)
		{
			i->second->close();
			delete i->second;
		}
}

std::string LoadableFontSet::serialize(const FontSet::FontDescriptor& fd)
{
	std::string font_string = fd;
	std::pair<std::string, std::string> font_fnames = convertToFilenames(font_string);
	std::string char_filename = CST::Common::fileJoinPaths(mFolder, font_fnames.second);
	std::ofstream charfile(char_filename.c_str(), std::ios::binary);
	std::string font = ((IndexedFont*)find(fd))->serialize(charfile);
	std::string font_filename = CST::Common::fileJoinPaths(mFolder, font_fnames.first);
	std::ofstream fontfile(font_filename.c_str(), std::ios::binary);
	fontfile.write(font.c_str(), font.size());
	charfile.close();
	fontfile.close();
	return font_string;
}


std::string LoadableFontSet::serialize()
{
	std::string result;
	for (std::map<FontDescriptor, Font*>::const_iterator i = mFonts.begin(); i != mFonts.end(); i++)
	{
		serialize(i->first);
		result += i->first.operator std::string() + FONTS_SEPARATOR;
	}
	return result;
}

bool LoadableFontSet::deserialize(const std::string& src)
{
	std::string font_desc;
	for (std::map<FontDescriptor, Font*>::const_iterator i = mFonts.begin(); i != mFonts.end(); i++)
	{
		if (i->second)
			delete i->second;
		if (mCharFiles.find(i->first) != mCharFiles.end())
		{
			mCharFiles[i->first]->close();
			delete mCharFiles[i->first];
		}
	}
	mFonts.clear();
	mCharFiles.clear();
	for (size_t i = 0; i < src.size(); i++)
	{
		if (src[i] == FONTS_SEPARATOR)
		{
			mFonts[font_desc] = NULL;
			font_desc.clear();
		}
		else
			font_desc += src[i];
	}
	return true;
}

bool LoadableFontSet::deserialize()
{
	std::list<std::string> files = CST::Common::listFiles(mFolder);
	std::string font_list;
	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++)
	{
		std::string subname = i->substr(i->size() - FONT_FILE_EXT.size());
		if (subname == FONT_FILE_EXT)
		{
			std::string descr = i->substr(0, i->size() - FONT_FILE_EXT.size());
			for (size_t i = 0; i < descr.size(); i++)
				if (descr[i] == '_')
					descr[i] = ':';
			font_list += descr + FONTS_SEPARATOR;
		}
	}
	return deserialize(font_list);
}

Font* LoadableFontSet::find(const FontDescriptor& fdsc)
{
	return getFont(fdsc);
}

IndexedFont* LoadableFontSet::getFont(const FontSet::FontDescriptor& fdsc)
{
	std::map<FontDescriptor, Font*>::iterator ptr = mFonts.find(fdsc);
	if (ptr == mFonts.end())
		return NULL;
	Font* found = ptr->second;
	if (found)
		return (IndexedFont*)found;
	std::pair<std::string, std::string> font_fnames = convertToFilenames(fdsc);
	CST::Common::blob font_content = CST::Common::fileRead(CST::Common::fileJoinPaths(mFolder, font_fnames.first));
	std::string str_font_content = CST::Common::blobToString(font_content);
	std::string fname = CST::Common::fileJoinPaths(mFolder, font_fnames.second);
	std::ifstream& char_file = *new std::ifstream(fname.c_str(), std::ios::binary);
	bool good = char_file.good() && !char_file.bad();
	mCharFiles[fdsc] = &char_file;
	IndexedFont* font = new IndexedFont(char_file);
	mFonts[fdsc] = font;
	font->deserialize(str_font_content);
	font_content.purge();
	return font;
}

std::pair<std::string, std::string> LoadableFontSet::convertToFilenames(const FontSet::FontDescriptor& fdsc)
{
	std::pair<std::string, std::string> result;

	std::string fonttype = fdsc;
	for (size_t i = 0; i < fonttype.size(); i++)
		if ((fonttype[i] <= ' ') || (fonttype[i] == ':'))
			fonttype[i] = '_';
	result.first = fonttype + FONT_FILE_EXT;
	result.second = fonttype + CHAR_FILE_EXT;
	return result;
}

bool LoadableFontSet::unload(const FontDescriptor& fdsc)
{
	std::string key = fdsc;
	if (mFonts.find(key) != mFonts.end())
	{
		std::ifstream* charfile = ((IndexedFont*)mFonts[key])->getCharFile();
		if (charfile)
			charfile->close();
		delete mFonts[key];
		mFonts[key] = NULL;
	}
	if (mCharFiles.find(key) != mCharFiles.end())
	{
		mCharFiles[key]->close();
		delete mCharFiles[key];
		mCharFiles.erase(mCharFiles.find(key));
	}
	return true;
}
