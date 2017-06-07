#include "ModelLoader.h"

namespace sh
{
	namespace scene
	{
		ModelBasePtr ModelLoader::Load(const char* path)
		{
			return Load(std::string(path));
		}
	}
}