#include "../Surface/bmp_surface.h"
#include "../Surface/easy_draw.h"
#include "../Drawing/brush.h"
#include "../Drawing/viewport.h"
#include "../Drawing/sprite.h"
#include "../Fonts/character.h"
#include "../Fonts/FontSet.h"
#include "../Drawing/transparency.h"
#include "../../../common-libs/libXjson/xjson_conv.h"
#include "../../../common-libs/common/common_utilities.h"
#include <iostream>

using namespace Componentality::Graphics;

const std::string FONT_PATH = "c:\\Componentality\\Fonts";
std::string OUT_FOLDER = "c:\\temp\\FONTS";

int main(int argc, char* argv[])
{
	CST::Common::makeDirectory(OUT_FOLDER);

	LoadableFontSet fontset(FONT_PATH);
	fontset.deserialize();
	std::list<FontSet::FontDescriptor> fonts = fontset.list();
	for (std::list<FontSet::FontDescriptor>::iterator i = fonts.begin(); i != fonts.end(); i++)
	{
		std::cout << "Reading fonts -> " << "Font face: " << i->getFace() << ", size: " << i->getSize() << ", type: " << i->getType() << std::endl;
		fontset.find(*i);
		fontset.setFolder(OUT_FOLDER);
		fontset.serialize(*i);
		fontset.setFolder(FONT_PATH);
		fontset.unload(*i);
	}


	return 0;
}

