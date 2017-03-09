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

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Unload() override;
			
			virtual const ShaderProgramPtr& GetShaderProgram() const override;
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const override;
			virtual const UniformBufferPtr& GetUniformBuffer() const override;
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const override;

		protected:			
			ShaderProgramPtr m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBufferPtr m_uniformBuffer;
			UniformBufferPtr m_transformUniformBuffer;

			
		};
	}
}

#endif