#include "GL3Driver.h"
#include "GL3VertexDeclaration.h"
#include "../GLIndexBuffer.h"
#include "../GLVertexBuffer.h"
#include "../GLTexture.h"
#include "../GLRenderTarget.h"
#include "GL3RenderPipeline.h"
#include "Batching/GL3RenderBatchManager.h"
#include "../GLCommon.h"
#include "../GLShader.h"

#include "Managers/GL3RenderStateManager.h"
#include "../Managers/GLHardwareBufferManager.h"
#include "../Managers/GLCommandBufferManager.h"
#include "../Managers/GLTextureManager.h"

#include "../../../gui/SpriteManager.h"

#include "../../Material.h"
#include "../GLContext/EGLContextManager.h"
#include "../../../scene/Mesh.h"
#include "../../../scene/Model.h"
#include "../../DepthStencilState.h"
#include "../../RasterizationState.h"
#include "../../BlendingState.h"
#include "../../GpuParams.h"
#include "../../Sampler.h"

#include "../../../Device.h"
#include "../../../scene/SceneManager.h"
#include "../../../scene/Camera.h"

#include <sstream>
using namespace sh;
using namespace video;


GLuint VAO, VBO;


int shaderProgram;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

GL3Driver::GL3Driver(GLContextManager* contextManager)
	:m_contextManager(contextManager)
{
	RenderStateManager::CreateInstance<GL3RenderStateManager>();
	HardwareBufferManager::CreateInstance<GLHardwareBufferManager>();
	CommandBufferManager::CreateInstance<GLCommandBufferManager>();
	RenderBatchManager::CreateInstance<GL3RenderBatchManager>();
	TextureManager::CreateInstance<GLTextureManager>();
}

////////////////////////////////////////////////////////////////////////

GL3Driver::~GL3Driver()
{
	glDeleteVertexArrays(1, &VAO);
	RenderStateManager::DestroyInstance();
	HardwareBufferManager::DestroyInstance();
	CommandBufferManager::DestroyInstance();
	RenderBatchManager::DestroyInstance();
	TextureManager::DestroyInstance();
}

////////////////////////////////////////////////////////////////////////

const String& GL3Driver::GetApiName() const
{
	static const String s_apiName = "GL3";
	return s_apiName;
}

////////////////////////////////////////////////////////////////////////

video::DriverType GL3Driver::GetType() const
{
	return DriverType::OPENGL_3;
}

////////////////////////////////////////////////////////////////////////

