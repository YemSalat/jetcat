#include "Font.h"
#include "../../../common-libs/common/common_utilities.h"

using namespace Componentality::Graphics;
using namespace CST::Common::LibJSON;

static std::string font__face   = "face";
static std::string font__bold   = "bold";
static std::string font__italic = "italic";
static std::string font__size   = "size";
static std::string font__base   = "base";
static std::string font__height = "lineHeight";
static std::string json__height = "height";
static std::string json__type   = "type";
static std::string json__characters = "chars";
static std::string font__kerning = "kerning";
static std::string font__first  = "first";
static std::string font__second = "second";
static std::string font__amount = "amount";
static std::string json__offset = "offset";

static std::string __info = "info";
static std::string __common = "common";
static std::string __char = "char";

static std::string __font = "font";
static std::string __index = "index";

Font::Font(const std::string& json)
{
	mType = FONT_NONE;
	deserialize(json);
}

Font::~Font()
{
	for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
		delete (i->second);
	mChars.clear();
	mKerning.clear();
}

std::string ____getFNTArgument(const std::string& src, const std::string& arg);

static std::string ____getFNTLineMarker(const std::string& src)
{
	std::string result;

	for (size_t i = 0; i < src.size(); i++)
	{
		if (src[i] <= ' ')
			break;
		else
			result += src[i];
	}
	return result;
}

