#ifndef SHADOW_MODEL_LOADER_INCLUDE
#define SHADOW_MODEL_LOADER_INCLUDE

#include "../../Globals.h"

namespace sh
{
	namespace scene
	{
		class ModelLoader : public Singleton<ModelLoader>
		{
		public:
			ModelLoader(){}
			virtual ~ModelLoader(){}

			ModelBasePtr Load(const char* path);

			virtual ModelBasePtr Load(const std::string &path) = 0;
		};
	}
}

#endif