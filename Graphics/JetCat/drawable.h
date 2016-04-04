#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "evtfilter.h"
#include "../Surface/surface.h"
#include "../Surface/bmp_surface.h"
#include "../Drawing/viewport.h"
#include "../../../common-libs/common/common_events.h"
#include <string>
#include <list>
#include <map>

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			struct Margins
			{
				size_t mLeft;
				size_t mTop;
				size_t mRight;
				size_t mBottom;
			public:
				Margins() : mLeft(0), mTop(0), mRight(0), mBottom(0) {} ; 
				Margins(const size_t left, const size_t top, const size_t right, const size_t bottom) :
					mLeft(left), mTop(top), mRight(right), mBottom(bottom) {};
				Margins& operator+=(const Margins& source)
				{
					mLeft += source.mLeft;
					mTop += source.mTop;
					mRight += source.mRight;
					mBottom += source.mBottom;
					return *this;
				}
				Point getTopLeft(ISurface& surface)
				{
					return Point(mLeft, mTop);
				}
				Point getRightBottom(ISurface& surface)
				{
					return Point(surface.getWidth() - mRight, surface.getHeight() - mBottom);
				}
			};

			class IArea
			{
			protected:
				Componentality::Graphics::Point mTopLeft;
				Componentality::Graphics::Point mRightBottom;
				int zOrder;
			public:
				IArea() { zOrder = 0; };
				IArea(const Componentality::Graphics::Point topleft, const Componentality::Graphics::Point rightbottom)
					: mTopLeft(topleft), mRightBottom(rightbottom) { zOrder = 0; };
				virtual ~IArea() {};
			public:
				virtual void setTopLeft(const Componentality::Graphics::Point topleft) { mTopLeft = topleft; }
				virtual void setRightBottom(const Componentality::Graphics::Point rightbottom) { mRightBottom = rightbottom; }
				virtual Componentality::Graphics::Point getTopLeft() const { return mTopLeft; }
				virtual Componentality::Graphics::Point getRightBottom() const { return mRightBottom; }
				virtual void setZOrder(const int zorder) { zOrder = zorder; }
				virtual int getZOrder() const { return zOrder; }
			};

			class IEventHandler : public CST::Common::Subscriber
			{
			public:
				IEventHandler() {}
				virtual ~IEventHandler() {}
				virtual bool send(CST::Common::Event& evt) { return onEvent(evt); }
			protected:
				virtual bool onEvent(CST::Common::Event&) = 0;
				virtual bool filter(CST::Common::Event&) = 0;
			};

			class IDrawable : public IArea, public IEventHandler
			{
				friend class IElement;
			protected:
				bool		mHidden;
			public:
				IDrawable() : mHidden(false) {};
				virtual ~IDrawable() {};
				virtual Margins draw(ISurface&) = 0;
			public:
				virtual bool isVisible() const { return !mHidden; }
				virtual bool isHidden() const { return mHidden; }
				virtual void hide() { mHidden = true; }
				virtual void show() { mHidden = false; }
				virtual bool filter(CST::Common::Event& event);
			};

			typedef std::string Tag;

			typedef std::map<Tag, IDrawable*> DrawingList;

			typedef std::list<Tag> DrawOrder;

			class IElement : public IDrawable
			{
			protected:
				DrawingList mSubItems;
				DrawOrder   mOrder;
				IElement*   mOwner;
			protected:
				IEventFilter* mEvtFilter;
			protected:
				IElement(IElement&) {};
			public:
				IElement() : mOwner(NULL), mEvtFilter(NULL) {};
				virtual ~IElement();
				virtual Margins draw(ISurface&);
				virtual void add(const Tag, IElement&);
				virtual void remove(const Tag);
				virtual void replace(const Tag, IElement&);
				virtual std::list<Tag> list() const;
				virtual DrawOrder getOrder() const { return mOrder; }
				virtual void setOrder(const DrawOrder& order) { mOrder = order; }
				virtual IDrawable* get(const Tag tag);
				virtual bool isHidden() const;
				virtual bool isMovable() const { return false; }
				virtual IElement* getOwner() const { return mOwner; }
			protected:
				bool isInOrder(Tag) const;
				virtual bool ignoreMargins() const { return false; }
			public:
				static Componentality::Graphics::ViewPort getDrawArea(ISurface&, const Margins&);
			protected:
				virtual bool onEvent(CST::Common::Event&);
				virtual bool filter(CST::Common::Event&);
			protected:
				virtual void raise(CST::Common::Event& event) { if (mOwner) mOwner->raise(event); }
			public:
				virtual void setEventFilter(IEventFilter* filter) { mEvtFilter = filter; }
				virtual IEventFilter* getEventFilter();
			};

			class IMovable
			{
			protected:
				Componentality::Graphics::Point mPosition;
			public:
				IMovable() {}
				virtual ~IMovable() {};
				virtual void move(const size_t x, const size_t y) { mPosition.x = x; mPosition.y = y; }
				virtual Componentality::Graphics::Point getPosition() const { return mPosition; }
			};

			class ISizeable
			{
			protected:
				size_t mWidth;
				size_t mHeight;
			public:
				ISizeable() {};
				virtual ~ISizeable() {};
				virtual void size(const size_t width, const size_t height) { mWidth = width; mHeight = height; }
				virtual std::pair<size_t, size_t> getSize() const { return std::pair<size_t, size_t>(mWidth, mHeight); }
			};

			class IRestorable
			{
			protected:
				virtual void backup(ISurface&) = 0;
				virtual void restore(ISurface&) = 0;
			public:
				IRestorable() {};
				virtual ~IRestorable() {};
				virtual void reset() = 0;
			};

			class Restorable : public IRestorable
			{
			public:
				BitmapSurface* mStorage;
			protected:
				virtual void backup(ISurface&);
				virtual void restore(ISurface&);
			public:
				Restorable() : mStorage(NULL) {};
				virtual ~Restorable() { if (mStorage) delete mStorage; };
				virtual void reset() { if (mStorage) delete mStorage; mStorage = NULL; };
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif