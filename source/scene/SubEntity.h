#ifndef SHADOW_SUB_ENTITY_INCLUDE
#define SHADOW_SUB_ENTITY_INCLUDE

namespace sh
{
	namespace video
	{
		class Material;
		class RenderCommand;
	}

	namespace scene
	{
		class Mesh;

		class SubEntity
		{
		public:
			explicit SubEntity(Mesh* mesh);

			void Render();
			void SetMaterial(sh::video::Material* material);
			video::RenderCommand* GetRenderCommand() { return m_renderCommand; }

		private:
			SubEntity(){}
			SubEntity(const SubEntity& other){}
			
		private:
			Mesh* m_mesh;			
			video::Material* m_material;
			video::RenderCommand* m_renderCommand;
		};
	}
}

#endif