#include "inputs.h"

using namespace Componentality::Graphics;
using namespace Componentality::Graphics::JetCat;

const std::string Componentality::Graphics::JetCat::EVENT_TYPE_POINTER		= ".evt.pointer";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_PRESS		= ".evt.press";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_DEPRESS		= ".evt.depress";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_DRAW_REQUEST	= ".evt.draw";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_TIMER		= ".evt.timer";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_MENU			= ".evt.menu";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_KEY			= ".evt.key";
const std::string Componentality::Graphics::JetCat::EVENT_TYPE_SET_DEFAULT  = ".evt.default";

bool PointerEvent::filter(Componentality::Graphics::Point lefttop, Componentality::Graphics::Point rightbottom)
{
	Componentality::Graphics::Point pos = get();

	if (pos.x < lefttop.x)
		return false;
	if (pos.y < lefttop.y)
		return false;
	if (pos.x > rightbottom.x)
		return false;
	if (pos.y > rightbottom.y)
		return false;

	return true;
}

TimerEvent::Counter TimerEvent::mCounter(0);