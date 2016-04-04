#ifndef __DIFFSRFC_H__
#define __DIFFSRFC_H__

#include "../Surface/surface.h"

namespace Componentality
{
	namespace Graphics
	{
		
		void operator-=(ISurface&, ISurface&);
		void operator+=(ISurface&, ISurface&);

	} // namespace Graphics
} // namespace Componentality

#endif