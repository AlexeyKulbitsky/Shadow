#ifndef SHADOW_RENDER_BATCH_MANAGER_INCLUDE
#define SHADOW_RENDER_BATCH_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderBatchManager
		{
		public:
			virtual ~RenderBatchManager() { }
			virtual void AddMesh(scene::Mesh* mesh) = 0;
			virtual void Submit() = 0;
			virtual void Clear() = 0;
		};
	}
}

#endif