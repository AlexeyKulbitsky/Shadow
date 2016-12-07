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
			
			Texture(){}
			virtual ~Texture(){}

			virtual void SetType(Type type) = 0;
			virtual void SetTiling(Tiling tilingU, Tiling tilingV) = 0;
			virtual void SetFiltering(Filtering filtering) = 0;

			bool HasMipMaps() const { return m_hasMipMaps; }

		protected:
			bool m_hasMipMaps = false;
		};
	}
}
#endif // !SHADOW_TEXTURE_INCLUDE