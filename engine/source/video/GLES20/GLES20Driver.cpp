#include "GLES20Driver.h"
#include "GLES20ShaderProgram.h"
#include "GLES20VertexDeclaration.h"
#include "GLES20IndexBuffer.h"
#include "GLES20VertexBuffer.h"
#include "GLES20UniformBuffer.h"
#include "GLES20RenderCommand.h"
#include "GLES20Texture.h"
#include "../Material.h"
#include "../GLContext/EGLContextManager.h"
#include "../../scene/Mesh.h"
#include "../../scene/Model.h"
#include "../UniformBuffer.h"
#include "../DepthStencilState.h"
#include "../RasterizationState.h"
#include <sstream>
using namespace sh;
using namespace video;

static GLenum const s_glWriteMask[] =
{
	GL_FALSE,
	GL_TRUE
};

static GLenum const s_glFrontFace[] =
{
	GL_CW,
	GL_CCW
};

static GLenum const s_glCullFace[] =
{
	GL_FRONT,
	GL_BACK,
	GL_NONE
};

static GLenum const s_glCompareFunction[] =
{
	GL_LESS,
	GL_LEQUAL,
	GL_EQUAL,
	GL_GEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_ALWAYS,
	GL_NEVER
};

static GLenum const s_glStencilOperation[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INVERT,
	GL_INCR,
	GL_DECR,
	GL_INCR_WRAP,
	GL_DECR_WRAP
};

static GLenum const s_glBlendFactor[] =
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_SRC_ALPHA_SATURATE
};
		
static GLenum const s_gBlendOperation[] =
{
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX
};

GLES20Driver::GLES20Driver(EGLContextManager* contextManager)
	:m_contextManager(contextManager)
{
}

////////////////////////////////////////////////////////////////////////

bool GLES20Driver::Init()
{
	InitGlobalUniforms();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);


	String vendor = (const char*)glGetString(GL_VENDOR);
	printf("Vendor: %s\n", vendor.c_str());
	String renderer = (const char*)glGetString(GL_RENDERER);
	printf("Renderer: %s\n", renderer.c_str());
	String version = (const char*)glGetString(GL_VERSION);
	printf("Version: %s\n", version.c_str());
	String shadingLanguage = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Shading language: %s\n", shadingLanguage.c_str());

	// Extensions
	String extensions = (const char*)glGetString(GL_EXTENSIONS);
	String buf; 
    std::stringstream ss(extensions); 
    std::vector<String> extensionsList; 
    while (ss >> buf)
        extensionsList.push_back(buf);
	printf("Enabled extensions:\n");
	for (auto ext : extensionsList)
	{
		printf("%s\n", ext.c_str());
	}

	GLint value;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
	m_maxTextureUnits = (u32)value;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
	m_maxTextureSize = (u32)value;

	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &value);
	m_maxCubeTextureSize = (u32)value;

	
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &value);
	GLint *valuePtr = new GLint[value];
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, valuePtr);
	for (int i = 0; i < value; ++i)
	{
		Texture::Format format = GLES20Texture::GetFormatFromGL(valuePtr[i]);
	}
	delete[] valuePtr;
	

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

void GLES20Driver::Render(RenderCommand* command)
{
	GLES20RenderCommand* glesRenderCommand = static_cast<GLES20RenderCommand*>(command);
	GLES20VertexBuffer* vertexBuffer = glesRenderCommand->GetGLVertexBuffer();
	GLES20IndexBuffer* indexBuffer = glesRenderCommand->GetGLIndexBuffer();
	GLES20VertexDeclaration* vertexDeclaration = glesRenderCommand->GetGLVertexInputDeclaration();
	UniformBuffer* uniformBuffer = glesRenderCommand->GetUniformBuffer();

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
		glDrawArrays(glesRenderCommand->GetGLTopology(), 0, vertexBuffer->GetVerticesCount());
	}

	// Unbind vertex buffer
	vertexBuffer->Unbind();
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetViewport(u32 x, u32 y, u32 width, u32 height)
{
	Driver::SetViewport(x, y, width, height);
	glViewport((GLint)x, (GLint)y, (GLint)width, (GLint)height);
}


