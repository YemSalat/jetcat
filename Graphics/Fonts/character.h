#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "../Drawing/bitmask.h"
#include "../Surface/bmp_surface.h"
#include "../../../common-libs/libjson/Json.h"
#include "FIndex.h"

namespace Componentality
{
	namespace Graphics
	{
		class Font;

		class Character : public AlphaMask, public CST::Common::LibJSON::Object, public JSONLoadable
		{
			friend class Font;
		protected:
			int mPrePrintXOffset;
			int mPrePrintYOffset;
			int mPostPrintXOffset;
			int mPostPrintYOffset;
		public:
			Character();
			Character(CST::Common::LibJSON::Object& obj) { parse(obj); }
			virtual ~Character();
		public:
			virtual std::string serialize();
			virtual bool deserialize(const std::string&);
		public:
			int getFromFNT(const std::string&, ISurface&);
		public:
			virtual std::pair<size_t, size_t> Parse(const std::string&, const size_t, const size_t);
			virtual std::string Serialize(const CST::Common::LibJSON::OPTIONS options = CST::Common::LibJSON::OPTIONS());
			virtual bool parse(CST::Common::LibJSON::Object&);
			virtual std::pair<size_t, size_t> save(std::ofstream& file) { return JSONLoadable::save(file); };
			virtual bool load(std::ifstream& file, const size_t begin, const size_t end) { return JSONLoadable::load(file, begin, end); };
		protected:
			virtual CST::Common::LibJSON::Object& save();
			virtual bool load(CST::Common::LibJSON::Object& obj) { return parse(obj); }
		};

	} // namespace Graphics
} // namespace Componentality

#endif