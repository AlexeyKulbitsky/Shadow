#ifndef SHADOW_MODEL_LOADER_INCLUDE
#define SHADOW_MODEL_LOADER_INCLUDE

#include <string>

namespace sh
{
	namespace scene
	{
		class Model;

		class ModelLoader
		{
		public:
			ModelLoader(){}
			virtual ~ModelLoader(){}

			virtual Model* Load(const char* path) = 0;
			virtual Model* Load(const std::string &path) = 0;

			static void SetInstance(ModelLoader* loader);
			static ModelLoader* GetInstance();
		protected:
			static ModelLoader* s_modelLoader;
		};
	}
}

#endif