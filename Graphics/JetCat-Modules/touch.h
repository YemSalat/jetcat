#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "../JetCat/inputs.h"
#include "../JetCat/drawable.h"
#include "../JetCat/placement.h"

#ifdef WIN32
#define DEFAULT_CALIBRATION_FILE "c:\\Componentality\\touch_calibrations.dat"
#else
#define DEFAULT_CALIBRATION_FILE "/var/componentality/touch_calibrations.dat"
#endif

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{
			extern const std::string EVENT_TYPE_LONG_PRESS;		// Event generated upon long press
			extern const std::string EVENT_TYPE_SHORT_PRESS;	// Event generated upon short press

			class LongPressEvent : public PressEvent
			{
			public:
				LongPressEvent() { mType = EVENT_TYPE_LONG_PRESS; }
				virtual ~LongPressEvent() {};
			};

			class ShortPressEvent : public PressEvent
			{
			public:
				ShortPressEvent() { mType = EVENT_TYPE_SHORT_PRESS; }
				virtual ~ShortPressEvent() {};
			};

			class TouchEventsProcessor : public Container
			{
			protected:
				size_t mLongPressBound;						// Time in milliseconds exceeding which we consider as long press
				size_t mTicksAfterPressed;					// Number of 100ms ticks after press happened
				bool   mSourceEventTransfer;				// True if source press/depress events to be also transferred on long press
				Point  mPoint;								// Point where pressed
			public:
				TouchEventsProcessor(const size_t bound, const bool process_source = true)
					: mLongPressBound(bound), mTicksAfterPressed(0), mSourceEventTransfer(process_source) {};
				virtual ~TouchEventsProcessor() {}
			protected:
				virtual bool onEvent(CST::Common::Event& event);
			};

			class TouchCalibrator : public Container
			{
			public:
				struct CALIBRATIONS
				{
					double mKX;
					double mBX;
					double mKY;
					double mBY;
				};
			protected:
				CALIBRATIONS mCalibrations;
			public:
				TouchCalibrator(const double kx = 1.0, const double ky = 1.0, const double bx = 0.0, const double by = 0.0);
				TouchCalibrator(const CALIBRATIONS&);
				TouchCalibrator(const std::string& filename);
				virtual ~TouchCalibrator();
				virtual operator CALIBRATIONS() const { return mCalibrations; };
				virtual TouchCalibrator& operator=(const CALIBRATIONS&);
				virtual void set(const double kx, const double ky, const double bx, const double by);
				virtual bool write(const std::string& filename);
				virtual bool read(const std::string& filename);
			protected:
				virtual bool onEvent(CST::Common::Event& event);
			};

			class Calibrate : public TouchCalibrator
			{
			protected:
				Point* mLeftTop;
				Point* mRightTop;
				Point* mLeftBottom;
				Point* mRightBottom;
				size_t mWidth;
				size_t mHeight;
				bool   mCleaned;
				std::string mCalibrationFile;
				bool   mCalibrated;
			public:
				Calibrate(const std::string mCalibrationFile = DEFAULT_CALIBRATION_FILE);
				virtual ~Calibrate();
			protected:
				virtual Margins draw(ISurface&);
				virtual bool onEvent(CST::Common::Event& event);
				virtual void drawTarget(ISurface&, Point);
				virtual void clear(ISurface&);
				static void calibrate(double p1, double p2, double a1, double a2, double& k, double& b);
				void calibrate(Point& pressed1, Point& pressed2, Point drawn1, Point drawn2);
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