////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetDepthRange(f32 zMin, f32 zMax)
{
	Driver::SetDepthRange(zMin, zMax);
	glDepthRangef(zMin, zMax);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetClearColor(const math::Vector4f& clearColor)
{
	Driver::SetClearColor(clearColor);
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetClearDepth(f32 clearDepth)
{
	Driver::SetClearDepth(clearDepth);
	glClearDepthf(m_clearDepth);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetClearStencil(u32 clearStencil)
{
	Driver::SetClearStencil(clearStencil);
	glClearStencil(static_cast<GLint>(m_clearStencil));
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearColorBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearDepthBuffer()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearStencilBuffer()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetDepthStencilState(DepthStencilState* depthStencilState)
{
	if (depthStencilState->enableDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(s_glCompareFunction[static_cast<size_t>(depthStencilState->depthCompareFunction)]);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	//glDepthMask(s_glWriteMask[static_cast<size_t>(depthStencilState->depthWriteMask)]);

	// Stencil part
	if (depthStencilState->enableStencilTest)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFuncSeparate(GL_FRONT,
			s_glCompareFunction[static_cast<size_t>(depthStencilState->frontFace.compare)],
			depthStencilState->reference,
			depthStencilState->stencilReadMask);
		glStencilMaskSeparate(GL_FRONT, depthStencilState->stencilWriteMask);
		glStencilOpSeparate(GL_FRONT,
			s_glStencilOperation[static_cast<size_t>(depthStencilState->frontFace.fail)],
			s_glStencilOperation[static_cast<size_t>(depthStencilState->frontFace.depthFail)],
			s_glStencilOperation[static_cast<size_t>(depthStencilState->frontFace.pass)]);

		glStencilFuncSeparate(GL_BACK,
			s_glCompareFunction[static_cast<size_t>(depthStencilState->backFace.compare)],
			depthStencilState->reference,
			depthStencilState->stencilReadMask);
		glStencilMaskSeparate(GL_BACK, depthStencilState->stencilWriteMask);
		glStencilOpSeparate(GL_BACK,
			s_glStencilOperation[static_cast<size_t>(depthStencilState->backFace.fail)],
			s_glStencilOperation[static_cast<size_t>(depthStencilState->backFace.depthFail)],
			s_glStencilOperation[static_cast<size_t>(depthStencilState->backFace.pass)]);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}	
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetRasterizationState(RasterizationState* rasterizationState)
{
	if (rasterizationState->cullFace != CullFace::NONE)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(s_glCullFace[static_cast<size_t>(rasterizationState->cullFace)]);
		glFrontFace(s_glFrontFace[static_cast<size_t>(rasterizationState->frontFace)]);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetBlendingState(BlendingState*)
{

}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::GetPixelData(u32 x, u32 y, u32 width, u32 height, u8* data)
{
	GLint viewport[4]; 
    glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

////////////////////////////////////////////////////////////////////////

VertexBufferPtr GLES20Driver::CreateVertexBuffer()
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////

VertexBufferPtr GLES20Driver::CreateVertexBuffer(const void* data, size_t size)
{
	VertexBufferPtr result = nullptr;
	result.reset(new GLES20VertexBuffer(data, size));
	return result;
}

////////////////////////////////////////////////////////////////////////

IndexBufferPtr GLES20Driver::CreateIndexBuffer()
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////

IndexBufferPtr GLES20Driver::CreateIndexBuffer(const void* data, size_t size)
{
	IndexBufferPtr result = nullptr;
	result.reset(new GLES20IndexBuffer(data, size));
	return result;
}

////////////////////////////////////////////////////////////////////////

UniformBufferPtr GLES20Driver::CreateUniformBuffer()
{
	UniformBufferPtr result = nullptr;
	result.reset(new GLES20UniformBuffer());
	return result;
}

////////////////////////////////////////////////////////////////////////

RenderCommand* GLES20Driver::CreateRenderCommand()
{
	return new GLES20RenderCommand();
}

////////////////////////////////////////////////////////////////////////

ShaderProgramPtr GLES20Driver::CreateShaderProgram()
{
	ShaderProgramPtr result = nullptr;
	result.reset(new GLES20ShaderProgram());
	return result;
}

////////////////////////////////////////////////////////////////////////

TexturePtr GLES20Driver::CreateTexture()
{
	TexturePtr result = nullptr;
	result.reset(new GLES20Texture());
	return result;
}

////////////////////////////////////////////////////////////////////////

VertexInputDeclaration* GLES20Driver::CreateVertexInputDeclaration()
{
	return new GLES20VertexDeclaration();
}

////////////////////////////////////////////////////////////////////////
