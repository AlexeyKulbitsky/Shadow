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
			m_availableExtensions.push_back("png");
			m_availableExtensions.push_back("jpg");
			m_availableExtensions.push_back("tga");
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

		const TexturePtr& TextureLoader::GetWhiteTexture()
		{ 
			if( !m_whiteTexture )
			{
				const u32 textureSize = 8U;
				video::TextureDescription textureDesc;
				textureDesc.type = TEX_TYPE_TEXTURE_2D;
				textureDesc.width = textureSize;
				textureDesc.height = textureSize;
				textureDesc.format = TextureFormat::RGBA;

				m_whiteTexture = video::Texture::Create(textureDesc);

				const u32 color = 0xFFFFFFFF;
				std::vector<u32> data(textureSize * textureSize, color);
				m_whiteTexture->SetData(0U, (char*)data.data());
			}

			return m_whiteTexture; 
		}

		//////////////////////////////////////////////////////////////

		TexturePtr TextureLoader::LoadSTB(const String& filename)
		{
			io::File file = io::FileSystem::GetInstance()->LoadFile(filename);
			const char* dataPtr = file.GetData().data();
			const u8* uDataPtr = reinterpret_cast<const u8*>(dataPtr);
			
			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = stbi_load_from_memory(uDataPtr, file.GetData().size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			SH_ASSERT(pixels, "failed to load texture image!");

			TextureDescription desc;
			desc.type = TEX_TYPE_TEXTURE_2D;
			desc.format = TextureFormat::RGBA;
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
			size_t i = 0;

			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = nullptr;
			{
				io::File file = io::FileSystem::GetInstance()->LoadFile(faces[i]);
				const char* dataPtr = file.GetData().data();
				const u8* uDataPtr = reinterpret_cast<const u8*>(dataPtr);
				pixels = stbi_load_from_memory(uDataPtr, file.GetData().size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
				SH_ASSERT(pixels, "failed to load texture image!");
			}


			TextureDescription desc;
			desc.type = TEX_TYPE_TEXTURE_CUBE;
			desc.format = TextureFormat::RGBA;
			desc.width = static_cast<u32>(texWidth);
			desc.height = static_cast<u32>(texHeight);
			TexturePtr texture = Texture::Create(desc);

			texture->SetFaceData(TextureFace(i), 0, pixels);
			stbi_image_free(pixels);

			for (i = 1; i < faces.size(); ++i)
			{
				io::File file = io::FileSystem::GetInstance()->LoadFile(faces[i]);
				const char* dataPtr = file.GetData().data();
				const u8* uDataPtr = reinterpret_cast<const u8*>(dataPtr);
				pixels = stbi_load_from_memory(uDataPtr, file.GetData().size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
				SH_ASSERT(pixels, "failed to load texture image!");
				
				texture->SetFaceData(TextureFace(i), 0, pixels);

				stbi_image_free(pixels);
			}

			return texture;
		}

		//////////////////////////////////////////////////////////////

	}
}