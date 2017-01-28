#ifndef SHADOW_MODEL_LOADER_INCLUDE
#define SHADOW_MODEL_LOADER_INCLUDE

#include "../../Globals.h"

namespace sh
{
	namespace scene
	{
		class ModelLoader
		{
		public:
			ModelLoader(){}
			virtual ~ModelLoader(){}

			virtual ModelBasePtr Load(const char* path) = 0;
			virtual ModelBasePtr Load(const std::string &path) = 0;

			static void SetInstance(ModelLoader* loader);
			static ModelLoader* GetInstance();
		protected:
			static ModelLoader* s_modelLoader;
		};
	}
}

#endif