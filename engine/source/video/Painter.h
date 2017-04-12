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
			Painter();
			void SetMaterial(const MaterialPtr& material);
			void SetCamera(const scene::CameraPtr& camera);

			void DrawLine(const math::Vector3f& a, const math::Vector3f& b);
			void DrawRect();
			void DrawQuad();
			void DrawCircle();
			void DrawArc();
			void DrawBox();
			void DrawText();

			void Flush();

		private:
			MaterialPtr m_material;
			scene::CameraPtr m_camera;

			std::vector<float> m_linesVertexArray;
			VertexBufferPtr m_linesBuffer;
			//RenderCommandPtr m_linesRenderCommand;
		};
	}
}

#endif
