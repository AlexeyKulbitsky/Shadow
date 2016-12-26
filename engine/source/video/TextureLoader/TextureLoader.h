#ifndef SHADOW_TEXTURE_LOADER_INCLUDE
#define SHADOW_TEXTURE_LOADER_INCLUDE

#include "../../Globals.h"

namespace sh
{
	namespace video
	{
		class Texture;

		class TextureLoader
		{
		public:
			TextureLoader();
			~TextureLoader();

			Texture* Load(const String& path);
			Texture* LoadCube(const std::vector<String>& faces);

		private:
			Texture* LoadSTB(const String& path);
			Texture* LoadSTBCube(const std::vector<String>& faces);
		};
	}
}

#endif