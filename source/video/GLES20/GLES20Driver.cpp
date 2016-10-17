#include "GLES20Driver.h"
#include "GLES20HardwareBuffer.h"
#include "../GLContext/EGLContextManager.h"
#include "../../scene/Mesh.h"

using namespace sh;
using namespace video;

GLES20Driver::GLES20Driver(EGLContextManager* contextManager)
	:m_contextManager(contextManager)
{
}

bool GLES20Driver::Init()
{
	return false;
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::BeginRendering()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::EndRendering()
{
	glFlush();
	m_contextManager->SwapBuffers();
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::DrawHardwareBuffer(const HardwareBuffer *buffer)
{
	const GLES20HardwareBuffer *gles20Buffer = static_cast<const GLES20HardwareBuffer*>(buffer);

	const void *verticesPointer = nullptr;
	const void *indicesPointer = nullptr;

	if (gles20Buffer->verticesUsageType != HardwareBuffer::UsageType::NONE)
	{
		glBindBuffer(GL_ARRAY_BUFFER, gles20Buffer->vbo_verticesID);
		verticesPointer = nullptr;
	}
	
	if (gles20Buffer->indicesUsageType != HardwareBuffer::UsageType::NONE)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gles20Buffer->vbo_indicesID);
		indicesPointer = nullptr;
	}
	
	// Positions
	//glEnableVertexAttribArray(index);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), verticesPointer);



	glDrawElements(GL_TRIANGLES, gles20Buffer->vbo_indicesSize, GL_UNSIGNED_INT, indicesPointer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////

HardwareBuffer* GLES20Driver::CreateHardwareBuffer(const scene::Mesh* mesh)
{
	const void* verticesPointer = mesh->GetVerticesPointer();
	const u32 verticesCount = mesh->GetVerticesCount();

	const void* indicesPointer = mesh->GetIndicesPointer();
	const u32 indicesCount = mesh->GetIndicesCount();

	GLES20HardwareBuffer *buffer = new GLES20HardwareBuffer();

	// Create vertex buffer
	glGenBuffers(1, &buffer->vbo_verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo_verticesID);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float), verticesPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create index buffer
	glGenBuffers(1, &buffer->vbo_indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo_indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(u16), indicesPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return buffer;
}