bool GL3Driver::Init()
{
	sh::Device::GetInstance()->windowResizeEvent.Connect(
		std::bind(&GL3Driver::OnWindowResized, this, std::placeholders::_1, std::placeholders::_2));

	sh::Device::GetInstance()->sursafeChangedEvent.Connect(
		std::bind(&GL3Driver::SetWindow, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	String extensions;// = (const char*)glGetString(GL_EXTENSIONS);
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

	int depthBits;
	glGetIntegerv(GL_DEPTH_BITS, &depthBits);


	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &value);
	GLint *valuePtr = new GLint[value];
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, valuePtr);
	for (int i = 0; i < value; ++i)
	{
		;//TextureFormat format = GLES20Texture::GetFormatFromGL( valuePtr[i] );
	}
	delete[] valuePtr;

	glGenVertexArrays(1, &VAO);










	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);







	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);






	return false;
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::BeginRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::EndRendering()
{
	glUseProgram(shaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glFlush();
	m_contextManager->SwapBuffers();
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetWindow(void* winId, u32 width, u32 height)
{
	m_contextManager->CreateContext(winId);
	SetViewport(0, 0, width, height);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetViewport(u32 x, u32 y, u32 width, u32 height)
{
	Driver::SetViewport(x, y, width, height);
	glViewport((GLint)x, (GLint)y, (GLint)width, (GLint)height);
}


////////////////////////////////////////////////////////////////////////

void GL3Driver::SetDepthRange(f32 zMin, f32 zMax)
{
	Driver::SetDepthRange(zMin, zMax);
	glDepthRangef(zMin, zMax);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetClearColor(const math::Vector4& clearColor)
{
	Driver::SetClearColor(clearColor);
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetClearDepth(f32 clearDepth)
{
	Driver::SetClearDepth(clearDepth);
	glClearDepthf(m_clearDepth);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetClearStencil(u32 clearStencil)
{
	Driver::SetClearStencil(clearStencil);
	glClearStencil(static_cast<GLint>(m_clearStencil));
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::ClearStencilBuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetDepthStencilState(const DepthStencilStatePtr& depthStencilState)
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

void GL3Driver::SetRasterizationState(const RasterizationStatePtr& rasterizationState)
{
	if (rasterizationState->cullFace != CullFace::CF_NONE)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(s_glCullFace[rasterizationState->cullFace]);
		glFrontFace(s_glFrontFace[rasterizationState->frontFace]);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetBlendingState(const BlendingStatePtr& blendingState)
{
	if (blendingState->enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(s_glBlendFactor[static_cast<size_t>(blendingState->srcAlpha)],
			s_glBlendFactor[static_cast<size_t>(blendingState->dstAlpha)]);
		glBlendEquation(s_glBlendOperation[static_cast<size_t>(blendingState->operationAlpha)]);

		/*
		glBlendFuncSeparate(s_glBlendFactor[static_cast<size_t>(blendingState->srcColor)],
		s_glBlendFactor[static_cast<size_t>(blendingState->dstColor)],
		s_glBlendFactor[static_cast<size_t>(blendingState->srcAlpha)],
		s_glBlendFactor[static_cast<size_t>(blendingState->dstAlpha)]);

		glBlendEquationSeparate(s_glBlendOperation[static_cast<size_t>(blendingState->operationColor)],
		s_glBlendOperation[static_cast<size_t>(blendingState->operationAlpha)]);

		glBlendColor(float, float, float, float);
		*/
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr&)
{
	GL3RenderPipeline* glPipeline = static_cast<GL3RenderPipeline*>(pipeline.get());
	SetBlendingState(glPipeline->GetBlendingState());
	SetRasterizationState(glPipeline->GetRasterizationState());
	SetDepthStencilState(glPipeline->GetDepthStencilState());
	glUseProgram(glPipeline->GetProgramID());
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetComputePipeline()
{

}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr&)
{
	const u8* data = params->GetData();
	const auto& samplers = params->GetSamplers();
	const auto& paramsInfo = params->GetParamsInfo();
	int samplerCounter = 0;

	for (size_t i = 0; i < 2U; ++i)
	{
		ShaderType shaderType = static_cast<ShaderType>(i);

		const SPtr<GpuParamsDescription>& desc = paramsInfo->GetParamsDescription(shaderType);
		if (!desc)
			continue;

		// Upload single params
		for (const auto& param : desc->params)
		{
			const GLfloat* dataPtr = reinterpret_cast<const float*>(data + param.second.offset);

			switch (param.second.type)
			{
			case GPDT_FLOAT1:
			{
				glUniform1fv(param.second.location, 1, dataPtr);
			}
			break;
			case GPDT_FLOAT2:
			{
				glUniform2fv(param.second.location, 1, dataPtr);
			}
			break;
			case GPDT_FLOAT3:
			{
				glUniform3fv(param.second.location, 1, dataPtr);
			}
			break;
			case GPDT_FLOAT4:
			{
				glUniform4fv(param.second.location, 1, dataPtr);
			}
			break;
			case GPDT_MATRIX3:
			{
				glUniformMatrix3fv(param.second.location, 1, GL_FALSE, dataPtr);
			}
			break;
			case GPDT_MATRIX4:
			{
				glUniformMatrix4fv(param.second.location, 1, GL_FALSE, dataPtr);
			}
			break;
			}
		}

		// Upload samplers

		for (const auto& samplerDesc : desc->samplers)
		{
			const u32 index = paramsInfo->GetIndex(samplerDesc.second.set, samplerDesc.second.binding);
			const auto& sampler = samplers[index];

			GLTexture* texture = static_cast<GLTexture*>(sampler->GetTexture().get());
			glActiveTexture(GL_TEXTURE0 + samplerCounter);
			GLenum textureTraget = s_glTextureType[texture->GetDescription().type];
			glBindTexture(textureTraget, texture->GetGLId());

			const auto& description = sampler->GetDescription();
			// Set minification filter
			{
				switch (description.mipFilter)
				{
				case TEX_FILT_NEAREST:
				{
					switch (description.minFilter)
					{
					case TEX_FILT_NONE:
					case TEX_FILT_NEAREST:
						glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						break;
					case TEX_FILT_LINEAR:
						glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						break;
					default:
						break;
					}
				}
				break;
				case TEX_FILT_LINEAR:
				{
					switch (description.minFilter)
					{
					case TEX_FILT_NONE:
					case TEX_FILT_NEAREST:
						glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						break;
					case TEX_FILT_LINEAR:
						glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					default:
						break;
					}
				}
				break;
				case TEX_FILT_NONE:
				{
					glTexParameteri(textureTraget, GL_TEXTURE_MIN_FILTER, s_glTextureFiltering[description.minFilter]);
				}
				break;
				default:
					break;
				}
			}
			// Set magnification filter
			{
				glTexParameteri(textureTraget, GL_TEXTURE_MAG_FILTER, s_glTextureFiltering[description.magFilter]);
			}
			glTexParameteri(textureTraget, GL_TEXTURE_WRAP_S, s_glTextureTiling[description.tilingU]);
			glTexParameteri(textureTraget, GL_TEXTURE_WRAP_T, s_glTextureTiling[description.tilingV]);
			if (texture->GetDescription().type == TEX_TYPE_TEXTURE_CUBE)
				glTexParameteri(textureTraget, GL_TEXTURE_WRAP_R, s_glTextureTiling[description.tilingW]);

			glUniform1i(samplerDesc.second.binding, samplerCounter);
			samplerCounter++;
		}
	}
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetTopology(Topology topology, const CommandBufferPtr&)
{
	m_currentTopology = s_glTopology[static_cast<size_t>(topology)];
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr&)
{
	GL3VertexDeclaration* vertexDeclaration = static_cast<GL3VertexDeclaration*>(declaration.get());
	for (auto attribute : vertexDeclaration->GetAttributes())
	{
		glEnableVertexAttribArray(attribute.index);
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, false, vertexDeclaration->GetStride(), attribute.pointer);
	}
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr&)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLVertexBuffer* glBuffer = static_cast<GLVertexBuffer*>(buffer.get());
	m_currentVertexBuffer = glBuffer->GetGLId();
	glBindBuffer(GL_ARRAY_BUFFER, m_currentVertexBuffer);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr&)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLIndexBuffer* glBuffer = static_cast<GLIndexBuffer*>(buffer.get());
	m_currentIndexBuffer = glBuffer->GetGLId();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_currentIndexBuffer);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::Draw(u32 offset, u32 verticesCount, u32 instancesCount, const CommandBufferPtr&)
{
	glDrawArrays(m_currentTopology, offset, verticesCount);
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::DrawIndexed(u32 offset, u32 indicesCount, u32 instancesCount, const CommandBufferPtr&)
{
	glDrawElements(m_currentTopology, indicesCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(u32)));
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::SetScissorRect(const math::Rect& scissor, const CommandBufferPtr&)
{
	const s32 height = static_cast<s32>(m_viewPort.w);
	glScissor(static_cast<s32>(scissor.upperLeftCorner.x),
		height - static_cast<s32>(scissor.upperLeftCorner.y + scissor.GetHeight()),
		scissor.GetWidth(),
		scissor.GetHeight());
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::GetPixelData(u32 x, u32 y, u8* data)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

////////////////////////////////////////////////////////////////////////

RenderTargetPtr GL3Driver::CreateRenderTarget() const
{
	RenderTargetPtr result = nullptr;
	result.reset(new GLRenderTarget());
	return result;
}

////////////////////////////////////////////////////////////////////////

void GL3Driver::OnWindowResized(int width, int height)
{
	SetViewport(0U, 0U, static_cast<u32>(width), static_cast<u32>(height));
}




























#if 0





bool GLisExtensionSupported(const char *extension)
{
	const char* supported = (const char*)glGetString(GL_EXTENSIONS);
	if (!supported)
		return false;

	const size_t extlen = strlen(extension);
	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match 
		p = strstr(p, extension);
		if (p == NULL)
			return false;  // No Match
						   // Make Sure That Match Is At The Start Of The String Or That
						   // The Previous Char Is A Space, Or Else We Could Accidentally
						   // Match "wglFunkywglExtension" With "wglExtension"
						   // Also, Make Sure That The Following Character Is Space Or NULL
						   // Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return true; // Match
	}
}

bool WGLisExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;
	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());
	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);
	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match 
		p = strstr(p, extension);
		if (p == NULL)
			return false;  // No Match
						   // Make Sure That Match Is At The Start Of The String Or That
						   // The Previous Char Is A Space, Or Else We Could Accidentally
						   // Match "wglFunkywglExtension" With "wglExtension"
						   // Also, Make Sure That The Following Character Is Space Or NULL
						   // Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return true; // Match
	}
}


int WGLinitMultisample(HWND hWnd, PIXELFORMATDESCRIPTOR pfd, int msaaSamples)
{
	// See If The String Exists In WGL!
	if (!WGLisExtensionSupported("WGL_ARB_multisample"))
	{
		return 0;
	}

	// check also framebuffer multisample for postfx
	if (!GLisExtensionSupported("GL_EXT_framebuffer_multisample"))
	{
		return 0;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)
	{
		return 0;
	}

	// Get Our Current Device Context
	HDC hDC = GetDC(hWnd);

	int		pixelFormat;
	int		valid;
	UINT	numFormats;
	float	fAttributes[] = { 0,0 };

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	int iAttributes[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, pfd.cColorBits,
		WGL_ALPHA_BITS_ARB, pfd.cAlphaBits,
		WGL_DEPTH_BITS_ARB, pfd.cDepthBits,
		WGL_STENCIL_BITS_ARB, pfd.cStencilBits,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, msaaSamples,
		0, 0
	};

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);

	// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		return pixelFormat;
	}

	// Return The Valid Format
	return 0;
}

LRESULT CALLBACK WglWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WglGetAAPixelFormat(HINSTANCE hInstance, int colorBits, int depthBits, int stencilBits, int samples)
{
	// Register A Window Class
	static bool classRegistered = false;
	if (!classRegistered)
	{
		WNDCLASSEX windowClass;												// Window Class
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						// Make Sure Memory Is Cleared
		windowClass.cbSize = sizeof(WNDCLASSEX);					// Size Of The windowClass Structure
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
		windowClass.lpfnWndProc = WglWndProc;				// WindowProc Handles Messages
		windowClass.hInstance = hInstance;				// Set The Instance
		windowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);			// Class Background Brush Color
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		windowClass.lpszClassName = "Dummy GL Window";				// Sets The Applications Classname
		if (RegisterClassEx(&windowClass) == 0)							// Did Registering The Class Fail?
		{
			return 0;													// Return False (Failure)
		}
		classRegistered = true;
	}

	RECT windowRect = { 0, 0, 100, 100 };	// Define Our Window Coordinates

											// Create The OpenGL Window
	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW,					// Extended Style
		TEXT("Dummy GL Window"),	// Class Name
		TEXT("Dummy GL Window"),					// Window Title
		WS_OVERLAPPEDWINDOW,							// Window Style
		0, 0,								// Window X,Y Position
		windowRect.right - windowRect.left,	// Window Width
		windowRect.bottom - windowRect.top,	// Window Height
		HWND_DESKTOP,						// Desktop Is Window's Parent
		0,									// No Menu
		hInstance, // Pass The Window Instance
		0);

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
		1,                                      //default version
		PFD_DRAW_TO_WINDOW |                    //window drawing support
		PFD_SUPPORT_OPENGL |                    //opengl support
		PFD_DOUBLEBUFFER,                       //double buffering support
		PFD_TYPE_RGBA,                          //RGBA color mode
		colorBits,                              //32 bit color mode
		0, 0, 0, 0, 0, 0,                       //ignore color bits
		0,                                      //no alpha buffer
		0,                                      //ignore shift bit
		0,                                      //no accumulation buffer
		0, 0, 0, 0,                             //ignore accumulation bits
		depthBits,                              //16 bit z-buffer size
		stencilBits,                            //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0
	};                              //layer masks ignored

	HDC hDC = GetDC(hWnd);  //get current windows device context

							/*      Choose best matching format*/
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (pixelFormat <= 0)
	{
		return false;
	}

	/*      Set the pixel format to the device context*/
	BOOL res = SetPixelFormat(hDC, pixelFormat, &pfd);
	if (res == FALSE)
	{
		return false;
	}

	HGLRC hRC = wglCreateContext(hDC);
	if (!hRC)
	{
		return false;
	}

	if (wglMakeCurrent(hDC, hRC) == FALSE)
	{
		wglDeleteContext(hRC);
		return false;
	}

	while (samples >= 2)
	{
		int format = WGLinitMultisample(hWnd, pfd, samples);
		if (format > 0)
		{
			wglDeleteContext(hRC);
			DestroyWindow(hWnd);
			return format;
		}
		samples >>= 1;
	}

	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return 0;
}

