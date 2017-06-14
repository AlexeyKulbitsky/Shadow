#ifndef SHADOW_TEXTURE_LOADER_INCLUDE
#define SHADOW_TEXTURE_LOADER_INCLUDE

#include "../../Globals.h"

namespace sh
{
	namespace video
	{
		class TextureLoader : public Singleton<TextureLoader>
		{
		public:
			TextureLoader();
			~TextureLoader();

			TexturePtr Load(const String& path);
			TexturePtr LoadCube(const std::vector<String>& faces);

			const TexturePtr& GetWhiteTexture();

		private:
			TexturePtr LoadSTB(const String& path);
			TexturePtr LoadSTBCube(const std::vector<String>& faces);

		private:
			TexturePtr m_whiteTexture;
		};
	}
}

#endif