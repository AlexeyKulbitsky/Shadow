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
		class SHADOW_API RenderBatchManager : public Singleton<RenderBatchManager>
		{
		public:
			virtual ~RenderBatchManager() { }
			virtual void AddMesh(const scene::MeshPtr& mesh) {}
			virtual void RemoveMesh(const scene::MeshPtr& mesh) {}
			virtual void Submit() {}
			virtual void Clear() {}
			virtual void UpdateLight(scene::Light* light) {}
		};
	}
}

#endif