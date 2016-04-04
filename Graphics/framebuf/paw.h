#include "../Surface/surface.h"
#include "../Drawing/sprite.h"
#include "framebuf.h"

namespace Componentality
{
	namespace Graphics
	{
	
		class Paw
		{
		protected:
			std::string mData;
		public:
			Paw();
			Paw(ISurface&);
			Paw(ISurface&, const size_t x, const size_t y, const size_t width, const size_t height);
			Paw(IFrameBuffer::Fragment);
			virtual ~Paw();
		public:
			virtual size_t getWidth() const;
			virtual size_t getHeight() const;
			virtual void capture(ISurface&, const size_t x, const size_t y, const size_t width, const size_t height);
			virtual void apply(ISurface&);
		protected:
			virtual operator IFrameBuffer::Fragment() const;
		};

	} // namespace Graphics
} // namespace Componentality
