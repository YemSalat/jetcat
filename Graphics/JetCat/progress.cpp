#include "progress.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;


ProgressIndicator::ProgressIndicator(
	const IProgressIndicator::ProgressIndicatorType type,
	Componentality::Graphics::Color foreground,
	Componentality::Graphics::Color background,
	const Margins margins,
	Componentality::Graphics::Color internal_frame,
	Componentality::Graphics::Color external_frame) :
			mType(type)
			, mForegroundColor(foreground)
			, mBackgroundColor(background)
			, mMargins(margins)
			, mInternalFrameColor(internal_frame)
			, mExternalFrameColor(external_frame)

{
	mCurrentValue = 0;
	mMaxValue = 100;
}

Margins ProgressIndicator::draw(ISurface& surface)
{
	if (isHidden())
		return Margins();
	double& value = mCurrentValue;
	double& max_value = mMaxValue;
	Drawer drawer(surface);
	Margins margins = Margins(0, 0, 1, 1);
	if (!mExternalFrameColor.isUndefined())
	{
		Point topleft(0, 0);
		Point bottomright(surface.getWidth() - 1, surface.getHeight() - 1);
		drawer.rectangle(topleft, bottomright, mExternalFrameColor);
		margins += Margins(1, 1, 1, 1);
	}
	drawer.filled_rectangle(margins.getTopLeft(surface), margins.getRightBottom(surface), mBackgroundColor);
	margins += mMargins;
	if (!mInternalFrameColor.isUndefined())
	{
		drawer.rectangle(margins.getTopLeft(surface), margins.getRightBottom(surface), mInternalFrameColor);
		margins += Margins(1, 1, 1, 1);
	}
	double _value = value;
	if (_value > max_value)
		_value = max_value;
	if (surface.getWidth() <= margins.mLeft + margins.mRight)
		return Margins(0, 0, 0, 0);
	if (surface.getHeight() <= margins.mTop + margins.mBottom)
		return Margins(0, 0, 0, 0);
	if (mType == HORIZONTAL)
	{
		double width = surface.getWidth() - mMargins.mLeft - mMargins.mRight;
		width *= _value;
		width /= max_value;
		Point bottomright = margins.getTopLeft(surface);
		bottomright.y = margins.getRightBottom(surface).y;
		bottomright.x += (size_t) width;
		drawer.filled_rectangle(margins.getTopLeft(surface), bottomright, mForegroundColor);
	}
	if (mType == VERTICAL)
	{
		double height = surface.getHeight() - mMargins.mTop - mMargins.mBottom;
		height *= _value;
		height /= max_value;
		Point topleft = margins.getRightBottom(surface);
		topleft.y -= (size_t) height;
		topleft.x = margins.getTopLeft(surface).x;
		drawer.filled_rectangle(topleft, margins.getRightBottom(surface), mForegroundColor);
	}
	return Margins();
}

Margins CustomProgressIndicator::draw(ISurface& surface)
{
	if (mFiles.empty() || isHidden())
		return Margins();
	double scale = mMaxValue / mFiles.size();
	int position = (int) (mCurrentValue / scale);
	if ((size_t) position > mFiles.size() - 1)
		position = mFiles.size() - 1;
	if (mLoadedBitmapName != mFiles[position])
		mBitmap.read(mLoadedBitmapName = mFiles[position]);
	surface.apply(mBitmap);
	return Margins();
}
