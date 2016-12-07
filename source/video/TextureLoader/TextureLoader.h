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

		private:
			void LoadSTB();
		};
	}
}

#endif