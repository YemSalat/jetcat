#ifndef __MENU_H__
#define __MENU_H__

#include "drawable.h"
#include "placement.h"
#include "focus.h"
#include "scroller.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			class IMenu : public Scroller
			{
			public:
				enum SelectionStrategy
				{
					NONE,						// No selection, just call
					SINGLE,						// Select given item, deselect previously selected item
					MULTIPLE					// Toggle selection without referring to other items
				};
			protected:
				CST::Common::mutex mLock;
				std::map<Tag, IElement*> mUnselectedItems;			// All menu items (unselected images)
				std::map<Tag, IElement*> mSelectedItems;			// All menu items (selected images)
				std::map<Tag, IElement*> mSelected;					// List of items being currently selected
				SelectionStrategy mSelectionStrategy;				// Selection strategy
				StdEventFilter mEventsFilter;
			public:
				IMenu();
				virtual ~IMenu();
				virtual void add(const Tag, IElement& unselected, IElement& selected);
				virtual void remove(const Tag& tag);
				virtual void select(const Tag&);
				virtual void unselect(const Tag&);
				virtual Tag  has(IElement&) = 0;
				virtual bool isSelected(const Tag&);
				virtual std::list<Tag> listSelected();
				virtual void setSelectionStrategy(const SelectionStrategy ss) { mSelectionStrategy = ss; }
				virtual SelectionStrategy getSelectionStrategy() const { return mSelectionStrategy; }
			protected:
				virtual Margins draw(ISurface&);
				virtual void add(const Tag& tag, IElement& element) { IElement::add(tag, element); };
				virtual bool onEvent(CST::Common::Event&);
				virtual size_t getPosition(const Tag& tag);
				virtual Tag  getByPosition(const size_t);
				virtual bool isHidden(const Tag& tag) { return mUnselectedItems[tag]->isHidden(); }
				virtual DrawOrder getOrder();
			};

			// Menu of the items having their pre-calculated placements
			class IPlaceableMenu : public IMenu
			{
			protected:
				std::map<Tag, std::pair<Placement*, Placement*> > mPlacements;
				std::map<Tag, IElement*> mUnselectedIndex;			// Index of originally added items
				std::map<Tag, IElement*> mSelectedIndex;			// Index of originally added items
				std::map<IElement*, Tag> mReverseIndex;				// Reverse index
			public:
				IPlaceableMenu() {};
				virtual ~IPlaceableMenu() {};
				virtual void add(const Tag&, IElement& unselected, IElement& selected);
				virtual void remove(const Tag& tag);
				virtual Tag  has(IElement&);
			protected:
				virtual Point getLocation(const Tag&) = 0;
				virtual std::pair<size_t, size_t> getSize(const Tag&) = 0;
				virtual void rearrange();
				virtual void initLocation() {};
				virtual Margins draw(ISurface&);
				virtual bool isHidden(const Tag& tag) { return mUnselectedIndex[tag]->isHidden(); }
			};

			class IVerticalMenu : public IPlaceableMenu
			{
			protected:
				std::map<Tag, size_t> mVerticalLocation;
				std::map<Tag, size_t> mItemsVerticalSize;
				size_t mVerticalSize;
				size_t mHorizontalSize;
			public:
				IVerticalMenu(const size_t h_size);
				virtual ~IVerticalMenu();
				virtual void add(const Tag&, IElement& unselected, IElement& selected, const size_t row_size);
				virtual void remove(const Tag& tag);
			protected:
				virtual Point getLocation(const Tag& tag);
				virtual std::pair<size_t, size_t> getSize(const Tag& tag) { return std::pair<size_t, size_t>(mHorizontalSize, mItemsVerticalSize[tag]); };
				virtual void add(const Tag& tag, IElement& unselected, IElement& selected) { return IPlaceableMenu::add(tag, unselected, selected); };
				virtual void initLocation() { mVerticalSize = 0; };
			};

			class IHorizontalMenu : public IPlaceableMenu
			{
			protected:
				std::map<Tag, size_t> mHorizontalLocation;
				std::map<Tag, size_t> mItemsHorizontalSize;
				size_t mHorizontalSize;
				size_t mVerticalSize;
			public:
				IHorizontalMenu(const size_t v_size);
				virtual ~IHorizontalMenu();
				virtual void add(const Tag&, IElement& unselected, IElement& selected, const size_t col_size);
				virtual void remove(const Tag& tag);
			protected:
				virtual Point getLocation(const Tag& tag);
				virtual std::pair<size_t, size_t> getSize(const Tag& tag) { return std::pair<size_t, size_t>(mItemsHorizontalSize[tag], mVerticalSize); };
				virtual void add(const Tag& tag, IElement& unselected, IElement& selected) { return IPlaceableMenu::add(tag, unselected, selected); };
				virtual void initLocation() { mHorizontalSize = 0; };
			};

			class IMenuItem : public IElement
			{
			protected:
				IMenu& mMenu;
			public:
				IMenuItem(IMenu& owner) : mMenu(owner) {}
				virtual ~IMenuItem() {};
			};

			class TextMenuItem : public IMenuItem
			{
			protected:
				IElement* mGround;
				IElement* mText;
			public:
				TextMenuItem(IMenu& menu, const Color foreground, const Color background, const std::string text = std::string());
				TextMenuItem(IMenu& menu, const Color foreground, const Color background, const std::list<int>);
				virtual ~TextMenuItem();
				virtual std::pair<Color, Color> getColor() const;
				virtual std::list<int> getText() const;
			protected:
				virtual bool onEvent(CST::Common::Event&);
			};

			class VerticalMenu : public IVerticalMenu, public IFocusable
			{
			public:
				VerticalMenu(IFocusController&, const size_t hsize);
				virtual ~VerticalMenu();
				virtual void select(const Tag&);
			protected:
				virtual Margins draw(ISurface&);
				virtual bool onEvent(CST::Common::Event&);
				virtual bool onSetFocus() { return true; };
				virtual bool onReleaseFocus() { return true; };
			};

			class HorizontalMenu : public IHorizontalMenu, public IFocusable
			{
			public:
				HorizontalMenu(IFocusController&, const size_t vsize);
				virtual ~HorizontalMenu();
				virtual void select(const Tag&);
			protected:
				virtual Margins draw(ISurface&);
				virtual bool onEvent(CST::Common::Event&);
				virtual bool onSetFocus() { return true; };
				virtual bool onReleaseFocus() { return true; };
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif