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

			virtual ModelBase* Load(const char* path) override;
			virtual ModelBase* Load(const std::string &path) override;
		};
	}
}
#endif
