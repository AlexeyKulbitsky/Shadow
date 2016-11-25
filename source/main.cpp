#include "Shadow.h"
#include "scene\Mesh.h"
#include "scene\Model.h"
#include "scene\ModelLoader\TinyObjModelLoader.h"
#include <pugixml.hpp>
#include "GLES20\GLES20ShaderProgram.h"
#include "VertexDeclaration.h"
#include "video/Material.h"
#include "video/RenderTechnique.h"
#include "video/RenderPass.h"
#include "video/UniformBuffer.h"
#include "video/Sampler.h"
#include "video\GLES20\GLES20Texture.h"

#include "pempek_assert.h"

#include <iostream>






using namespace sh;
using namespace video;

int main()
{
	sh::Device* device = sh::CreateDevice();
	sh::video::Driver* driver = device->GetDriver();

	sh::scene::ModelLoader* modelLoader = new sh::scene::TinyObjModelLoader();
	//sh::scene::Model* model = modelLoader->Load("../data/models/nanosuit/nanosuit.obj");
	sh::scene::Model* model = modelLoader->Load("../data/models/triangle/triangle.obj");

	sh::video::RenderTechnique* rt = new sh::video::RenderTechnique();
	rt->Load("../data/shaders/gles20/test.xml");

	sh::video::Material* mat = new sh::video::Material();
	mat->SetRenderTechnique(rt);

	sh::video::Texture* texture = driver->CreateTexture();
	texture->SetType(sh::video::Texture::Type::TEXTURE_2D);
	texture->Load("../data/textures/statue.jpg");
	texture->SetFiltering(sh::video::Texture::Filtering::BILINEAR);
	texture->SetTiling(sh::video::Texture::Tiling::REPEAT, sh::video::Texture::Tiling::REPEAT);

	sh::video::UniformBuffer* uniBuffer = mat->GetRenderTechnique()->GetRenderPass(0)->GetShaderProgram()->GetUniformBuffer();
	uniBuffer->GetSampler(0)->Set(texture);

	for (size_t i = 0; i < model->GetMeshesCount(); ++i)
	{
		sh::scene::Mesh* mesh = model->GetMesh(i);
		mesh->SetMaterial(mat);
		mesh->Init();
	}

	if (device)
	{		
		while (true)
		{
			driver->BeginRendering();

			//driver->DrawMesh(nullptr);

			model->Render();

			driver->EndRendering();
		}
	}
	return 0;
}