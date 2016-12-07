#include "TextureLoader.h"
#include "../Texture.h"

#include "../../Device.h"
#include "../Driver.h"

namespace sh
{
	namespace video
	{
		TextureLoader::TextureLoader()
		{

		}

		//////////////////////////////////////////////////////////////

		TextureLoader::~TextureLoader()
		{

		}

		//////////////////////////////////////////////////////////////

		Texture* TextureLoader::Load(const String& path)
		{
			Driver* driver = Device::GetInstance()->GetDriver();
			Texture* texture = driver->CreateTexture();


			return texture;
		}

		//////////////////////////////////////////////////////////////

		void TextureLoader::LoadSTB()
		{

		}

	}
}