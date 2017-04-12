#include "GLES20Driver.h"
#include "GLES20VertexDeclaration.h"
#include "GLES20IndexBuffer.h"
#include "GLES20VertexBuffer.h"
#include "GLES20Texture.h"
#include "GLES20RenderTarget.h"
#include "GLES20RenderPIpeline.h"
#include "GLES20RenderBatchManager.h"
#include "GLES20Common.h"
#include "GLES20Shader.h"
#include "GLES20Sampler.h"

#include "Managers/GLES20RenderStateManager.h"
#include "Managers/GLES20HardwareBufferManager.h"
#include "Managers/GLES20CommandBufferManager.h"

#include "../Material.h"
#include "../GLContext/EGLContextManager.h"
#include "../../scene/Mesh.h"
#include "../../scene/Model.h"
#include "../UniformBuffer.h"
#include "../DepthStencilState.h"
#include "../RasterizationState.h"
#include "../BlendingState.h"
#include "../GpuParams.h"

#include <sstream>
using namespace sh;
using namespace video;



GLES20Driver::GLES20Driver( EGLContextManager* contextManager )
	:m_contextManager( contextManager )
{
	GLES20RenderStateManager::CreateInstance();
	GLES20HardwareBufferManager::CreateInstance();
	CommandBufferManager::CreateInstance<GLES20CommandBufferManager>();
}

////////////////////////////////////////////////////////////////////////

GLES20Driver::~GLES20Driver()
{
	GLES20RenderStateManager::DestroyInstance();
	GLES20HardwareBufferManager::DestroyInstance();
	CommandBufferManager::DestroyInstance();
}

////////////////////////////////////////////////////////////////////////

const String& GLES20Driver::GetApiName() const
{
	static const String s_apiName = "GLES20";
	return s_apiName;
}

////////////////////////////////////////////////////////////////////////

