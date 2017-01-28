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
			Driver* driver = Device::GetInstance()->GetDriver();
			TexturePtr texture = driver->CreateTexture();
			texture->SetType(Texture::Type::TEXTURE_2D);

			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			SH_ASSERT(pixels, "failed to load texture image!");
			
			texture->Bind();

			texture->LoadData(0, texWidth, texHeight, pixels);
			texture->GenerateMipMaps();

			texture->Unbind();

			stbi_image_free(pixels);

			return texture;
		}

		//////////////////////////////////////////////////////////////

		TexturePtr TextureLoader::LoadSTBCube(const std::vector<String>& faces)
		{
			Driver* driver = Device::GetInstance()->GetDriver();
			TexturePtr texture = driver->CreateTexture();
			texture->SetType(Texture::Type::TEXTURE_CUBE);

			texture->Bind();

			int texWidth, texHeight, texChannels;			
			for (size_t i = 0; i < faces.size(); ++i)
			{
				stbi_uc* pixels = stbi_load(faces[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
				
				texture->LoadFaceData(Texture::Face(i), 0, texWidth, texHeight, pixels);

				stbi_image_free(pixels);
			}

			texture->GenerateMipMaps();
			texture->Unbind();

			return texture;
		}

		//////////////////////////////////////////////////////////////

	}
}