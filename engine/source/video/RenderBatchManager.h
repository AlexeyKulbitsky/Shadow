#ifndef SHADOW_RENDER_BATCH_MANAGER_INCLUDE
#define SHADOW_RENDER_BATCH_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class Light;
	}

	namespace video
	{
		class RenderBatchManager : public Singleton<RenderBatchManager>
		{
		public:
			virtual ~RenderBatchManager() { }
			virtual void AddMesh(const scene::MeshPtr& mesh) = 0;
			virtual void Submit() = 0;
			virtual void Clear() = 0;
			virtual void UpdateLight(scene::Light* light) = 0;
		};
	}
}

#endif