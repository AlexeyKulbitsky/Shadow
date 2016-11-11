#ifndef SHADOW_TINY_OBJ_MODEL_LOADER_INCLUDE
#define SHADOW_TINY_OBJ_MODEL_LOADER_INCLUDE

#include "ModelLoader.h"

namespace sh
{
	namespace scene
	{
		class TinyObjModelLoader: public ModelLoader
		{
		public:
			TinyObjModelLoader();
			virtual ~TinyObjModelLoader();

			virtual Model* Load(const char* path) override;
			virtual Model* Load(const std::string &path) override;
		};
	}
}
#endif
