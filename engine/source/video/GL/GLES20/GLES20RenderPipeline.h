#ifndef SHADOW_GLES20_RENDER_PIPELINE_INCLUDE
#define SHADOW_GLES20_RENDER_PIPELINE_INCLUDE

#include "video/RenderPipeline.h"
#include "video/GL/GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderPipeline : public RenderPipeline
		{
			friend class GLES20RenderStateManager;
		public:
			GLES20RenderPipeline();
			virtual ~GLES20RenderPipeline();
			
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const override;

            GLuint GetProgramID() const;

		private:
			void ParseAttributes();
			void ParseUniforms();
			GLES20RenderPipeline(const RenderPipelineDescription& description);

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