#define PARSEATTR(attr)		\
if (attr.empty())			\
	return false;			\
	int __##attr;			\
	sscanf(attr.c_str(), "%d", &__##attr);


bool Font::readFNT(const std::string& fnt, ISurface& texture)
{
	for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
		delete (i->second);
	mChars.clear();
	mKerning.clear();
	std::string line = fnt;
	
	while (!line.empty())
	{
		if (____getFNTLineMarker(line) == "info")
		{
			std::string face = ____getFNTArgument(line, font__face);
			std::string size = ____getFNTArgument(line, font__size);
			std::string bold = ____getFNTArgument(line, font__bold);
			std::string ital = ____getFNTArgument(line, font__italic);
			if (face.size() > 1)
				if (face[0] == '"')
					face.erase(0, 1);
			if (face.size() > 1)
				if (face[face.size() - 1] == '"')
					face.erase(face.size() - 1, 1);
			mFontFace = face;
			PARSEATTR(size);
			PARSEATTR(bold);
			PARSEATTR(ital);
			mSize = __size;
			if (__bold && __ital)
				mType = FONT_BOLD_ITALIC;
			else if (__bold)
				mType = FONT_BOLD;
			else if (__ital)
				mType = FONT_ITALIC;
			else
				mType = FONT_NORMAL;
		}
		else if (____getFNTLineMarker(line) == "common")
		{
			std::string base = ____getFNTArgument(line, font__base);
			std::string height = ____getFNTArgument(line, font__height);
			PARSEATTR(base);
			PARSEATTR(height);
			mBase = __base;
			mHeight = __height;
		}
		else if (____getFNTLineMarker(line) == "char")
		{
			Character* character = new Character;
			int index = character->getFromFNT(line, texture);
			if (index >= 0)
				mChars[index] = character;
			else
			{
				delete character;
				return false;
			}
		}
		else if (____getFNTLineMarker(line) == "kerning")
		{
			std::string first = ____getFNTArgument(line, font__first);
			std::string second = ____getFNTArgument(line, font__second);
			std::string offset = ____getFNTArgument(line, font__amount);
			PARSEATTR(first);
			PARSEATTR(second);
			PARSEATTR(offset);
			long long index = (((long long)__first) << 32) | ((long long)__second);
			mKerning[index] = __offset;
		}

		size_t pos = line.find('\n');
		if (pos == (size_t)-1)
			line.clear();
		line.erase(0, pos + 1);
		if (line.size() && (line[0] <= ' '))
			line.erase(0, 1);
	}

	return true;
}

std::string Font::serialize(const bool no_chars)
{
	Number type = (long)mType;
	Number size = (long)mSize;
	Number base = (long)mBase;
	Number height = (long)mHeight;
	String face = mFontFace;

	Object chars;
	Object kerning;

	AddObject(json__type, &type);
	AddObject(json__height, &height);
	AddObject(font__face, &face);
	AddObject(font__size, &size);
	AddObject(font__base, &base);
	AddObject(json__characters, &chars);
	AddObject(font__kerning, &kerning);

	if (!no_chars)
		for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
			chars.AddObject(CST::Common::ntoa<int>(i->first), i->second);

	for (std::map<long long, int>::iterator i = mKerning.begin(); i != mKerning.end(); i++)
	{
		std::string key = CST::Common::ntoa<long long>(i->first);
		Number* kerning_val = new Number((long) i->second);
		kerning.AddObject(key, kerning_val);
	}

	std::string result = Object::Serialize();
	
	if (!no_chars)
		for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
			chars.RemoveObject(CST::Common::ntoa<int>(i->first));

	RemoveObject(font__kerning);
	RemoveObject(json__characters);
	RemoveObject(json__type);
	RemoveObject(json__height);
	RemoveObject(font__face);
	RemoveObject(font__size);
	RemoveObject(font__base);

	return result;
}

bool Font::deserialize(const std::string& source)
{
	Reset();
	for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
		delete (i->second);
	mChars.clear();
	mKerning.clear();
	std::pair<size_t, size_t> result = Object::Parse(source, 0, source.length());
	bool _result = (result != Parseable::ERROR_VAL);

	if (_result)
		return false;

	bool __result = parse(*this);
	Reset();

	return __result;
}

Point Font::print(ISurface& surface, const Point start, const std::string& text, PrintCharacterCallback* cb)
{
	return print(surface, start, stringToList((std::string&)text), cb);
}

Point Font::print(ISurface& surface, const Point start, const std::list<int>& text, PrintCharacterCallback* cb)
{
	int prev = -1;
	Point current = start;
	size_t counter = 0;
	for (std::list<int>::const_iterator i = text.begin(); i != text.end(); i++, counter++)
	{
		int symbol = *i;
		if (cb)
			cb->onCharacterPrinting(counter, symbol, current);
		current = print(surface, current, symbol, prev);
		if (cb)
			cb->onCharacterPrinted(counter, symbol, current);
		prev = symbol;
	}
	return current;
}

Point Font::print(ISurface& surface, const Point position, const int char_index, const int char_prev)
{
	Character* character = loadCharacter(char_index);
	if (!character)
		return position;
	int kerning_offset = 0;
	if (char_prev >= 0)
	{
		long long index = (((long long)char_prev) << 32) | ((long long)char_index);
		kerning_offset = mKerning[index];
	}
	character->setForeground(mForeground);
	character->apply(surface, position.x + character->mPrePrintXOffset + kerning_offset, position.y + character->mPrePrintYOffset);
	Point result = position;
	result.x += character->mPostPrintXOffset + kerning_offset;
	result.y += character->mPostPrintYOffset;
	return result;
}

std::pair<size_t, size_t> Font::Parse(const std::string& src, const size_t begin, const size_t end)
{
	std::string _src = src.substr(begin, end - begin);
	if (!deserialize(_src))
		return Parseable::ERROR_VAL;
	else
		return std::pair<size_t, size_t>(begin, end);
}

std::string Font::Serialize(const CST::Common::LibJSON::OPTIONS options)
{
	return serialize();
}
	
bool Font::parse(CST::Common::LibJSON::Object& obj)
{
	JsonObject* chars = obj[json__characters];
	JsonObject* type = obj[json__type];
	JsonObject* height = obj[json__height];
	JsonObject* face = obj[font__face];
	JsonObject* size = obj[font__size];
	JsonObject* base = obj[font__base];
	JsonObject* kerning = obj[font__kerning];

	if (chars && (chars->GetObjectType() == JsonObject::OBJECT))
	{
		Object* _chars = (Object*)chars;
		std::vector<std::string> keys = _chars->GetKeys();
		for (size_t i = 0; i < keys.size(); i++)
		{
			JsonObject* jchar = (*_chars)[keys[i]];
			if (jchar->GetObjectType() == JsonObject::OBJECT)
			{
				Character* c = new Character(*(Object*)jchar);
				int id;
				sscanf(keys[i].c_str(), "%d", &id);
				mChars[id] = c;
			}
		}
	}

	if (face && (face->GetObjectType() == JsonObject::STRING))
		mFontFace = ((String*)face)->GetValue();

	if (type && (type->GetObjectType() == JsonObject::NUMBER))
	{
		Number* _type = (Number*)type;
		if (_type->GetType() == Number::INTEGER)
			mType = (FontType)_type->GetValue().integer_value;
	}

	if (size && (size->GetObjectType() == JsonObject::NUMBER))
	{
		Number* _size = (Number*)size;
		if (_size->GetType() == Number::INTEGER)
			mSize = (FontType)_size->GetValue().integer_value;
	}

	if (height && (height->GetObjectType() == JsonObject::NUMBER))
	{
		Number* _height = (Number*)height;
		if (_height->GetType() == Number::INTEGER)
			mHeight = (FontType)_height->GetValue().integer_value;
	}

	if (base && (base->GetObjectType() == JsonObject::NUMBER))
	{
		Number* _base = (Number*)base;
		if (_base->GetType() == Number::INTEGER)
			mBase = (FontType)_base->GetValue().integer_value;
	}

	if (kerning && (kerning->GetObjectType() == JsonObject::OBJECT))
	{
		Object* _kerning = (Object*)kerning;
		std::vector<std::string> keys = _kerning->GetKeys();
		for (size_t i = 0; i < keys.size(); i++)
		{
			long long index;
			const char* str = keys[i].c_str();
			sscanf(str, "%lld", &index);
			JsonObject* value = (*_kerning)[keys[i]];
			if (value->GetObjectType() != JsonObject::NUMBER)
				continue;
			Number* _value = (Number*)value;
			if (_value->GetType() != Number::INTEGER)
				continue;
			mKerning[index] = _value->GetValue().integer_value;
		}
	}

	return true;
}


std::pair<size_t, size_t> Font::measure(const std::string& text)
{
	return measure(stringToList(text));
}

std::pair<size_t, size_t> Font::measure(const std::list<int>& text)
{
	size_t xmax = 0, ymax = this->mHeight;
	int char_prev = -1;
	std::list<int>& _text = (std::list<int>&) text;

	for (std::list<int>::iterator i = _text.begin(); i != _text.end(); i++)
	{
		int char_index = *i;
		Character* character = loadCharacter(char_index);
		if (!character)
			continue;
		int kerning_offset = 0;
		if (char_prev >= 0)
		{
			long long index = (((long long)char_prev) << 32) | ((long long)char_index);
			kerning_offset = mKerning[index];
		}
		xmax += character->mPostPrintXOffset + kerning_offset;
		ymax = ____max(ymax, character->getHeight() + character->mPrePrintYOffset);
	}
	return std::pair<size_t, size_t>(xmax, ymax);
}

////////////////////////////////////////////////////////////////////////////////

IndexedFont::IndexedFont() : mCharFile(NULL)
{
}

IndexedFont::IndexedFont(Font& src)
{
	mChars = src.mChars;
	mKerning = src.mKerning;
	mType = src.mType;
	mFontFace = src.mFontFace;
	mSize = src.mSize;
	mBase = src.mBase;
	mHeight = src.mHeight;
	mForeground = src.mForeground;
	mCharFile = NULL;
	for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
		mIndex.add(i->first, 0, 0);
}

IndexedFont::IndexedFont(std::ifstream& charfile)
{
	mCharFile = &charfile;
}

IndexedFont::~IndexedFont()
{
}

std::string IndexedFont::serialize(std::ofstream& charfile)
{
	std::list<int> _chars = mIndex.list();
	std::map<int, int> chars;
	for (std::map<int, Character*>::iterator i = mChars.begin(); i != mChars.end(); i++)
		chars[i->first] = i->first;
	for (std::list<int>::iterator i = _chars.begin(); i != _chars.end(); i++)
		chars[*i] = *i;
	for (std::map<int, int>::iterator i = chars.begin(); i != chars.end(); i++)
	{
		Character* c = loadCharacter(i->first);
		if (c)
		{
			std::pair<size_t, size_t> bend = c->save(charfile);
			mIndex.add(i->first, bend.first, bend.second);
		}
	}
	charfile.flush();
	std::string font_str = serialize(true);
	Object* obj = new Object;
	obj->Parse(font_str, 0, font_str.size());
	obj->AddObject(__index, mIndex.serialize());
	font_str = obj->Serialize();
	delete obj;
	return font_str;
}

bool IndexedFont::deserialize(const std::string& src)
{
	Reset();
	Object::Parse(src, 0, src.size());
	if (!Font::parse(*this))
		return false;
	JsonObject* index = operator[](__index);
	if (!index || (index->GetObjectType() != OBJECT))
	{
		delete index;
		return false;
	}
	mIndex.deserialize(*(Object*)index);
	return true;
}

Character* IndexedFont::loadCharacter(const int id)
{
	std::map<int, Character*>::iterator ptr = mChars.find(id);
	if (ptr != mChars.end())
		return ptr->second;
	if (mCharFile && mIndex.contain(id))
	{
		Character* c = new Character;
		std::pair<size_t, size_t> bend = mIndex[id];
		c->load(*mCharFile, bend.first, bend.second);
		mChars[id] = c;
		return c;
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////

std::list<int> Componentality::Graphics::stringToList(const std::string& source)
{
	std::list<int> result;
	for (size_t i = 0; i < source.length(); i++)
		result.push_back(source[i]);
	return result;
}

std::string Componentality::Graphics::listToString(std::list<int>& source)
{
	std::string result;
	for (std::list<int>::iterator i = source.begin(); i != source.end(); i++)
		result += (char)*i;
	return result;
}
