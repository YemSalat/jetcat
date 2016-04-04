#include "FIndex.h"
#include "../../../common-libs/common/common_utilities.h"

using namespace Componentality::Graphics;
using namespace CST::Common;
using namespace CST::Common::LibJSON;

static const std::string ___loadtable		= "table";
static const std::string ___loadtable_id	= "id";
static const std::string ___loadtable_begin = "begin";
static const std::string ___loadtable_end	= "end";

CST::Common::LibJSON::Object* LoadTable::serialize()
{
	Object* result = new Object;
	Array* array = new Array;
	result->AddObject(___loadtable, array);
	for (std::map<int, std::pair<size_t, size_t> >::iterator i = mTable.begin(); i != mTable.end(); i++)
	{
		Number* id = new Number((long)i->first);
		Number* begin = new Number((long)i->second.first);
		Number* end = new Number((long)i->second.second);
		Object* item = new Object;
		item->AddObject(___loadtable_id, id);
		item->AddObject(___loadtable_begin, begin);
		item->AddObject(___loadtable_end, end);
		array->Push(item);
	}
	return result;
}

void LoadTable::deserialize(CST::Common::LibJSON::Object& obj)
{
	JsonObject* array = obj[___loadtable];
	if (!array || (array->GetObjectType() != JsonObject::ARRAY))
		return;
	for (size_t i = 0; i < ((Array*)array)->Size(); i++)
	{
		JsonObject* item = (*(Array*)array)[i];
		if (item && (item->GetObjectType() == JsonObject::OBJECT))
		{
			JsonObject* id = (*(Object*)item)[___loadtable_id];
			if (!id || (id->GetObjectType() != JsonObject::NUMBER) || (((Number*)id)->GetType() != Number::INTEGER))
				continue;
			JsonObject* begin = (*(Object*)item)[___loadtable_begin];
			if (!begin || (begin->GetObjectType() != JsonObject::NUMBER) || (((Number*)begin)->GetType() != Number::INTEGER))
				continue;
			JsonObject* end = (*(Object*)item)[___loadtable_end];
			if (!end || (end->GetObjectType() != JsonObject::NUMBER) || (((Number*)end)->GetType() != Number::INTEGER))
				continue;
			std::pair<size_t, size_t> bend((size_t)((Number*)begin)->GetValue().integer_value, (size_t)((Number*)end)->GetValue().integer_value);
			mTable[(int)((Number*)id)->GetValue().integer_value] = bend;
		}
	}
}

std::list<int> LoadTable::list()
{
	std::list<int> result;
	for (std::map<int, std::pair<size_t, size_t> >::iterator i = mTable.begin(); i != mTable.end(); i++)
		result.push_back(i->first);
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////

std::pair<size_t, size_t> JSONLoadable::save(std::ofstream& file)
{
	Object& obj = save();
	std::pair<size_t, size_t> result((size_t)file.tellp(), (size_t)file.tellp());
	std::string tosave = obj.Serialize();
	file.write(tosave.c_str(), tosave.size());
	result.second = (size_t) file.tellp();
	return result;
}

bool JSONLoadable::load(std::ifstream& file, const size_t begin, const size_t end)
{
	if (end <= begin)
		return false;
	file.seekg(begin);
	CST::Common::blob read(end - begin);
	file.read(read.mData, read.mSize);
	Object* obj = new Object;
	obj->Parse(CST::Common::blobToString(read), 0, read.mSize);
	read.purge();
	bool result = load(*obj);
	delete obj;
	return result;
}
