#include "Shadow.h"
#include "scene\Mesh.h"
#include <pugixml.hpp>
#include "GLES20\GLES20ShaderProgram.h"
#include "Vertex.h"
#include "video/Material.h"
#include "ShAssert.h"

#include <iostream>

float vertices[] =
{
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

short indices[] =
{
	0, 1, 2
};

sh::video::GLES20ShaderProgram shader;
sh::video::VertexDecalaration vertexDeclaration;
sh::video::Material material;


using namespace sh;
using namespace video;

int main()
{
	
	vertexDeclaration.AddAttribute(Attribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3));
	vertexDeclaration.AddAttribute(Attribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 3));

	sh::scene::Mesh mesh;

	mesh.SetVerticesData(vertices, 18);
	mesh.SetIndicesData(indices, 3);

	sh::Device* device = sh::CreateDevice();
	
	shader.Load("../data/shaders/test.xml");

	material.SetShaderProgram(&shader);

	if (device)
	{
		sh::video::Driver* driver = device->GetDriver();

		HardwareBuffer::CreateInfo info;
		info.bufferType = HardwareBuffer::BufferType::VERTEX_AND_INDEX;
		info.verticesUsageType = HardwareBuffer::UsageType::STATIC;
		info.indicesUsageType = HardwareBuffer::UsageType::STATIC;
		info.topology = HardwareBuffer::Topology::TRIANGLE_LIST;

		sh::video::HardwareBuffer *buffer = driver->CreateHardwareBuffer(info);
		driver->InitHardwareBuffer(buffer, (const void*)vertices, 18, (const void*)indices, 3);
		buffer->SetVertexDeclaration(vertexDeclaration);
		mesh.SetHardwareBuffer(buffer);
		mesh.SetMaterial(&material);


		while (true)
		{
			driver->BeginRendering();

			if (buffer)
			{
				//driver->DrawHardwareBuffer(buffer);
				driver->DrawMesh(&mesh);
			}

			driver->EndRendering();
		}
	}
	return 0;
}