bool GLES20Driver::Init()
{
	InitGlobalUniforms();

	glEnable( GL_DEPTH_TEST );
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );


	String vendor = (const char*)glGetString( GL_VENDOR );
	printf( "Vendor: %s\n", vendor.c_str() );
	String renderer = (const char*)glGetString( GL_RENDERER );
	printf( "Renderer: %s\n", renderer.c_str() );
	String version = (const char*)glGetString( GL_VERSION );
	printf( "Version: %s\n", version.c_str() );
	String shadingLanguage = (const char*)glGetString( GL_SHADING_LANGUAGE_VERSION );
	printf( "Shading language: %s\n", shadingLanguage.c_str() );

	// Extensions
	String extensions = (const char*)glGetString( GL_EXTENSIONS );
	String buf;
	std::stringstream ss( extensions );
	std::vector<String> extensionsList;
	while( ss >> buf )
		extensionsList.push_back( buf );
	printf( "Enabled extensions:\n" );
	for( auto ext : extensionsList )
	{
		printf( "%s\n", ext.c_str() );
	}

	GLint value;
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &value );
	m_maxTextureUnits = (u32)value;

	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &value );
	m_maxTextureSize = (u32)value;

	glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, &value );
	m_maxCubeTextureSize = (u32)value;

	int depthBits;
	glGetIntegerv( GL_DEPTH_BITS, &depthBits );


	glGetIntegerv( GL_NUM_COMPRESSED_TEXTURE_FORMATS, &value );
	GLint *valuePtr = new GLint[value];
	glGetIntegerv( GL_COMPRESSED_TEXTURE_FORMATS, valuePtr );
	for( int i = 0; i < value; ++i )
	{
		TextureFormat format = GLES20Texture::GetFormatFromGL( valuePtr[i] );
	}
	delete[] valuePtr;


	return false;
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::BeginRendering()
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::EndRendering()
{
	glFlush();
	m_contextManager->SwapBuffers();
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetViewport( u32 x, u32 y, u32 width, u32 height )
{
	Driver::SetViewport( x, y, width, height );
	glViewport( (GLint)x, (GLint)y, (GLint)width, (GLint)height );
}


////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetDepthRange( f32 zMin, f32 zMax )
{
	Driver::SetDepthRange( zMin, zMax );
	glDepthRangef( zMin, zMax );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetClearColor( const math::Vector4f& clearColor )
{
	Driver::SetClearColor( clearColor );
	glClearColor( m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetClearDepth( f32 clearDepth )
{
	Driver::SetClearDepth( clearDepth );
	glClearDepthf( m_clearDepth );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetClearStencil( u32 clearStencil )
{
	Driver::SetClearStencil( clearStencil );
	glClearStencil( static_cast<GLint>( m_clearStencil ) );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearColorBuffer()
{
	glClear( GL_COLOR_BUFFER_BIT );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearDepthBuffer()
{
	glClear( GL_DEPTH_BUFFER_BIT );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearStencilBuffer()
{
	glClear( GL_STENCIL_BUFFER_BIT );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::ClearBuffers()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetDepthStencilState( const DepthStencilStatePtr& depthStencilState )
{
	if( depthStencilState->enableDepthTest )
	{
		glEnable( GL_DEPTH_TEST );
		//glDepthFunc(s_glCompareFunction[static_cast<size_t>(depthStencilState->depthCompareFunction)]);
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
	}
	//glDepthMask(s_glWriteMask[static_cast<size_t>(depthStencilState->depthWriteMask)]);

	// Stencil part
	if( depthStencilState->enableStencilTest )
	{
		glEnable( GL_STENCIL_TEST );
		glStencilFuncSeparate( GL_FRONT,
							   s_glCompareFunction[static_cast<size_t>( depthStencilState->frontFace.compare )],
							   depthStencilState->reference,
							   depthStencilState->stencilReadMask );
		glStencilMaskSeparate( GL_FRONT, depthStencilState->stencilWriteMask );
		glStencilOpSeparate( GL_FRONT,
							 s_glStencilOperation[static_cast<size_t>( depthStencilState->frontFace.fail )],
							 s_glStencilOperation[static_cast<size_t>( depthStencilState->frontFace.depthFail )],
							 s_glStencilOperation[static_cast<size_t>( depthStencilState->frontFace.pass )] );

		glStencilFuncSeparate( GL_BACK,
							   s_glCompareFunction[static_cast<size_t>( depthStencilState->backFace.compare )],
							   depthStencilState->reference,
							   depthStencilState->stencilReadMask );
		glStencilMaskSeparate( GL_BACK, depthStencilState->stencilWriteMask );
		glStencilOpSeparate( GL_BACK,
							 s_glStencilOperation[static_cast<size_t>( depthStencilState->backFace.fail )],
							 s_glStencilOperation[static_cast<size_t>( depthStencilState->backFace.depthFail )],
							 s_glStencilOperation[static_cast<size_t>( depthStencilState->backFace.pass )] );
	}
	else
	{
		glDisable( GL_STENCIL_TEST );
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetRasterizationState( const RasterizationStatePtr& rasterizationState )
{
	if( rasterizationState->cullFace != CullFace::CF_NONE )
	{
		glEnable( GL_CULL_FACE );
		glCullFace( s_glCullFace[static_cast<size_t>( rasterizationState->cullFace )] );
		glFrontFace( s_glFrontFace[static_cast<size_t>( rasterizationState->frontFace )] );
	}
	else
	{
		glDisable( GL_CULL_FACE );
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetBlendingState( const BlendingStatePtr& blendingState )
{
	if( blendingState->enabled )
	{
		glEnable( GL_BLEND );
		glBlendFunc( s_glBlendFactor[static_cast<size_t>( blendingState->srcAlpha )],
					 s_glBlendFactor[static_cast<size_t>( blendingState->dstAlpha )] );
		glBlendEquation( s_glBlendOperation[static_cast<size_t>( blendingState->operationAlpha )] );

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
		glDisable( GL_BLEND );
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetRenderPipeline( const RenderPipelinePtr& pipeline, const CommandBufferPtr& )
{
	GLES20RenderPipeline* glPipeline = static_cast<GLES20RenderPipeline*>( pipeline.get() );
	SetBlendingState( glPipeline->GetBlendingState() );
	SetRasterizationState( glPipeline->GetRasterizationState() );
	SetDepthStencilState( glPipeline->GetDepthStencilState() );
	glUseProgram( glPipeline->GetProgramID() );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetComputePipeline()
{

}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetGpuParams( const GpuParamsPtr& params, const CommandBufferPtr& )
{
	const u8* data = params->GetData();

	for( size_t i = 0; i < 2U; ++i )
	{
		ShaderType shaderType = static_cast<ShaderType>( i );

		const SPtr<GpuParamsDescription>& desc = params->GetDescription( shaderType );
		if( !desc )
			continue;

		for( const auto& param : desc->params )
		{
			const GLfloat* dataPtr = reinterpret_cast<const float*>( data + param.second.offset );

			switch( param.second.type )
			{
				case GPDT_FLOAT1:
				{
					glUniform1fv( param.second.location, 1, dataPtr );
				}
				break;
				case GPDT_FLOAT2:
				{
					glUniform2fv( param.second.location, 1, dataPtr );
				}
				break;
				case GPDT_FLOAT3:
				{
					glUniform3fv( param.second.location, 1, dataPtr );
				}
				break;
				case GPDT_MATRIX3:
				{
					glUniformMatrix3fv( param.second.location, 1, GL_FALSE, dataPtr );
				}
				break;
				case GPDT_MATRIX4:
				{
					glUniformMatrix4fv( param.second.location, 1, GL_FALSE, dataPtr );
				}
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetTopology( Topology topology, const CommandBufferPtr& )
{
	m_currentTopology = s_glTopology[static_cast<size_t>( topology )];
}

void GLES20Driver::SetAutoUniformsBatch( const UniformsBatchPtr& batch, const CommandBufferPtr& )
{
	for( auto uniform : batch->m_uniforms )
	{
		uniform->Upload();
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetVertexDeclaration( const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& )
{
	GLES20VertexDeclaration* vertexDeclaration = static_cast<GLES20VertexDeclaration*>( declaration.get() );
	for( auto attribute : vertexDeclaration->GetAttributes() )
	{
		glEnableVertexAttribArray( attribute.index );
		glVertexAttribPointer( attribute.index, attribute.size, attribute.type, false, vertexDeclaration->GetStride(), attribute.pointer );
	}
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetVertexBuffer( const VertexBufferPtr& buffer, const CommandBufferPtr& )
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	GLES20VertexBuffer* glBuffer = static_cast<GLES20VertexBuffer*>( buffer.get() );
	m_currentVertexBuffer = glBuffer->GetGLId();
	glBindBuffer( GL_ARRAY_BUFFER, m_currentVertexBuffer );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::SetIndexBuffer( const IndexBufferPtr& buffer, const CommandBufferPtr& )
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	GLES20IndexBuffer* glBuffer = static_cast<GLES20IndexBuffer*>( buffer.get() );
	m_currentIndexBuffer = glBuffer->GetGLId();
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_currentIndexBuffer );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::Draw( u32 offset, u32 verticesCount, u32 instancesCount, const CommandBufferPtr& )
{

}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::DrawIndexed( u32 offset, u32 indicesCount, u32 instancesCount, const CommandBufferPtr& )
{
	glDrawElements( GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void*)offset );
}

////////////////////////////////////////////////////////////////////////

void GLES20Driver::GetPixelData( u32 x, u32 y, u32 width, u32 height, u8* data )
{
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );
	glReadPixels( x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data );
}

////////////////////////////////////////////////////////////////////////

TexturePtr GLES20Driver::CreateTexture() const
{
	TexturePtr result = nullptr;
	result.reset( new GLES20Texture() );
	return result;
}

////////////////////////////////////////////////////////////////////////

TexturePtr GLES20Driver::CreateTexture( const TextureDescription& description ) const
{
	TexturePtr result = nullptr;
	result.reset( new GLES20Texture( description ) );
	return result;
}

////////////////////////////////////////////////////////////////////////

VertexInputDeclarationPtr GLES20Driver::CreateVertexInputDeclaration() const
{
	VertexInputDeclarationPtr result = nullptr;
	result.reset( new GLES20VertexDeclaration() );
	return result;
}

////////////////////////////////////////////////////////////////////////

RenderTargetPtr GLES20Driver::CreateRenderTarget() const
{
	RenderTargetPtr result = nullptr;
	result.reset( new GLES20RenderTarget() );
	return result;
}

////////////////////////////////////////////////////////////////////////

RenderPipelinePtr GLES20Driver::CreateRenderPipeline() const
{
	RenderPipelinePtr result = nullptr;
	result.reset( new GLES20RenderPipeline() );
	return result;
}

////////////////////////////////////////////////////////////////////////

RenderPipelinePtr GLES20Driver::CreateRenderPipeline( const RenderPipelineDescription& description ) const
{
	RenderPipelinePtr result = nullptr;
	result.reset( new GLES20RenderPipeline( description ) );
	return result;
}

////////////////////////////////////////////////////////////////////////

RenderBatchManagerPtr GLES20Driver::CreateRenderBatchManager() const
{
	RenderBatchManagerPtr result;
	result.reset( new GLES20RenderBatchManager() );
	return result;
}

////////////////////////////////////////////////////////////////////////

ShaderPtr GLES20Driver::CreateShader( const ShaderDescription& description ) const
{
	ShaderPtr result;
	result.reset( new GLES20Shader( description ) );
	return result;
}

////////////////////////////////////////////////////////////////////////

SamplerPtr GLES20Driver::CreateSampler( const SamplerDescription& description ) const
{
	SamplerPtr result;
	result.reset( new GLES20Sampler( description ) );
	return result;
}

////////////////////////////////////////////////////////////////////////
