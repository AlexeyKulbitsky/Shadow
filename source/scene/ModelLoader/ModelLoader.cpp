#include "ModelLoader.h"

namespace sh
{
	namespace scene
	{
		ModelLoader* ModelLoader::s_modelLoader = nullptr;

		void ModelLoader::SetInstance(ModelLoader* loader)
		{
			s_modelLoader = loader;
		}

		ModelLoader* ModelLoader::GetInstance()
		{
			return s_modelLoader;
		}
	}
}