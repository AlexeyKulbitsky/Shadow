#include "Shadow.h"
#include "scene\Mesh.h"
#include <pugixml.hpp>
#include "GLES20\GLES20ShaderProgram.h"

#include <iostream>

float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};
sh::scene::Mesh mesh;
sh::video::GLES20ShaderProgram shader;

int main()
{
	mesh.Init(vertices, 18);

	sh::Device* device = sh::CreateDevice();

	shader.Load("../data/shaders/test.xml");

	if (device)
	{
		sh::video::Driver* driver = device->GetDriver();

		sh::video::HardwareBuffer *buffer = driver->CreateHardwareBuffer(&mesh);

		while (true)
		{
			driver->BeginRendering();

			if (buffer)
			{
				driver->DrawHardwareBuffer(buffer);
			}

			driver->EndRendering();
		}
	}
	return 0;
}