#include "global.h"
#include "drawable.h"
#include <vector>

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			class IProgressIndicator
			{
			public:
				enum ProgressIndicatorType
				{
					VERTICAL,
					HORIZONTAL
				};
			public:
				IProgressIndicator() {};
				virtual ~IProgressIndicator() {};
				virtual void indicate(const double value, const double max_value = 100) = 0;
			};

			class ProgressIndicator : public IProgressIndicator, public IElement
			{
			protected:
				ProgressIndicatorType mType;
				Componentality::Graphics::Color mForegroundColor;
				Componentality::Graphics::Color mBackgroundColor;
				Componentality::Graphics::Color mInternalFrameColor;
				Componentality::Graphics::Color mExternalFrameColor;
				Margins mMargins;
				double mCurrentValue;									// Value to be indicated
				double mMaxValue;										// Maximum of the scale
			public:
				ProgressIndicator(
					const IProgressIndicator::ProgressIndicatorType type,
					Componentality::Graphics::Color foreground,
					Componentality::Graphics::Color background,
					const Margins margins,
					Componentality::Graphics::Color internal_frame = Componentality::Graphics::Color(),
					Componentality::Graphics::Color external_frame = Componentality::Graphics::Color());
				virtual ~ProgressIndicator() {};
				virtual void indicate(const double value, const double max_value = 100) { mCurrentValue = value; mMaxValue = max_value; };
				virtual Margins draw(ISurface&);
			};

			class CustomProgressIndicator : public IProgressIndicator, public IElement
			{
			protected:
				std::vector<std::string> mFiles;
				double mCurrentValue;									// Value to be indicated
				double mMaxValue;										// Maximum of the scale
				std::string mLoadedBitmapName;
				BitmapSurface mBitmap;
			public:
				CustomProgressIndicator(
					const std::vector<std::string> files) : mFiles(files), mBitmap(0, 0) {};
				virtual ~CustomProgressIndicator() {};
				virtual void indicate(const double value, const double max_value) { mCurrentValue = value; mMaxValue = max_value; };
				virtual void indicate(const double value) { mCurrentValue = value; };
				virtual Margins draw(ISurface&);
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality