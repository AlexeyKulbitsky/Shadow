#include "TextureLoader.h"
#include "../Texture.h"

#include "../../Device.h"
#include "../Driver.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

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

		TexturePtr TextureLoader::Load(const String& path)
		{			
			// Check file extension
			String::size_type pos = String(path).find_last_of(".");
			String extension = String(path).substr(pos + 1);
			if (extension == "png")
			{
				printf("Extension PNG\n");
			}
			else if (extension == "jpg")
			{
				printf("Extension JPG\n");
			}

			return LoadSTB(path);
		}

		//////////////////////////////////////////////////////////////

		TexturePtr TextureLoader::LoadCube(const std::vector<String>& faces)
		{
			return LoadSTBCube(faces);
		}

		//////////////////////////////////////////////////////////////

		TexturePtr TextureLoader::LoadSTB(const String& path)
		{
			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			SH_ASSERT(pixels, "failed to load texture image!");

			TextureDescription desc;
			desc.type = TEX_TYPE_TEXTURE_2D;
			desc.format = texChannels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
			desc.width = texWidth;
			desc.height = texHeight;
			TexturePtr texture = Texture::Create(desc);
			
			texture->SetData(0, pixels);

			stbi_image_free(pixels);

			return texture;
		}

		//////////////////////////////////////////////////////////////

		TexturePtr TextureLoader::LoadSTBCube(const std::vector<String>& faces)
		{
			TextureDescription desc;
			desc.type = TEX_TYPE_TEXTURE_CUBE;
			TexturePtr texture = Texture::Create(desc);

			
			int texWidth, texHeight, texChannels;			
			for (size_t i = 0; i < faces.size(); ++i)
			{
				stbi_uc* pixels = stbi_load(faces[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
				
				texture->SetFaceData(TextureFace(i), 0, pixels);

				stbi_image_free(pixels);
			}


			return texture;
		}

		//////////////////////////////////////////////////////////////

	}
}