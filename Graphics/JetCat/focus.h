#ifndef __FOCUS_H__
#define __FOCUS_H__

#include "drawable.h"
#include "inputs.h"
#include "../../../common-libs/common/common_utilities.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class IFocusController;

			class IFocusable : public CST::Common::Subscriber
			{
				friend class IFocusController;
			protected:
				IFocusController& mController;
			public:
				IFocusable(IFocusController& controller) : mController(controller) {};
				virtual ~IFocusable() {};
			protected:
				virtual bool onSetFocus() = 0;
				virtual bool onReleaseFocus() = 0;
			public:
				virtual bool requestFocus();
				virtual bool releaseFocus();
			};

			class IFocusController
			{
			protected:
				CST::Common::mutex mLock;
				IFocusable* mFocusHolder;
			public:
				IFocusController() : mFocusHolder(NULL) {};
				virtual ~IFocusController() {};
			public:
				virtual bool requestFocus(IFocusable&);
				virtual bool releaseFocus(IFocusable&);
			protected:
				bool sendEventToFocusHolder(CST::Common::Event& event) { if (mFocusHolder) return mFocusHolder->onEvent(event); else return false; }
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif