#ifndef SHADOW_GLES20_RENDER_PIPELINE_INCLUDE
#define SHADOW_GLES20_RENDER_PIPELINE_INCLUDE

#include "../RenderPipeline.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderPipeline : public RenderPipeline
		{
		public:
			GLES20RenderPipeline();			
			virtual ~GLES20RenderPipeline();

			virtual RenderPipelinePtr Clone() override;

			virtual void Load(const pugi::xml_node &node);
			virtual void Unload();		
			
			virtual const ShaderProgramPtr& GetShaderProgram() const { return m_shaderProgram; }
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			virtual const UniformBufferPtr& GetUniformBuffer() const { return m_uniformBuffer; }
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const { return m_transformUniformBuffer; }
			
		protected:		
			ShaderProgramPtr m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBufferPtr m_uniformBuffer;
			UniformBufferPtr m_transformUniformBuffer;		
		};
	}
}

#endif