WGLDisplay::WGLDisplay(void* window, WGLDisplay* parent, bool moveWindow)
	: m_context(0)
	, m_moveWindow(moveWindow)
	, m_state(State::NOT_READY)
	, m_orientation(Orientation::O_0)
	, m_isLocked(false)
	, m_window(window)
	, m_dc(0)
	, m_profileGLES20(false)
{
	PASSERT2(!parent || parent->GetClassId() == GetClassId(), "Bad parent display. Must be the same class");
	m_parent = parent;
}

WGLDisplay::~WGLDisplay()
{
	wglMakeCurrent(NULL, NULL);
	if (m_context)
		wglDeleteContext((HGLRC)m_context);

	//delete m_renderTarget;
}

void* WGLDisplay::CreateSharedContext()
{
	if (GetState() != State::READY)
		return 0;

	PASSERT(m_context != 0);//check primary context has been previously created

	HGLRC context = (HGLRC)CreateCompatibleContext((HDC)m_dc, (HGLRC)m_context);
	return context;
}

bool WGLDisplay::SetThreadContext(void* context)
{
	if (GetState() != State::READY)
	{
		return false;
	}

	if (!wglMakeCurrent((HDC)m_dc, (HGLRC)context))
	{
		return false;
	}

	if (context)
	{
		return gles::setContext(context);
	}

	return true;
}



