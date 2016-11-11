#include "GLES20Driver.h"
#include "GLES20HardwareBuffer.h"
#include "GLES20ShaderProgram.h"
#include "GLES20VertexDeclaration.h"
#include "GLES20IndexBuffer.h"
#include "GLES20VertexBuffer.h"
#include "GLES20RenderCommand.h"
#include "Material.h"
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

void GLES20Driver::DrawMesh(scene::Mesh* mesh)
{
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::DrawHardwareBuffer(HardwareBuffer *buffer) const
{
	GLES20HardwareBuffer *gles20Buffer = static_cast<GLES20HardwareBuffer*>(buffer);

	switch (gles20Buffer->info.bufferType)
	{
	case HardwareBuffer::BufferType::VERTEX:
		break;
	case HardwareBuffer::BufferType::INDEX:
		break;
	case HardwareBuffer::BufferType::VERTEX_AND_INDEX:
	{
		glBindBuffer(GL_ARRAY_BUFFER, gles20Buffer->glVertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gles20Buffer->glIndexBufferID);

		for (auto attribute : gles20Buffer->glVertexDeclaration->GetAttributes())
		{
			glEnableVertexAttribArray(attribute.index);

			Attribute* attr = gles20Buffer->m_vertexDeclaration.GetAttribute(attribute.semantic);

			glVertexAttribPointer(attribute.index, attribute.size, attribute.type, false, gles20Buffer->m_vertexDeclaration.GetStride(), (const void*)(attr->offset));
		}

		glDrawElements(gles20Buffer->glTopology, gles20Buffer->glIindicesSize, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
		break;
	default:
		break;
	}

	


}

////////////////////////////////////////////////////////////////////////

HardwareBuffer* GLES20Driver::CreateHardwareBuffer(const HardwareBuffer::CreateInfo& info) const
{
	GLES20HardwareBuffer *gles20Buffer = new GLES20HardwareBuffer();
	gles20Buffer->info = info;

	switch (info.bufferType)
	{
	case::HardwareBuffer::BufferType::VERTEX:
		// Create vertex buffer
		glGenBuffers(1, &gles20Buffer->glVertexBufferID);
		break;
	case::HardwareBuffer::BufferType::INDEX:
		// Create index buffer
		glGenBuffers(1, &gles20Buffer->glIndexBufferID);
		break;
	case::HardwareBuffer::BufferType::VERTEX_AND_INDEX:
		// Create vertex and index buffer
		glGenBuffers(1, &gles20Buffer->glVertexBufferID);
		glGenBuffers(1, &gles20Buffer->glIndexBufferID);
		break;
	default:
		break;
	}
	
	return gles20Buffer;
}

void GLES20Driver::InitHardwareBuffer(HardwareBuffer *buffer, const void* verticesPointer, u32 verticesSize, const void* indicesPointer, u32 indicesSize) const
{
	GLES20HardwareBuffer *gles20Buffer = static_cast<GLES20HardwareBuffer*>(buffer);

	switch (gles20Buffer->info.verticesUsageType)
	{
	case HardwareBuffer::UsageType::STATIC:
		gles20Buffer->glVerticesUsageType = GL_STATIC_DRAW;
		break;
	case HardwareBuffer::UsageType::DYNAMIC:
		gles20Buffer->glVerticesUsageType = GL_DYNAMIC_DRAW;
		break;
	case HardwareBuffer::UsageType::STREAM:
		gles20Buffer->glVerticesUsageType = GL_STREAM_DRAW;
		break;
	default:
		break;
	}

	switch (gles20Buffer->info.indicesUsageType)
	{
	case HardwareBuffer::UsageType::STATIC:
		gles20Buffer->glIndicesUsageType = GL_STATIC_DRAW;
		break;
	case HardwareBuffer::UsageType::DYNAMIC:
		gles20Buffer->glIndicesUsageType = GL_DYNAMIC_DRAW;
		break;
	case HardwareBuffer::UsageType::STREAM:
		gles20Buffer->glIndicesUsageType = GL_STREAM_DRAW;
		break;
	default:
		break;
	}

	switch (gles20Buffer->info.topology)
	{
	case HardwareBuffer::Topology::LINE_LIST:
		gles20Buffer->glTopology = GL_LINES;
		break;
	case HardwareBuffer::Topology::LINE_LOOP:
		gles20Buffer->glTopology = GL_LINE_LOOP;
		break;
	case HardwareBuffer::Topology::LINE_STRIP:
		gles20Buffer->glTopology = GL_LINE_STRIP;
		break;
	case HardwareBuffer::Topology::TRIANGLE_FAN:
		gles20Buffer->glTopology = GL_TRIANGLE_FAN;
		break;
	case HardwareBuffer::Topology::TRIANGLE_LIST:
		gles20Buffer->glTopology = GL_TRIANGLES;
		break;
	case HardwareBuffer::Topology::TRIANGLE_STRIP:
		gles20Buffer->glTopology = GL_TRIANGLE_STRIP;
		break;
	default:
		break;
	}

	gles20Buffer->glVerticesSize = verticesSize;
	gles20Buffer->glIindicesSize = indicesSize;

	switch (gles20Buffer->info.bufferType)
	{
	case::HardwareBuffer::BufferType::VERTEX:
	{
		// Init vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, gles20Buffer->glVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), verticesPointer, gles20Buffer->glVerticesUsageType);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
		break;
	case::HardwareBuffer::BufferType::INDEX:
	{
		// Init index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gles20Buffer->glIndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(u16), indicesPointer, gles20Buffer->glIndicesUsageType);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
		break;
	case::HardwareBuffer::BufferType::VERTEX_AND_INDEX:
	{
		// Init vertex and index buffer
		glBindBuffer(GL_ARRAY_BUFFER, gles20Buffer->glVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), verticesPointer, gles20Buffer->glVerticesUsageType);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gles20Buffer->glIndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(u16), indicesPointer, gles20Buffer->glIndicesUsageType);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
		break;
	default:
		break;
	}
	
}

void GLES20Driver::Render(RenderCommand* command)
{
	GLES20RenderCommand* glesRenderCommand = static_cast<GLES20RenderCommand*>(command);
	GLES20VertexBuffer* vertexBuffer = glesRenderCommand->GetGLVertexBuffer();
	GLES20IndexBuffer* indexBuffer = glesRenderCommand->GetGLIndexBuffer();
	GLES20RenderState* renderState = glesRenderCommand->GetGLRenderState();
	GLES20ShaderProgram* shaderProgram = glesRenderCommand->GetGLShaderProgram();
	GLES20VertexDeclaration* vertexDeclaration = glesRenderCommand->GetGLVertexInputDeclaration();

	// Bind current shader
	shaderProgram->BindProgram();

	// Send uniforms to shader


	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetGLId());

	// Send attributes to shader
	for (auto attribute : vertexDeclaration->GetAttributes())
	{
		glEnableVertexAttribArray(attribute.index);
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, false, vertexDeclaration->GetStride(), attribute.pointer);
	}

	// Render with indices
	if (command->IsUseIndices())
	{
		// Bind indices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetGLId());
		// Draw with indices
		glDrawElements(glesRenderCommand->GetGLTopology(), indexBuffer->GetIndicesCount(), indexBuffer->GetGLIndexType(), 0);
		// Unbind indices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0U);
	}
	else
	{
		//glDrawArrays(glesRenderCommand->GetGLTopology(), 0, 3);
	}

	// Unbind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0U);

	// Unbind current shader
	shaderProgram->UnbindProgram();
}

VertexBuffer* GLES20Driver::CreateVertexBuffer()
{
	return nullptr;
}

VertexBuffer* GLES20Driver::CreateVertexBuffer(const void* data, size_t size)
{
	return new GLES20VertexBuffer(data, size);
}

IndexBuffer* GLES20Driver::CreateIndexBuffer()
{
	return nullptr;
}


IndexBuffer* GLES20Driver::CreateIndexBuffer(const void* data, size_t size)
{
	return new GLES20IndexBuffer(data, size);
}

RenderCommand* GLES20Driver::CreateRenderCommand()
{
	return new GLES20RenderCommand();
}

ShaderProgram* GLES20Driver::CreateShaderProgram()
{
	return new GLES20ShaderProgram();
}