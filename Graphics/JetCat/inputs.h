#ifndef __INPUTS_H__
#define __INPUTS_H__

#include "drawable.h"
#include "../Surface/easy_draw.h"
#include "../../../common-libs/common/common_events.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			extern const std::string EVENT_TYPE_POINTER;			// Mouse move event
			extern const std::string EVENT_TYPE_PRESS;				// Mouse left button down or touch screen press
			extern const std::string EVENT_TYPE_DEPRESS;			// Mouse left button up or touch screen depress
			extern const std::string EVENT_TYPE_DRAW_REQUEST;		// Request to re-draw the subtree or all the desktop
			extern const std::string EVENT_TYPE_TIMER;				// 100ms timer event
			extern const std::string EVENT_TYPE_MENU;				// Menu item selected
			extern const std::string EVENT_TYPE_KEY;				// Keyboard event
			extern const std::string EVENT_TYPE_SET_DEFAULT;		// Reset graphic caches and other parameters to default

			class IInputEvent : public CST::Common::Event
			{
			protected:
				bool mNotToModal;										// true if message shall not be sent to non-modal windows if modal is present
				bool mNotToHidden;										// true if message shall not be sent to hidden objects also
			protected:
				IInputEvent(const std::string type) : CST::Common::Event(type), mNotToModal(true), mNotToHidden(true) {};
				IInputEvent() : mNotToModal(true), mNotToHidden(true) {};
			public:
				virtual ~IInputEvent() { };
				virtual std::string getType() const { return mType; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) = 0;
				bool isNotToModal() const { return mNotToModal; }
				bool isNotToHidden() const { return mNotToHidden; }
			protected:
				virtual void setPayload(Payload& payload) { if (mPayload) delete mPayload; mPayload = &payload; }
				virtual Payload* getPayload() const { return mPayload; }
			};

			class PointerEvent : public IInputEvent
			{
			protected:
				PointerEvent(std::string type) : IInputEvent(type) { };
			public:
				class Point : public Componentality::Graphics::Point, public CST::Common::Event::Payload
				{
				public:
					Point(const size_t _x = 0, const size_t _y = 0) : Componentality::Graphics::Point(_x, _y) {};
					virtual ~Point() {};
				};
			public:
				PointerEvent() : IInputEvent(EVENT_TYPE_POINTER) { mPayload = new Point; };
				PointerEvent(const Componentality::Graphics::Point point) : IInputEvent(EVENT_TYPE_POINTER) { mPayload = new Point; set(point); };
				PointerEvent(const size_t x, const size_t y) : IInputEvent(EVENT_TYPE_POINTER)
					{ mPayload = new Point; ((Point*)mPayload)->x = x; ((Point*)mPayload)->y = y; };
				virtual ~PointerEvent() { if (mPayload) delete (Point*)mPayload; mPayload = NULL; };
				virtual Componentality::Graphics::Point get() const { return *(Point*)mPayload; }
				virtual void set(const Componentality::Graphics::Point point) { ((Point*)mPayload)->x = point.x; ((Point*)mPayload)->y = point.y; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom);
			};

			class PressEvent : public PointerEvent
			{
			public:
				PressEvent() : PointerEvent(EVENT_TYPE_PRESS) { mPayload = new Point; };
				PressEvent(const Componentality::Graphics::Point point) : PointerEvent(point) { mType = EVENT_TYPE_PRESS; };
				PressEvent(const size_t x, const size_t y) : PointerEvent(x, y) { mType = EVENT_TYPE_PRESS; };
				virtual ~PressEvent() { };
			};

			class DepressEvent : public PointerEvent
			{
			public:
				DepressEvent() : PointerEvent(EVENT_TYPE_DEPRESS) { mPayload = new Point; };
				DepressEvent(const Componentality::Graphics::Point point) : PointerEvent(point) { mType = EVENT_TYPE_DEPRESS; };
				DepressEvent(const size_t x, const size_t y) : PointerEvent(x, y) { mType = EVENT_TYPE_DEPRESS; };
				virtual ~DepressEvent() { };
			};

			class DrawRequestEvent : public IInputEvent
			{
			public:
				struct Initiator : public CST::Common::Event::Payload
				{
					Initiator(IElement* initiator, const bool all = false) : mInitiator(initiator), mUpdateAll(all) {}
					IElement* mInitiator;
					bool mUpdateAll;			// True means a request to update all the objects tree
				};
			protected:
				DrawRequestEvent(std::string type) : IInputEvent(type) { };
			public:
				DrawRequestEvent(IElement* initiator) : IInputEvent(EVENT_TYPE_DRAW_REQUEST) { setPayload(*new Initiator(initiator)); };
				DrawRequestEvent() : IInputEvent(EVENT_TYPE_DRAW_REQUEST) { setPayload(*new Initiator(NULL, true)); };
				virtual ~DrawRequestEvent() { if (mPayload) delete (Initiator*)mPayload; mPayload = NULL; };
				virtual Initiator* get() const { if (mPayload) return (Initiator*)mPayload; else return NULL; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) { return true; };
			};

			class TimerEvent : public IInputEvent
			{
			public:
				struct Counter : public CST::Common::Event::Payload
				{
					size_t mCounter;
					Counter(const size_t value) { mCounter = 0; }
				};
			protected:
				static Counter mCounter;
			protected:
				TimerEvent(std::string type) : IInputEvent(type) { mNotToHidden = mNotToModal = false; setPayload(*new Counter(0)); };
			public:
				TimerEvent() : IInputEvent(EVENT_TYPE_TIMER) { mNotToHidden = mNotToModal = false; setPayload(*new Counter(0)); };
				virtual ~TimerEvent() { if (mPayload) delete mPayload; mPayload = NULL; };
				virtual size_t getCounter() const { if (mPayload) return ((Counter*)mPayload)->mCounter; else return 0; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) { return true; };
				static size_t getGlobalCounter() { return mCounter.mCounter; }
				static void appendGlobalCounter() { mCounter.mCounter += 1; }
			};

			class MenuEvent : public IInputEvent
			{
			protected:
				struct Payload : public CST::Common::Event::Payload
				{
					IElement* mMenuItem;
					Payload(IElement* item) : mMenuItem(item) {}
				};
			public:
				MenuEvent(IElement* item) : IInputEvent(EVENT_TYPE_MENU) { setPayload(*new Payload(item)); }
				virtual ~MenuEvent() { if (mPayload) delete mPayload; mPayload = NULL; };
				virtual IElement* getItem() const { if (mPayload) return ((Payload*)mPayload)->mMenuItem; else return NULL; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) { return true; };
			};

			class KeyEvent : public IInputEvent
			{
			public:
				enum KEY_TYPE {
					ALPHABETH,
					SPECIAL
				};
				enum EVENT_TYPE
				{
					DOWN,
					UP
				};
			protected:
				struct Payload : public CST::Common::Event::Payload
				{
					int mKeyCode;										// ABC key or special key code
					KEY_TYPE mKeyType;									// Key type
					EVENT_TYPE mEventType;								// Event type
					IElement* mOriginator;								// Message sender
					Payload(IElement* sender, const KEY_TYPE ktype, const EVENT_TYPE etype, const int code) : 
						mOriginator(sender), mKeyCode(code), mKeyType(ktype), mEventType(etype) {}
				};
			public:
				KeyEvent(IElement* item, const KEY_TYPE kt, const EVENT_TYPE et, const int code) : IInputEvent(EVENT_TYPE_KEY) 
					{ setPayload(*new Payload(item, kt, et, code)); }
				virtual ~KeyEvent() { if (mPayload) delete mPayload; mPayload = NULL; };
				virtual IElement* getOriginator() const { if (mPayload) return ((Payload*)mPayload)->mOriginator; else return NULL; }
				virtual int getKeyCode() const { return ((Payload*)mPayload)->mKeyCode; }
				virtual KEY_TYPE getKeyType() const { return ((Payload*)mPayload)->mKeyType; }
				virtual EVENT_TYPE getEventType() const { return ((Payload*)mPayload)->mEventType; }
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) { return true; };
			public:
				enum VKEYS
				{
					VKEY_LEFT		= -1000,
					VKEY_RIGHT		= -1001,
					VKEY_UP			= -1002,
					VKEY_DOWN		= -1003,
					VKEY_BACKSPACE	= -1004,
					VKEY_DELETE		= -1005,
					VKEY_ENTER		= -1006,
					VKEY_ESCAPE		= -1007,
					VKEY_TAB		= -1008
				};
			};

			class SetDefaultEvent : public IInputEvent
			{
			protected:
				SetDefaultEvent(const std::string type) : IInputEvent(type) {};
			public:
				SetDefaultEvent() : IInputEvent(EVENT_TYPE_SET_DEFAULT) { this->mNotToHidden = true; };
				virtual ~SetDefaultEvent() {};
				virtual bool filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom) { return true; };
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif