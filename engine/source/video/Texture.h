#ifndef SHADOW_TEXTURE_INCLUDE
#define SHADOW_TEXTURE_INCLUDE

#include "../resources/Resource.h"

namespace sh
{
	namespace video
	{
		class Texture : public Resource
		{
		public:
			enum class Type
			{
				TEXTURE_1D,
				TEXTURE_2D,
				TEXTURE_3D,
				TEXTURE_CUBE
			};

			enum class Tiling
			{
				REPEAT,
				MIRRORED_REPEAT,
				CLAMP_TO_EDGE
			};

			enum class Filtering
			{
				NEAREST,
				LINEAR,
				BILINEAR,
				TRILINEAR,
				ANISOTROPIC
			};

			enum class Face
			{
				FRONT_FACE,
				BACK_FACE,
				RIGHT_FACE,
				LEFT_FACE,
				TOP_FACE,
				BOTTOM_FACE,

				COUNT = 6
			};

			enum class Format
			{
				UNDEFINED,
				DXT1_RGB,
				DXT1_RGBA
			};
			
			Texture(){}
			virtual ~Texture(){}

			virtual void SetType(Type type) = 0;
			virtual void SetTiling(Tiling tilingU, Tiling tilingV) = 0;
			virtual void SetFiltering(Filtering filtering) = 0;
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void LoadData(u32 mipLevel, s32 width, s32 height, const void* data) = 0;
			virtual void LoadFaceData(Face face, u32 mipLevel, s32 width, s32 height, const void* data) = 0;
			virtual void GenerateMipMaps() = 0;

			bool HasMipMaps() const { return m_hasMipMaps; }

		protected:
			bool m_hasMipMaps = false;
		};
	}
}
#endif // !SHADOW_TEXTURE_INCLUDE