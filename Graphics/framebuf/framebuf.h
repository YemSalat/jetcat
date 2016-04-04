#ifndef __FRAMEBUF_H__
#define __FRAMEBUF_H__

#include "../Surface/surface.h"
#include "../Surface/bmp_surface.h"
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{

		class IFrameBuffer : public ISurface
		{
		public:
			struct Fragment
			{
				size_t x;
				size_t y;
				size_t width;
				size_t height;
				ISurface* surface;
			};
		protected:
			ISurface& mMasterSurface;
			size_t mXGranularity;
			size_t mYGranularity;
			bool* mChanged;
		public:
			IFrameBuffer(ISurface& master, const size_t x_granularity, const size_t y_granularity);
			virtual ~IFrameBuffer();
		public:
			bool& changed(const size_t x, const size_t y);
			virtual Fragment getSubSurface(const size_t x, const size_t y) = 0;
			virtual std::list<Fragment> getChanged() = 0;
			virtual void reset();
		public:
			// Set individual pixel's color
			virtual void plot(const size_t x, const size_t y, const Color&);
			// Get individual pixel's color
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const;
			// Get height
			virtual size_t getHeight() const;
		};

		class GenericFrameBuffer : public ISurface, public CST::Common::ThreadSet
		{
		protected:
			BitmapSurface* mCache;
			ISurface& mMasterSurface;
			bool mExited;
			bool mStopUpdate;
			bool mChanged;
			size_t mUpdatePeriod;
		public:
			GenericFrameBuffer(ISurface&, const size_t update_period = 20);
			virtual ~GenericFrameBuffer();
		public:
			// Set individual pixel's color
			virtual void plot(const size_t x, const size_t y, const Color&);
			// Get individual pixel's color
			virtual Color peek(const size_t x, const size_t y);
			// Get width
			virtual size_t getWidth() const;
			// Get height
			virtual size_t getHeight() const;
		protected:
			static void __process(GenericFrameBuffer*);
		};

		template <class MASTER>
		class CommonFrameBuffer : public GenericFrameBuffer
		{
		public:
			CommonFrameBuffer(const size_t update_period = 20) : GenericFrameBuffer(*new MASTER, update_period)	{}
			virtual ~CommonFrameBuffer()
			{
				exit();
				while (!mExited)
					CST::Common::sleep(10);
				delete &mMasterSurface;
			}

		};

	} // namespace Graphics
} // namespace Componentality

#endif
