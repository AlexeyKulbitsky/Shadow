#ifndef SHADOW_PAINTER_INCLUDE
#define SHADOW_PAINTER_INCLUDE

#include "../Globals.h"
#include "GpuParams.h"

namespace sh
{
	namespace video
	{
		class SHADOW_API Painter
		{
		public:
			struct Vertex
			{
				Vertex(const math::Vector3& _position, const math::Vector2& _uv, const math::Vector4& _color) 
					: position(_position)
					, uv(_uv)
					, color(_color)
				{ }
				Vertex(const math::Vector2Int& _position, const math::Vector2& _uv, const math::Vector4& _color) 
					: position((float)_position.x, (float)_position.y, 0.0f)
					, uv(_uv)
					, color(_color)
				{ }
				Vertex() 
					: position(0.0f) 
					, uv(0.0f)
					, color(0.0f)
				{ }
				math::Vector3 position;
				math::Vector2 uv;
				math::Vector4 color;
			};

			Painter();
			// Set current active material
			void SetMaterial(const MaterialPtr& material);
			// Get current active material
            const MaterialPtr& GetMaterial() const;
			void SetCamera(const scene::CameraPtr& camera);
            const scene::CameraPtr& GetCamera() const;
			// Set current clipping rectangle
			void SetClipRect(const math::Rect& rect);
			// Get current clipping rectangle
			const math::Rect& GetClipRect() const;

			void DrawLine(const math::Vector3& a, const math::Vector3& b);
			void DrawRect(const math::Rect& rect, const gui::SpritePtr& sprite);
			void DrawRect(const Vertex& upperLeft, const Vertex& downRight);
			void DrawQuad(const Vertex& downLeft, const Vertex& upperLeft, const Vertex& upperRight, const Vertex& downRight);
			void DrawCircle();
			void DrawArc();
			void DrawBox(const math::AABB& box);
			void DrawText();
			void DrawTriangleList(const std::vector<float>& vertices, const std::vector<u32>& indices, u32 verticesCount);

			void Flush();

		private:
			MaterialPtr m_material;

			std::vector<MaterialPtr> m_materials;
            std::vector<scene::CameraPtr> m_cameras;
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
                u32 cameraIndex = 0U;
				math::Rect clipRect = math::Rect(0, 0, 0, 0);
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
                u32 cameraIndex = 0U;
				math::Rect clipRect = math::Rect(0, 0, 0, 0);
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
