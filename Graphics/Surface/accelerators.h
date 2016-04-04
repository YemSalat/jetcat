#ifndef __ACCELERATORS_H__
#define __ACCELERATORS_H__

namespace Componentality
{

	namespace Graphics
	{
		enum ACCELERATORS
		{
			ACCELERATOR_DRAW_PIXEL		= 0x00,
			ACCELERATOR_FILL_RECTANGLE  = 0x01,
			ACCELERATOR_HORIZONTAL_LINE = 0x02,

			ACCELERATOR_MAX = 0x03
		};

		class Accelerator
		{
		public:
			Accelerator() {};
			virtual ~Accelerator() {};
		};

	} // namespace Graphics

} // namespace Componentality

#endif
