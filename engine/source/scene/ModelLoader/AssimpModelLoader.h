#ifndef SHADOW_ASSIMP_MODEL_LOADER_INCLUDE
#define SHADOW_ASSIMP_MODEL_LOADER_INCLUDE
#if 0
#include "ModelLoader.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace sh
{

namespace scene
{

	class AssimpModelLoader : public ModelLoader
	{
	public:
		AssimpModelLoader(){}
		virtual ~AssimpModelLoader(){}

		virtual ModelBasePtr Load(const std::string &path) override;

	private:
		void ProcessNode(aiNode* node, const aiScene* scene, const ModelBasePtr& model);
		MeshBasePtr ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};

} // scene

} // sh

#endif

#endif