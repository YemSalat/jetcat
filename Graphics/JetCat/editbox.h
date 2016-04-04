#ifndef __EDITBOX_H__
#define __EDITBOX_H__

#include "drawable.h"
#include "label.h"
#include "cursor.h"
#include "focus.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class EditBox : public Label, public IFocusable
			{
			protected:
				CST::Common::mutex mLock;
				Cursor* mCursor;
				Color   mCursorColor;
				Color	mBackgroundColor;
				Color	mFrameColor;
				size_t  mPosition;
				int		mScrolling;
				int		mSpace;
				StdEventFilter mEventsFilter;
			public:
				EditBox(IFocusController&);
				EditBox(IFocusController&, const std::string text);
				EditBox(IFocusController&, const std::string text, Componentality::Graphics::Color color, Margins margins = Margins());
				EditBox(IFocusController&, const std::list<int> text);
				EditBox(IFocusController&, const std::list<int> text, Componentality::Graphics::Color color, Margins margins = Margins());
				virtual ~EditBox();
				virtual Margins draw(ISurface& surface);
				virtual size_t getPosition() const { return mPosition; }
				virtual void setPosition(const size_t pos);
				virtual Color getCursorColor() const { return mCursorColor; }
				virtual void setCursorColor(const Color color) { mCursorColor = color; }
				virtual Color getBackgroundColor() const { return mBackgroundColor; }
				virtual void setBackgroundColor(const Color color) { mBackgroundColor = color; }
				virtual Color getFrameColor() const { return mFrameColor; }
				virtual void setFrameColor(const Color color) { mFrameColor = color; }
			protected:
				virtual std::pair<Point, std::pair<size_t, size_t> > positionCursor(ISurface& surface);
				virtual bool onSetFocus();
				virtual bool onReleaseFocus();
				virtual bool onEvent(CST::Common::Event&);
				void init();
				virtual bool isAlphabetic(const int);
				virtual void insert(const int);
				virtual void remove();
			protected:
				virtual IAlignmentDefinitions::Alignments getAlignments() const { return Label::getAlignments(); }
				virtual void setAlignment(const IAlignmentDefinitions::Alignments align) { Label::setAlignment(align); }
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
