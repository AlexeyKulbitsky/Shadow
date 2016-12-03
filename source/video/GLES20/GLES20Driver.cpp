#include "GLES20Driver.h"
#include "GLES20HardwareBuffer.h"
#include "GLES20ShaderProgram.h"
#include "GLES20VertexDeclaration.h"
#include "GLES20IndexBuffer.h"
#include "GLES20VertexBuffer.h"
#include "GLES20UniformBuffer.h"
#include "GLES20RenderCommand.h"
#include "GLES20Texture.h"
#include "Material.h"
#include "../GLContext/EGLContextManager.h"
#include "../../scene/Mesh.h"
#include "../UniformBuffer.h"

using namespace sh;
using namespace video;

GLES20Driver::GLES20Driver(EGLContextManager* contextManager)
	:m_contextManager(contextManager)
{
}

bool GLES20Driver::Init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	return false; 
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::BeginRendering()
{
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


void GLES20Driver::Render(RenderCommand* command)
{
	GLES20RenderCommand* glesRenderCommand = static_cast<GLES20RenderCommand*>(command);
	GLES20VertexBuffer* vertexBuffer = glesRenderCommand->GetGLVertexBuffer();
	GLES20IndexBuffer* indexBuffer = glesRenderCommand->GetGLIndexBuffer();
	GLES20RenderState* renderState = glesRenderCommand->GetGLRenderState();
	GLES20ShaderProgram* shaderProgram = glesRenderCommand->GetGLShaderProgram();
	GLES20VertexDeclaration* vertexDeclaration = glesRenderCommand->GetGLVertexInputDeclaration();
	UniformBuffer* uniformBuffer = glesRenderCommand->GetUniformBuffer();

	// Bind current shader
	shaderProgram->BindProgram();

	// Send uniforms to shader
	uniformBuffer->Upload();

	// Bind vertex buffer
	vertexBuffer->Bind();

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
		indexBuffer->Bind();
		// Draw with indices
		glDrawElements(glesRenderCommand->GetGLTopology(), indexBuffer->GetIndicesCount(), indexBuffer->GetGLIndexType(), 0);
		// Unbind indices buffer
		indexBuffer->Unbind();
	}
	else
	{
		//glDrawArrays(glesRenderCommand->GetGLTopology(), 0, 3);
	}

	// Unbind vertex buffer
	vertexBuffer->Unbind();

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

UniformBuffer* GLES20Driver::CreateUniformBuffer()
{
	return new GLES20UniformBuffer();
}

RenderCommand* GLES20Driver::CreateRenderCommand()
{
	return new GLES20RenderCommand();
}

ShaderProgram* GLES20Driver::CreateShaderProgram()
{
	return new GLES20ShaderProgram();
}

Texture* GLES20Driver::CreateTexture()
{
	return new GLES20Texture();
}

VertexInputDeclaration* GLES20Driver::CreateVertexInputDeclaration()
{
	return new GLES20VertexDeclaration();
}

RenderState* GLES20Driver::CreateRenderState()
{
	return new GLES20RenderState();
}