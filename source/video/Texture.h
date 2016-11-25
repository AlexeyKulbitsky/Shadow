#ifndef SHADOW_TEXTURE_INCLUDE
#define SHADOW_TEXTURE_INCLUDE

namespace sh
{
	namespace video
	{
		class Texture
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
			
			Texture(){}
			virtual ~Texture(){}

			virtual void Load(const char* filename) = 0;
			virtual void SetType(Type type) = 0;
			virtual void SetTiling(Tiling tilingU, Tiling tilingV) = 0;
			virtual void SetFiltering(Filtering filtering) = 0;
		};
	}
}
#endif // !SHADOW_TEXTURE_INCLUDE