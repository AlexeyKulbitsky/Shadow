#ifndef SHADOW_PAINTER_INCLUDE
#define SHADOW_PAINTER_INCLUDE

#include "../Globals.h"
#include "GpuParams.h"

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

			struct LinesRenderable
			{
				VertexBufferPtr vertexBuffer;
				sh::video::GpuParamsPtr params;
				sh::video::GpuParamMatrix4f wvpMatrix;
				sh::video::CommandBufferPtr commandBuffer;
			};

			struct LinesBatch
			{
				VertexInputDeclarationPtr inputDeclaration;
				u32 startIndex = 0U;
				u32 verticesCount = 0U;

			};
			LinesRenderable m_linesRenderable;
			LinesBatch m_linesBatch;
		};
	}
}

#endif
