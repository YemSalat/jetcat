#ifndef __IMAGEBOX_H__
#define __IMAGEBOX_H__

#include "global.h"
#include "drawable.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class Image : public IElement
			{
			protected:
				std::string mFilename;
			public:
				Image();
				Image(const std::string file) :
					mFilename(file) {};
				virtual ~Image();
				virtual Margins draw(ISurface&);
				virtual std::string getFilename() const { return mFilename; }
				virtual void setFilename(const std::string file) { mFilename = file; }
			};
		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif