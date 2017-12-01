#ifndef SHADOW_MODEL_LOADER_INCLUDE
#define SHADOW_MODEL_LOADER_INCLUDE

#include "../../Globals.h"

namespace sh
{
	namespace scene
	{
		class SHADOW_API ModelLoader : public Singleton<ModelLoader>
		{
		public:
			ModelLoader(){}
			virtual ~ModelLoader(){}

			ModelBasePtr Load(const char* path);

			virtual ModelBasePtr Load(const std::string &path) { return ModelBasePtr(); }
			const std::vector<String>& GetAvalilableExtensions() const { return m_availableExtensions; }

		protected:
			std::vector<String> m_availableExtensions;
		};
	}
}

#endif