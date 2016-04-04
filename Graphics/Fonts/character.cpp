#include "character.h"
#include "../../../common-libs/Base64/Base64.h"
#include "../../../common-libs/common/common_utilities.h"
#include "../../../common-libs/Scanback/sbapi.h"
#include <stdlib.h>

using namespace Componentality::Graphics;
using namespace CST::Common;
using namespace CST::Common::LibJSON;

CST::Common::blob ____initblob(CST::Common::blob blob);

// JSON keywords
static std::string __xpreoffs  = "xpre";
static std::string __ypreoffs  = "ypre";
static std::string __xpostoffs = "xpost";
static std::string __ypostoffs = "ypost";
static std::string __content   = "char";
static std::string __width     = "width";
static std::string __height    = "height";
static std::string __compressed = "pchar";
static std::string __compressed_size = "psize";

// FNT keywords
static std::string fnt__index  = "char id";
static std::string fnt__xcoord = "x";
static std::string fnt__ycoord = "y";
static std::string fnt__width  = "width";
static std::string fnt__height = "height"; 
static std::string fnt__xpreoffs = "xoffset";
static std::string fnt__ypreoffs = "yoffset";
static std::string fnt__xpostoffs = "xadvance";

Character::Character() : AlphaMask(1, 1)
{
	mPrePrintXOffset  = 0;
	mPrePrintYOffset  = 0;
	mPostPrintXOffset = 0;
	mPostPrintYOffset = 0;
}

Character::~Character()
{
}

std::string Character::serialize()
{
	Reset();
	Number xpreoffs  = (long int) mPrePrintXOffset;
	Number ypreoffs  = (long int) mPrePrintYOffset;
	Number xpostoffs = (long int) mPostPrintXOffset;
	Number ypostoffs = (long int) mPostPrintYOffset;
	Number width	 = (long int) mWidth;
	Number height	 = (long int) mHeight;
	std::string __content__;
	CST::Common::blob memory = getMemory();
	__content__.assign(memory.mData, memory.mSize);
#ifdef FONTS_USE_SBC_COMPRESSION
	Number size = (long int) __content__.size();
	Componentality::Common::RLESBCompressor compressor;
	String content = BASE64_encode(CST::Common::blobToString(compressor(CST::Common::stringToBlob(__content__))));
	AddObject(__compressed_size, &size);
	AddObject(__compressed, &content);
#else
	String content = BASE64_encode(__content__);
	AddObject(__content, &content);
#endif
	AddObject(__xpreoffs, &xpreoffs);
	AddObject(__ypreoffs, &ypreoffs);
	AddObject(__xpostoffs, &xpostoffs);
	AddObject(__ypostoffs, &ypostoffs);
	AddObject(__width, &width);
	AddObject(__height, &height);

	std::string result = Object::Serialize();

	RemoveObject(__compressed_size);
	RemoveObject(__compressed);
	RemoveObject(__content);
	RemoveObject(__xpreoffs);
	RemoveObject(__ypreoffs);
	RemoveObject(__xpostoffs);
	RemoveObject(__ypostoffs);
	RemoveObject(__width);
	RemoveObject(__height);
	return result;
}

bool Character::deserialize(const std::string& source)
{
	Reset();
	std::pair<size_t, size_t> result = Object::Parse(source, 0, source.length());
	bool _result = (result != Parseable::ERROR_VAL);

	if (!_result)
		return false;

	bool __result = parse(*this);
	Reset();
	return __result;
}

std::string ____getFNTArgument(const std::string& src, const std::string& arg)
{
	size_t pos = src.find(arg);
	size_t i;
	if (pos == (size_t)-1)
		return "";
	for (i = pos + arg.size(); i < src.size(); i++)
	{
		if (src[i] == '=')
		{
			break;
		}
	}
	i += 1;
	if (i >= src.size())
		return "";
	for (; i < src.size(); i++)
		if (src[i] > ' ')
			break;
	if (i >= src.size())
		return "";
	std::string result;
	for (; i < src.size(); i++)
	{
		if (src[i] > ' ')
			result += src[i];
		else
			break;
	}
	return result;
}

