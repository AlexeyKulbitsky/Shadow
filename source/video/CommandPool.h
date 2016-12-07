#ifndef SHADOW_COMMAND_POOL_INCLUDE
#define SHADOW_COMMAND_POOL_INCLUDE

#include "../Globals.h"

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
			HashMap<String, CommandBuffer*> m_buffers;
		};
	}
}

#endif