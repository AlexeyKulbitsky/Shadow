#ifndef SHADOW_COMMAND_POOL_INCLUDE
#define SHADOW_COMMAND_POOL_INCLUDE

#include "../Globals.h"
#include "RenderPass.h"

namespace sh
{
	namespace scene
	{
		class Mesh;
	}

	namespace video
	{
		class CommandBuffer;

		class CommandPool
		{
		public:
			void AddMesh(scene::Mesh* mesh);
			void Submit();

		protected:
			typedef HashMap<String, CommandBuffer*> PerTechniqueCommandBuffersMap;

			PerTechniqueCommandBuffersMap m_buffers[(size_t)RenderPass::Layer::COUNT];
		};
	}
}

#endif