#define PARSEATTR(attr)		\
	if (attr.empty())		\
		return -1;			\
	int __##attr;			\
	sscanf(attr.c_str(), "%d", &__##attr);


int Character::getFromFNT(const std::string& src, ISurface& texture)
{
	Reset();
	getMemory().purge();
	mPrePrintXOffset = 0;
	mPrePrintYOffset = 0;
	mPostPrintXOffset = 0;
	mPostPrintYOffset = 0;
	mWidth = mHeight = 0;

	std::string charid = ____getFNTArgument(src, fnt__index);
	std::string xpos = ____getFNTArgument(src, fnt__xcoord);
	std::string ypos = ____getFNTArgument(src, fnt__ycoord);
	std::string width = ____getFNTArgument(src, fnt__width);
	std::string height = ____getFNTArgument(src, fnt__height);
	std::string xpreoffs = ____getFNTArgument(src, fnt__xpreoffs);
	std::string ypreoffs = ____getFNTArgument(src, fnt__ypreoffs);
	std::string xpostoffs = ____getFNTArgument(src, fnt__xpostoffs);

	PARSEATTR(charid);
	PARSEATTR(xpos);
	PARSEATTR(ypos);
	PARSEATTR(width);
	PARSEATTR(height);
	PARSEATTR(xpreoffs);
	PARSEATTR(ypreoffs);
	PARSEATTR(xpostoffs);

	mWidth = __width;
	mHeight = __height;
	mPrePrintXOffset = __xpreoffs;
	mPrePrintYOffset = __ypreoffs;
	mPostPrintXOffset = __xpostoffs;

	mAlphamem = ____initblob(CST::Common::blob(mWidth * mHeight));

	for (size_t i = 0; i < mWidth; i++)
	{
		for (size_t j = 0; j < mHeight; j++)
		{
			ColorRGB color = texture.peek(__xpos + i, __ypos + j);
			if (color.blue || color.green || color.red)
			{
				unsigned int alpha = color.blue + color.green + color.red;
				alpha /= 3;
				setAlpha(i, j, (unsigned char)alpha);
			}
		}
	}

	return __charid;
};

std::pair<size_t, size_t> Character::Parse(const std::string& src, const size_t begin, const size_t end)
{
	std::string _src = src.substr(begin, end - begin);
	if (!deserialize(_src))
		return Parseable::ERROR_VAL;
	else
		return std::pair<size_t, size_t>(begin, end);
}

std::string Character::Serialize(const OPTIONS options)
{
	return serialize();
}

bool Character::parse(CST::Common::LibJSON::Object& obj)
{
	JsonObject* compressed = obj.operator[](__compressed);
	JsonObject* size = obj.operator[](__compressed_size);
	JsonObject* content = obj.operator[](__content);
	JsonObject* xpreoffs = obj.operator[](__xpreoffs);
	JsonObject* ypreoffs = obj.operator[](__ypreoffs);
	JsonObject* xpostoffs = obj.operator[](__xpostoffs);
	JsonObject* ypostoffs = obj.operator[](__ypostoffs);
	JsonObject* width = obj.operator[](__width);
	JsonObject* height = obj.operator[](__height);

	if (compressed && (compressed->GetObjectType() == STRING) && size && (size->GetObjectType() == NUMBER) &&
		((Number*)size)->GetType() == Number::INTEGER)
	{
		getMemory().purge();
		Componentality::Common::RLESBDecompressor decompressor(((Number*)size)->GetValue().integer_value);
		std::string _content = BASE64_decode(((String*)compressed)->GetValue());
		_content = CST::Common::blobToString(decompressor(CST::Common::stringToBlob(_content)));
		if (_content.empty())
			return false;
		CST::Common::blob data(_content.length());
		memcpy(data.mData, _content.c_str(), _content.size());
		mAlphamem = data;
	}
	else if (content && (content->GetObjectType() == STRING))
	{
		getMemory().purge();
		std::string _content = BASE64_decode(((String*)content)->GetValue());
		if (_content.empty())
			return false;
		CST::Common::blob data(_content.length());
		memcpy(data.mData, _content.c_str(), _content.size());
		mAlphamem = data;
	}

	if (xpreoffs && (xpreoffs->GetObjectType() == NUMBER) && (((Number*)xpreoffs)->GetType() == Number::INTEGER))
		mPrePrintXOffset = ((Number*)xpreoffs)->GetValue().integer_value;

	if (ypreoffs && (ypreoffs->GetObjectType() == NUMBER) && (((Number*)ypreoffs)->GetType() == Number::INTEGER))
		mPrePrintYOffset = ((Number*)ypreoffs)->GetValue().integer_value;

	if (xpostoffs && (xpostoffs->GetObjectType() == NUMBER) && (((Number*)xpostoffs)->GetType() == Number::INTEGER))
		mPostPrintXOffset = ((Number*)xpostoffs)->GetValue().integer_value;

	if (ypostoffs && (ypostoffs->GetObjectType() == NUMBER) && (((Number*)ypostoffs)->GetType() == Number::INTEGER))
		mPostPrintYOffset = ((Number*)ypostoffs)->GetValue().integer_value;

	if (width && (width->GetObjectType() == NUMBER) && (((Number*)width)->GetType() == Number::INTEGER))
		mWidth = ((Number*)width)->GetValue().integer_value;

	if (height && (height->GetObjectType() == NUMBER) && (((Number*)height)->GetType() == Number::INTEGER))
		mHeight = ((Number*)height)->GetValue().integer_value;

	return true;
}

CST::Common::LibJSON::Object& Character::save()
{
	CST::Common::LibJSON::Object* object = new Object;
	Number& xpreoffs = *new Number((long int)mPrePrintXOffset);
	Number& ypreoffs = *new Number((long int)mPrePrintYOffset);
	Number& xpostoffs = *new Number((long int)mPostPrintXOffset);
	Number& ypostoffs = *new Number((long int)mPostPrintYOffset);
	Number& width = *new Number((long int)mWidth);
	Number& height = *new Number((long int)mHeight);
	std::string __content__;
	CST::Common::blob memory = getMemory();
	__content__.assign(memory.mData, memory.mSize);
#ifdef FONTS_USE_SBC_COMPRESSION
	Number& size = *new Number((long int)__content__.size());
	Componentality::Common::RLESBCompressor compressor;
	String& content = *new String(BASE64_encode(CST::Common::blobToString(compressor(CST::Common::stringToBlob(__content__)))));
	object->AddObject(__compressed_size, &size);
	object->AddObject(__compressed, &content);
#else
	String& content = *new String(BASE64_encode(__content__));
	object->AddObject(__content, &content);
#endif
	object->AddObject(__xpreoffs, &xpreoffs);
	object->AddObject(__ypreoffs, &ypreoffs);
	object->AddObject(__xpostoffs, &xpostoffs);
	object->AddObject(__ypostoffs, &ypostoffs);
	object->AddObject(__width, &width);
	object->AddObject(__height, &height);

	return *object;
}
