#ifndef __MESSAGEBOX_H__
#define __MESSAGEBOX_H__

#include "drawable.h"
#include "label.h"
#include "window.h"
#include "placement.h"
#include "buttons.h"
#include "inputs.h"
#include <map>

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class MessageWindow : public ModalWindow
			{
			protected:
				Label*  mLabel;
				Shadow* mShadow;
				Frame*  mFrame;
				Background* mBackground;
				Placement* mLabelPlacement;
				std::map<IDrawable*, Placement*> mPlacements;
				std::map<IDrawable*, Placement*> mIndex;
				size_t mButtonY;
			public:
				MessageWindow(WindowManager& owner, const bool greying = false);
				virtual ~MessageWindow();
			public:
				virtual Margins draw(ISurface&);
				virtual void addButton(const Tag&, IElement&, size_t width = 0, size_t height = 0);
				virtual void removeButton(const Tag&);
				virtual void setText(const std::string& text) { mLabel->setText(text); }
				virtual void setText(const std::list<int>& text) { mLabel->setText(text); }
				virtual std::list<int> getText() const { return mLabel->getText(); }
				virtual FontSet::FontDescriptor getFont() const { return mLabel->getFont(); }
				virtual void setFont(const FontSet::FontDescriptor font) { mLabel->setFont(font); }
				virtual size_t getButtonY() const { return mButtonY; }
				virtual void setButtonY(const size_t button_y) { mButtonY = button_y; }
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif
