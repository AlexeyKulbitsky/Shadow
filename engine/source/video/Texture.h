#ifndef SHADOW_TEXTURE_INCLUDE
#define SHADOW_TEXTURE_INCLUDE

#include "../resources/Resource.h"

namespace sh
{
	namespace video
	{
		struct SHADOW_API TextureDescription
		{
			TextureType type;
			TextureFormat format;
			u32 width;
			u32 height;
			u32 depth;
			u32 mipLevels;
			u32 samplesCount;
		};

		class SHADOW_API Texture : public Resource
		{
		public:			
			Texture(){}
			virtual ~Texture(){}

			virtual void SetData(u32 mipLevel, void* data) = 0;
			virtual void SetFaceData(TextureFace face, u32 mipLevel, const void* data) = 0;

			const TextureDescription& GetDescription() const { return m_description; }
			static TexturePtr Create(const TextureDescription& description);

		protected:
			Texture(const TextureDescription& description);

		protected:
			bool m_hasMipMaps = false; 
			TextureDescription m_description;
		};
	}
}
#endif // !SHADOW_TEXTURE_INCLUDE