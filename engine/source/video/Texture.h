#ifndef SHADOW_TEXTURE_INCLUDE
#define SHADOW_TEXTURE_INCLUDE

#include "../resources/Resource.h"

namespace sh
{
	namespace video
	{
		struct TextureDescription
		{
			TextureType type;
			TextureFormat format;
			u32 width;
			u32 height;
			u32 depth;
			u32 mipLevels;
			u32 samplesCount;
		};

		class Texture : public Resource
		{
		public:			
			Texture(){}
			virtual ~Texture(){}

			virtual void SetType(TextureType type) = 0;
			virtual void SetTiling(TextureTiling tilingU, TextureTiling tilingV) = 0;
			virtual void SetFiltering(TextureFiltering filtering) = 0;
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void LoadData(u32 mipLevel, s32 width, s32 height, const void* data) = 0;
			virtual void LoadFaceData(TextureFace face, u32 mipLevel, s32 width, s32 height, const void* data) = 0;
			virtual void GenerateMipMaps() = 0;

			bool HasMipMaps() const { return m_hasMipMaps; }

			static TexturePtr Create(const TextureDescription& description);

		protected:
			Texture(const TextureDescription& description);

		protected:
			bool m_hasMipMaps = false; 
			TextureDescription m_description;

			friend class TextureManager;
		};
	}
}
#endif // !SHADOW_TEXTURE_INCLUDE