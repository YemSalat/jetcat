#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "drawable.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class Cursor : public IElement, public Restorable
			{
			protected:
				CST::Common::mutex mLock;
				Point mLocation;
				size_t mWidth;
				size_t mHeight;
				bool   mShown;
				Color  mColor;
				size_t mPeriod;
				size_t mTimeSinceToggle;
			public:
				Cursor();
				Cursor(const Point);
				Cursor(const Point, const size_t width, const size_t height);
				Cursor(const Point, const size_t width, const size_t height, const Color);
				virtual ~Cursor();
				virtual Margins draw(ISurface&);
				virtual std::pair<size_t, size_t> getSize() const { return std::pair<size_t, size_t>(mWidth, mHeight); }
				virtual void setSize(const size_t, const size_t);
				virtual Point getLocation() const { return mLocation; };
				virtual void setLocation(const Point);
				virtual Color getColor() const { return mColor; }
				virtual void setColor(const Color color) { mColor = color; }
				virtual void setPeriod(const size_t period) { mPeriod = period; }
				virtual size_t getPeriod() const { return mPeriod; }
				virtual void reset();
			protected:
				virtual bool onEvent(CST::Common::Event&);
			};

		} // namespace JetCat
	}; // namespace Graphics
}; // namespace Componentality

#endif