bool WGLDisplay::Init(const DisplayMode& mode, const DisplayMode& postFXMode)
{
	PASSERT2(!m_isLocked, "Cannot do this while the display is locked");
	if (m_isLocked)
		return false;

	DisplayMode newMode = mode;
	DisplayMode newPostFXMode = postFXMode;

	if (newMode.size.x == 0)
		newMode.size.x = GetNativeSize().x;
	if (newMode.size.y == 0)
		newMode.size.y = GetNativeSize().y;

	if (newPostFXMode.size.x == 0)
		newPostFXMode.size.x = newMode.size.x;
	if (newPostFXMode.size.y == 0)
		newPostFXMode.size.y = newMode.size.y;

	//sanity checks
	if (newMode.color == DisplayMode::Color::NONE ||
		newMode.size.x == 0 || newMode.size.y == 0)
		return false;

	u32 width = newMode.size.x;
	u32 height = newMode.size.y;

	if (m_moveWindow)
	{
		RECT rcClient, rcWindow;
		POINT ptDiff;
		GetClientRect((HWND)m_window, &rcClient);
		GetWindowRect((HWND)m_window, &rcWindow);
		ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
		ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
		MoveWindow((HWND)m_window, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, TRUE);
	}

	bool reinit = false; //a full reinit or just a resolution change?
	if (
		newMode.aa != m_mode.aa ||
		newMode.color != m_mode.color ||
		newMode.depth != m_mode.depth ||
		newMode.stencil != m_mode.stencil ||
		newPostFXMode.aa != m_postFXMode.aa ||
		newPostFXMode.color != m_postFXMode.color ||
		newPostFXMode.depth != m_postFXMode.depth ||
		newPostFXMode.stencil != m_postFXMode.stencil
		)
		reinit = true;

	if (m_pfxRenderTargets[0])
		m_pfxRenderTargets[0]->Destroy();
	if (m_pfxRenderTargets[1])
		m_pfxRenderTargets[1]->Destroy();
	if (m_pfxRenderTargets[2])
		m_pfxRenderTargets[2]->Destroy();

	if (reinit)
	{
		u32		pixelFormat = 0;			// Holds The Results After Searching For A Match

		u32 colorDepth = newMode.color == DisplayMode::Color::FULL ? 24 : 16;
		u32 depthDepth = 0;
		if (newMode.depth == DisplayMode::Depth::FULL)
			depthDepth = 24;
		else if (newMode.depth == DisplayMode::Depth::HALF)
			depthDepth = 16;
		else if (newMode.depth == DisplayMode::Depth::NONE)
			depthDepth = 0;

		u32 stencilDepth = 0;
		if (newMode.stencil == DisplayMode::Stencil::FULL)
			stencilDepth = 8;
		else if (newMode.stencil == DisplayMode::Stencil::BOOL)
			stencilDepth = 1;
		else if (newMode.stencil == DisplayMode::Stencil::NONE)
			stencilDepth = 0;

		u32 samples = 1;
		if (newMode.aa == DisplayMode::AA::X2)
			samples = 2;
		else if (newMode.aa == DisplayMode::AA::X4)
			samples = 4;
		else if (newMode.aa == DisplayMode::AA::X8)
			samples = 8;
		else if (newMode.aa == DisplayMode::AA::X16)
			samples = 16;

		static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			colorDepth,									// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			depthDepth,									// 16Bit Z-Buffer (Depth Buffer)  
			stencilDepth,								// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		wglMakeCurrent(NULL, NULL);
		if (m_context)
			wglDeleteContext((HGLRC)m_context);
		m_context = 0;

		if (!(m_dc = GetDC((HWND)m_window)))							// Did We Get A Device Context?
		{
			MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			goto ERR;
		}

		// check multisampling capabilities
		if (samples >= 2)
		{
			pixelFormat = WglGetAAPixelFormat(GetModuleHandle(0), colorDepth, depthDepth, stencilDepth, samples);
			if (pixelFormat == 0)
			{
				// multisampling not supported
				goto ERR;
			}
		}
		if (pixelFormat == 0)
		{
			if (!(pixelFormat = ChoosePixelFormat((HDC)m_dc, &pfd)))	// Did Windows Find A Matching Pixel Format?
			{
				MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
				goto ERR;
			}
		}

		if (!SetPixelFormat((HDC)m_dc, pixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
		{
			MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			goto ERR;
		}

		// try to create a default context to check if the pixel format is valid
		m_context = wglCreateContext((HDC)m_dc);
		if (m_context != 0 && wglMakeCurrent((HDC)m_dc, (HGLRC)m_context))
		{
			// create best compatible context
			HGLRC parentContext = (m_parent != 0) ? (HGLRC)m_parent->GetContext() : 0;
			HGLRC newContext = (HGLRC)CreateCompatibleContext((HDC)m_dc, parentContext);
			if (newContext != 0)
			{
				// enable new context
				wglMakeCurrent((HDC)m_dc, newContext);
				// delete the previous context
				wglDeleteContext((HGLRC)m_context);
				m_context = newContext;
			}
		}
		else
		{
			if (m_context != 0)
			{
				wglDeleteContext((HGLRC)m_context);
				m_context = 0;
			}
		}

		if (m_context == 0)
		{
			MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			goto ERR;
		}

		const char *extensions = (const char*)glGetString(GL_EXTENSIONS);
		if (strstr(extensions, "WGL_EXT_swap_control") != 0)
		{
			typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
			PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

			//vsync off
			if (wglSwapIntervalEXT)
			{
#if defined(JET_WINDOWS_USE_60FPS_VSYNC)
				wglSwapIntervalEXT(1);
#else
				wglSwapIntervalEXT(0);
#endif
			}
		}
	}

	Driver* driver = System::GetDriver();
	PASSERT(driver);

	//defines to allow compilation of GLES SL.
	if (m_profileGLES20 == false)
	{
		driver->AddShaderDefine("highp");
		driver->AddShaderDefine("mediump");
		driver->AddShaderDefine("lowp");
	}

	m_mode = newMode;
	m_postFXMode = newPostFXMode;
	m_size = newMode.size;

	//build the default render target
	if (reinit)
	{
		u32 fboId = 0;
		glGetIntegerv(gles::iGL_FRAMEBUFFER_BINDING, (GLint*)&fboId);

		GLES20RenderTargetWrapper* rt = new GLES20RenderTargetWrapper(fboId);
		rt->Create("default", core::GetRTFormatString(m_mode), m_size);
		rt->SetHasColorTarget(m_mode.color != DisplayMode::Color::NONE);
		rt->SetHasDepthTarget(m_mode.depth != DisplayMode::Depth::NONE);
		rt->SetHasStencilTarget(m_mode.stencil != DisplayMode::Stencil::NONE);

		m_renderTarget = RenderTargetPtr(rt);
		//System::GetDriver()->AddRenderTarget(m_renderTarget);
	}

	((GLES20RenderTargetWrapper*)m_renderTarget.get())->SetAllocatedSize(m_size);
	((GLES20RenderTargetWrapper*)m_renderTarget.get())->SetSize(m_size);

	SetOrientation(m_orientation);

	m_state = State::READY;

	if (reinit)
		System::GetDriver()->Init(this);

	return true;

ERR:
	wglMakeCurrent(NULL, NULL);
	if (m_context)
		wglDeleteContext((HGLRC)m_context);
	m_context = 0;

	return false;
}

void WGLDisplay::Cleanup()
{
	m_renderTarget.reset();

	if (m_pfxRenderTargets[0])
		m_pfxRenderTargets[0].reset();
	if (m_pfxRenderTargets[1])
		m_pfxRenderTargets[1].reset();
	if (m_pfxRenderTargets[2])
		m_pfxRenderTargets[2].reset();
}

bool WGLDisplay::MakeCurrent()
{
	if (GetState() == State::READY)
	{
		PROFILE_BLOCK("WglMakeCurrent");
		return wglMakeCurrent((HDC)m_dc, (HGLRC)m_context) ? true : false;
	}
	return false;
}


void WGLDisplay::Swap()
{
	if (GetState() == State::READY)
	{
		MakeCurrent();
		System::GetDriver()->FinishAllRendering();
#ifdef JET_USE_DEBUGGER
		if (dbg::Debugger::GetInstance()->IsEliminateSwap())
		{
			return;
		}
#endif
		::SwapBuffers((HDC)m_dc);
	}
}

void WGLDisplay::SetOrientation(Orientation orientation)
{
	if (GetState() == State::READY)
	{
		m_orientation = orientation;

		switch (m_orientation.value())
		{
		case Orientation::O_90:
		case Orientation::O_270:
			m_size = math::vec2u(m_mode.size.y, m_mode.size.x);
			break;
		default:
			m_size = m_mode.size;
			break;
		}

		((GLES20RenderTargetWrapper*)m_renderTarget.get())->SetSize(m_size);
	}
}

String WGLDisplay::GetRTFormat() const
{
	return core::GetRTFormatString(m_mode);
}

RenderTargetPtr WGLDisplay::CreateCompatibleRenderTarget() const
{
	if (GetState() != State::READY)
		return RenderTargetPtr();

	String format = GetRTFormat();
	RenderTargetPtr rt = RenderTarget::New();
	rt->Create("", format, GetSize());
	return rt;
}

RenderTargetPtr WGLDisplay::GetPostEffectRenderTarget(u32 idx) const
{
	if (idx >= 3 || GetState() != State::READY)
	{
		return RenderTargetPtr();
	}

	return m_pfxRenderTargets[idx];
}

const RenderTargetPtr& WGLDisplay::CreatePostEffectRenderTarget(u32 idx, u32 aaSamples) const
{
	if (idx >= 3 || GetState() != State::READY)
	{
		m_pfxRenderTargets[0] = RenderTargetPtr();
		return m_pfxRenderTargets[0];
	}

	//return it from the shared display if any, but only if they are the same size and format
	//Orientation doesn't matter for render targets, so ignore it even if it's different
	if (m_parent)
	{
		const DisplayMode& pmode = m_parent->GetMode();
		if (m_parent &&
			m_parent->GetSize() == GetSize() &&
			pmode.color == m_mode.color &&
			pmode.depth == m_mode.depth &&
			pmode.stencil == m_mode.stencil &&
			pmode.aa == m_mode.aa)
			return m_parent->CreatePostEffectRenderTarget(idx, aaSamples);
	}

	// check postfx aa mode
	if (idx == 2 && m_pfxRenderTargets[idx].get() != 0 && m_pfxRenderTargets[idx]->IsValid())
	{
		PASSERT(aaSamples > 0);
		bool destroyRT = false;
		String rtFormat = m_pfxRenderTargets[idx]->GetFormat();

		if (aaSamples == 2)   destroyRT = (rtFormat.find("A2") == String::npos);
		else if (aaSamples == 4)   destroyRT = (rtFormat.find("A4") == String::npos);
		else if (aaSamples == 8)   destroyRT = (rtFormat.find("A8") == String::npos);
		else if (aaSamples == 16)  destroyRT = (rtFormat.find("A16") == String::npos);
		else
		{
			PASSERT(0);
		}

		if (destroyRT)
		{
			m_pfxRenderTargets[idx]->Destroy();
			m_pfxRenderTargets[idx] = RenderTargetPtr();
		}
	}

	if (!m_pfxRenderTargets[idx])
	{
		m_pfxRenderTargets[idx] = RenderTarget::New();
	}

	if (!m_pfxRenderTargets[idx]->IsValid())
	{
		String name = String::Format("effect%d", idx);
		DisplayMode dmode = m_postFXMode;

		if (idx != 2)
		{
			dmode.aa = DisplayMode::AA::NONE;
		}
		else
		{
			if (aaSamples == 0) dmode.aa = DisplayMode::AA::NONE;
			else if (aaSamples == 2) dmode.aa = DisplayMode::AA::X2;
			else if (aaSamples == 4) dmode.aa = DisplayMode::AA::X4;
			else if (aaSamples == 8) dmode.aa = DisplayMode::AA::X8;
			else if (aaSamples == 16) dmode.aa = DisplayMode::AA::X16;
			else
			{
				PASSERT(0);
			}
		}

		//the second PFX rt doesn't need depth/stencil, as it will hold only a quad
		if (idx == 1)
		{
			dmode.depth = DisplayMode::Depth::NONE;
			dmode.stencil = DisplayMode::Stencil::NONE;
		}

		m_pfxRenderTargets[idx]->Create(name, core::GetRTFormatString(dmode), GetSize());
	}


	return m_pfxRenderTargets[idx];
}

math::vec2u WGLDisplay::GetNativeSize() const
{
	return math::vec2u(960, 640);
}

bool WGLDisplay::IsProfileGLES20() const
{
	return m_profileGLES20;
}

void* WGLDisplay::CreateCompatibleContext(void* dc, void* shareContext)
{
	// wglGetProcAddress requires a valid context in order to query function pointers{
	HGLRC context = 0;

	if (m_profileGLES20)
	{
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (wglCreateContextAttribsARB != 0)
		{
			int attribList[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
				WGL_CONTEXT_MINOR_VERSION_ARB, 0,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_ES2_PROFILE_BIT_EXT,
				0 };
			context = wglCreateContextAttribsARB((HDC)dc, (HGLRC)shareContext, attribList);
			m_profileGLES20 = (context != 0);
		}
	}

#ifndef NDEBUG
	// try to create a context with debug enabled
	if (context == nullptr)
	{
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (wglCreateContextAttribsARB != 0)
		{
			int attribList[] = {
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
				0 };
			context = wglCreateContextAttribsARB((HDC)dc, (HGLRC)shareContext, attribList);
		}
	}
#endif

	// try to create a default context
	if (context == nullptr)
	{
		if (!(context = wglCreateContext((HDC)dc)))
		{
			return 0;
		}

		PASSERT(context);
		//wglShareLists(context, (HGLRC)m_context)// wrong, SECOND PARAMETER MUST BE A EMPTY CONTEXT
		if (
			shareContext != 0 &&
			!wglShareLists((HGLRC)shareContext, context)
			)
		{
			wglDeleteContext(context);
			return 0;
		}
	}
	return context;
}

void WGLDisplay::Disconnect()
{
	if (m_state != State::DISCONNECTED)
	{
		m_state = State::DISCONNECTED;
		if (m_pfxRenderTargets[0])
		{
			m_pfxRenderTargets[0].reset();
		}
		if (m_pfxRenderTargets[1])
		{
			m_pfxRenderTargets[1].reset();
		}
		if (m_pfxRenderTargets[2])
		{
			m_pfxRenderTargets[2].reset();
		}

		wglMakeCurrent(NULL, NULL);
		if (m_context)
		{
			wglDeleteContext((HGLRC)m_context);
		}
		m_context = 0;
	}
}

#endif