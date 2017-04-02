#ifndef SHADOW_GLES20_RENDER_PIPELINE_INCLUDE
#define SHADOW_GLES20_RENDER_PIPELINE_INCLUDE

#include "../RenderPipeline.h"
#include "GLES20Common.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderPipeline : public RenderPipeline
		{
		public:
			GLES20RenderPipeline();
			virtual ~GLES20RenderPipeline();

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Unload() override;
			
			virtual const ShaderProgramPtr& GetShaderProgram() const override;
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const override;
			virtual const UniformBufferPtr& GetUniformBuffer() const override;
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const override;

			GLuint GetProgramID() const { return m_programID; }

		private:
			void LoadConstants(const pugi::xml_node &node);
			void LoadShaders(const pugi::xml_node &node);

		protected:			
			ShaderProgramPtr m_shaderProgram;
			VertexInputDeclarationPtr m_vertexInputDeclaration;
			UniformBufferPtr m_uniformBuffer;
			UniformBufferPtr m_transformUniformBuffer;

			GLuint m_programID;
		};
	}
}

#endif