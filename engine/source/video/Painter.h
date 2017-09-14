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
			struct Vertex
			{
				Vertex(const math::Vector3f& _position, const math::Vector2f& _uv, const math::Vector4f& _color) 
					: position(_position)
					, uv(_uv)
					, color(_color)
				{ }
				Vertex(const math::Vector2i& _position, const math::Vector2f& _uv, const math::Vector4f& _color) 
					: position(_position, 0.0f)
					, uv(_uv)
					, color(_color)
				{ }
				Vertex() 
					: position(0.0f) 
					, uv(0.0f)
					, color(0.0f)
				{ }
				math::Vector3f position;
				math::Vector2f uv;
				math::Vector4f color;
			};

			Painter();
			void SetMaterial(const MaterialPtr& material);

			// Get current active material
			const MaterialPtr& GetMaterial() const { return m_material; }
			void SetCamera(const scene::CameraPtr& camera);
			void SetClipRect(const math::Rectu& rect);

			void DrawLine(const math::Vector3f& a, const math::Vector3f& b);
			void DrawRect(const math::Rectu& rect, const gui::SpritePtr& sprite);
			void DrawRect(const Vertex& upperLeft, const Vertex& downRight);
			void DrawQuad();
			void DrawCircle();
			void DrawArc();
			void DrawBox(const math::AABBf& box);
			void DrawText();
			void DrawTriangleList(const std::vector<float>& vertices, const std::vector<u32>& indices, u32 verticesCount);

			void Flush();

		private:
			MaterialPtr m_material;

			std::vector<MaterialPtr> m_materials;
			scene::CameraPtr m_camera;

			sh::video::CommandBufferPtr m_commandBuffer;

			// Lines geometry
			std::vector<float> m_linesVertexArray;
			VertexBufferPtr m_linesVertexBuffer;
			struct LinesBatch
			{
				u32 startIndex = 0U;
				u32 verticesCount = 0U;
				u32 materialIndex = 0U;
			};
			struct Lines
			{
				std::vector<LinesBatch> linesBatches;
				u32 verticesCount;
			};
			Lines m_lines;

			// Triangles geometry
			std::vector<float> m_trianglesVertexArray;
			std::vector<u32> m_trianglesIndexArray;
			VertexBufferPtr m_trianglesVertexBuffer;
			IndexBufferPtr m_trianglesIndexBuffer;
			struct TrianglesBatch
			{
				u32 startIndex = 0U;
				u32 indicesCount = 0U;
				u32 verticesCount = 0U;
				u32 materialIndex = 0U;
				math::Rectu clipRect = math::Rectu(0U, 0U, 0U, 0U);
			};
			struct Triangles
			{
				std::vector<TrianglesBatch> trianglesBatches;
				u32 indicesCount = 0U;
				u32 verticesCount = 0U;
			};
			Triangles m_triangles;
			
			
		};
	}
}

#endif
