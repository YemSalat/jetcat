#include "../Surface/bmp_surface.h"
#include "../Surface/easy_draw.h"
#include "../Drawing/brush.h"
#include "../Drawing/viewport.h"
#include "../Drawing/sprite.h"
#include "../Fonts/character.h"
#include "../Fonts/FontSet.h"
#include "../Drawing/transparency.h"

#include "../../../common-libs/common/common_utilities.h"

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("Usage: CJFmaker <input-fnt-file> <input-bmp-texture> <output-fonts-folder>\n");
		return 1;
	}

	{
		CST::Common::File file;
		file.open(argv[1], CST::Common::File::READ);
		if (!file.good() || file.bad())
		{
			printf("Cannot open FNT file\n");
			return 2;
		}
		size_t size = file.size();
		char* content = new char[size];
		file.read(content, size);
		CST::Common::blob blb;
		blb.mData = content; blb.mSize = size;
		std::string text = CST::Common::blobToString(blb);
		file.close();

		Componentality::Graphics::BitmapSurface bmps(1, 1);
		bmps.read(argv[2]);

		Componentality::Graphics::IndexedFont font;
		font.readFNT(text, bmps);

		Componentality::Graphics::LoadableFontSet fset(argv[3]);
		fset.append(font);
		fset.serialize();
	}

	return 0;
}

