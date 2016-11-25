#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADWO_MATERIAL_INCLUDE

#include "Color.h"

namespace sh
{
	namespace video
	{
		class RenderTechnique;

		class Material
		{
		public:
			Material() {}
			void SetRenderTechnique(RenderTechnique* technique) { m_renderTechnique = technique; }

			RenderTechnique* GetRenderTechnique() { return m_renderTechnique; }
		private:
			RenderTechnique* m_renderTechnique = nullptr;
		};
	}
}

#endif