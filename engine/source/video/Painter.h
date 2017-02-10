#ifndef SHADOW_PAINTER_INCLUDE
#define SHADOW_PAINTER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class Painter
		{
		public:
			void SetMaterial(const MaterialPtr& material);
			void SetCamera(const scene::CameraPtr& camera);

			void DrawLine(const math::Vector3f& a, const math::Vector3f& b);
			void DrawRect();
			void DrawQuad();
			void DrawCircle();
			void DrawArc();
			void DrawBox();
			void DrawText();

		private:
			MaterialPtr m_material;
			scene::CameraPtr m_camera;
		};
	}
}

#endif
