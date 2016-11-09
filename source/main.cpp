#include "Shadow.h"
#include "scene\Mesh.h"
#include <pugixml.hpp>
#include "GLES20\GLES20ShaderProgram.h"
#include "VertexDeclaration.h"
#include "video/Material.h"
#include "pempek_assert.h"

#include <iostream>



sh::video::GLES20ShaderProgram shader;
sh::video::Material material;


using namespace sh;
using namespace video;

int main()
{
	sh::Device* device = sh::CreateDevice();

	sh::scene::Mesh mesh;

// 	shader.Load("../data/shaders/test.xml");
// 
// 	material.SetShaderProgram(&shader);

	if (device)
	{
		sh::video::Driver* driver = device->GetDriver();

// 		HardwareBuffer::CreateInfo info;
// 		info.bufferType = HardwareBuffer::BufferType::VERTEX_AND_INDEX;
// 		info.verticesUsageType = HardwareBuffer::UsageType::STATIC;
// 		info.indicesUsageType = HardwareBuffer::UsageType::STATIC;
// 		info.topology = HardwareBuffer::Topology::TRIANGLE_LIST;
// 
// 		sh::video::HardwareBuffer *buffer = driver->CreateHardwareBuffer(info);
// 		driver->InitHardwareBuffer(buffer, (const void*)vertices, 18, (const void*)indices, 3);
// 		buffer->SetVertexDeclaration(vertexDeclaration);
// 		mesh.SetHardwareBuffer(buffer);
// 		mesh.SetMaterial(&material);

		
		while (true)
		{
			driver->BeginRendering();

// 			if (buffer)
// 			{
// 				driver->DrawMesh(&mesh);
// 			}

			//driver->DrawHardwareBuffer(nullptr);
			driver->DrawMesh(nullptr);

			driver->EndRendering();
		}
	}
	return 0;
}