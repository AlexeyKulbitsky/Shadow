#include "Shadow.h"
#include "scene\Mesh.h"
#include "scene\Model.h"
#include "scene\ModelLoader\TinyObjModelLoader.h"
#include <pugixml.hpp>
#include "GLES20\GLES20ShaderProgram.h"
#include "VertexDeclaration.h"
#include "video/Material.h"
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
	sh::video::ShaderProgram* shaderProgram = driver->CreateShaderProgram();
	shaderProgram->Load("../data/shaders/gles20/test.xml");

	sh::video::Texture* texture = driver->CreateTexture();
	texture->SetType(sh::video::Texture::Type::TEXTURE_2D);
	texture->Load("../data/textures/statue.jpg");
	texture->SetFiltering(sh::video::Texture::Filtering::BILINEAR);
	texture->SetTiling(sh::video::Texture::Tiling::REPEAT, sh::video::Texture::Tiling::REPEAT);

	for (size_t i = 0; i < model->GetMeshesCount(); ++i)
	{
		sh::scene::Mesh* mesh = model->GetMesh(i);
		mesh->SetShaderProgram(shaderProgram);
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