#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADWO_MATERIAL_INCLUDE

#include "Color.h"

namespace sh
{
	namespace video
	{
		class Material
		{
		public:
			Material()
				: ambientColor(255U, 255U, 255U, 255U)
				, diffuseColor(255U, 255U, 255U, 255U)
				, emissiveColor(255U, 255U, 255U, 255U)
				, specularColor(255U, 255U, 255U, 255U)
			{}

			Material(const Material& other)
				: ambientColor(other.ambientColor)
				, diffuseColor(other.diffuseColor)
				, emissiveColor(other.emissiveColor)
				, specularColor(other.specularColor)
			{}
			Color ambientColor;
			Color diffuseColor;
			Color emissiveColor;
			Color specularColor;		
		};
	}
}

#endif