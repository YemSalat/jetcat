#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "drawable.h"
#include "inputs.h"
#include "focus.h"
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class WindowManager;

			class Window : public IElement, public IMovable, public ISizeable
			{
			public:
				Window(WindowManager& owner);
				virtual ~Window();
				virtual bool isMovable() const { return true; }
			protected:
				virtual Margins draw(ISurface&);
			};

			class ModalWindow : public Window
			{
			protected:
				bool mGreying;
			public:
				ModalWindow(WindowManager& owner, const bool greying = false) : Window(owner), mGreying(greying) {};
				virtual ~ModalWindow() {};
			protected:
				virtual bool onEvent(CST::Common::Event&);
				virtual Margins draw(ISurface&);
				virtual void grey(const size_t x, const size_t y, Color& color);
			};

			class WindowManager : protected CST::Common::Dispatcher, 
				protected CST::Common::ThreadSet, 
				protected IElement,
				public IFocusController
			{
				friend class Window;
			protected:
				ISurface* mSurface;
				mutable CST::Common::mutex mLock;
			public:
				WindowManager();
				WindowManager(ISurface&);
				virtual ~WindowManager();
				virtual void sendEvent(CST::Common::Event& event) { CST::Common::Dispatcher::sendEvent(event); };
				virtual void draw();
				virtual void setSurface(ISurface&);
				virtual ISurface* getSurface() const;
				virtual void reset();
			protected:
				// Process message queue asynchronously
				static void __process(WindowManager* master);
				static void __timer(WindowManager* master);
			protected:
				virtual void dispose(CST::Common::Event* event) { if (event) delete event; }
				virtual Margins draw(ISurface& surface) { return IElement::draw(surface); };
				virtual bool ignoreMargins() const { return true; }
			protected:
				virtual void raise(CST::Common::Event& event) { sendEvent(event); }
				virtual bool onEvent(CST::Common::Event&);
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif