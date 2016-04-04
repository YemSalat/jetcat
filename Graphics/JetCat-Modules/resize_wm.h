#ifndef __RESIZE_WM__
#define __RESIZE_WM__

#include "../JetCat/window.h"
#include "../Drawing/scale.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class ScalingWindowManager : public WindowManager
			{
			protected:
				double mXScaleFactor;
				double mYScaleFactor;
			public:
				ScalingWindowManager(ISurface& surface, const double xscale = 1.0, const double yscale = 1.0);
				virtual ~ScalingWindowManager();
			protected:
				virtual bool onEvent(CST::Common::Event&);
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality


#endif // ! __RESIZE_WM__
