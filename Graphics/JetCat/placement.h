#ifndef __PLACEMENT_H__
#define __PLACEMENT_H__

#include "drawable.h"
#include "inputs.h"

namespace Componentality
{
	namespace Graphics
	{
		namespace JetCat
		{

			// Movable container. Main method to place sub-elements to given location
			class Placement : public IElement, public IMovable, public ISizeable
			{
			public:
				Placement();
				Placement(const size_t x, const size_t y, const size_t width, const size_t height);
				virtual ~Placement();
				virtual bool isMovable() const { return true; }
			protected:
				virtual Margins draw(ISurface&);
			};

			// Unmovable container. Only used to limit re-drawing
			class Container : public IElement
			{
			public:
				Container() {};
				virtual ~Container() {};
			};

			// Cached placement
			class FastPlacement : public Placement, public Restorable
			{
			protected:
				Margins mMargins;
			public:
				FastPlacement();
				virtual ~FastPlacement();
			protected:
				virtual Margins draw(ISurface&);
				virtual void raise(CST::Common::Event& event);
				virtual bool onEvent(CST::Common::Event&);
			};

			// Placement restoring original image
			class Substrate : public Restorable, public IElement
			{
			public:
				Substrate() {};
				virtual ~Substrate() {};
			protected:
				virtual Margins draw(ISurface&);
				virtual bool onEvent(CST::Common::Event&);
			};

		} // namespace JetCat
	} // namespace Graphics
} // namespace Componentality

#endif