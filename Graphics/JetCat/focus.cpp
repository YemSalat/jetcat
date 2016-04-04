#include "focus.h"

using namespace Componentality::Graphics::JetCat;

bool IFocusable::requestFocus() 
{ 
	return mController.requestFocus(*this); 
};

bool IFocusable::releaseFocus() 
{ 
	return mController.releaseFocus(*this); 
};

////////////////////////////////////////////////////////////////////////////////

bool IFocusController::requestFocus(IFocusable& item)
{
	CST::Common::scoped_lock lock(mLock);
	if (&item == mFocusHolder)
		return true;
	if (mFocusHolder && (!mFocusHolder->onReleaseFocus()))
		return false;
	else
	{
		bool result = item.onSetFocus();
		if (result)
			mFocusHolder = &item;
		return result;
	}
}

bool IFocusController::releaseFocus(IFocusable& item)
{
	CST::Common::scoped_lock lock(mLock);
	if (!item.onReleaseFocus())
		return false;
	if (&item == mFocusHolder)
		mFocusHolder = &item;
	return true;
}
