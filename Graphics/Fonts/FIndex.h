#ifndef __FINDEX_H__
#define __FINDEX_H__

#include <fstream>
#include <map>
#include <list>
#include "../../../common-libs/libjson/Json.h"

namespace Componentality
{
	namespace Graphics
	{

		class ILoadable
		{
		public:
			ILoadable() {};
			virtual ~ILoadable() {};
		public:
			virtual std::pair<size_t, size_t> save(std::ofstream&) = 0;
			virtual bool load(std::ifstream&, const size_t begin, const size_t end) = 0;
		};

		class LoadTable
		{
		protected:
			std::map<int, std::pair<size_t, size_t> > mTable;
		public:
			LoadTable() {};
			virtual ~LoadTable() {};
		public:
			virtual CST::Common::LibJSON::Object* serialize();
			virtual void deserialize(CST::Common::LibJSON::Object&);
			virtual void add(const int id, const size_t begin, const size_t end) { mTable[id] = std::pair<size_t, size_t>(begin, end); }
			virtual std::pair<size_t, size_t> operator[](const int id) { return mTable[id]; }
			virtual void reset() { mTable.clear(); }
			virtual bool contain(const int id) { return mTable.find(id) != mTable.end(); }
			virtual std::list<int> list();
		};

		class JSONLoadable : public ILoadable
		{
		public:
			JSONLoadable() {};
			virtual ~JSONLoadable() {};
		public:
			virtual std::pair<size_t, size_t> save(std::ofstream&);
			virtual bool load(std::ifstream&, const size_t begin, const size_t end);
		protected:
			virtual CST::Common::LibJSON::Object& save() = 0;
			virtual bool load(CST::Common::LibJSON::Object&) = 0;
		};

	} // namespace Graphics
} // namespace Componentality

#endif
