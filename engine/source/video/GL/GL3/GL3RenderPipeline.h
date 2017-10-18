#ifndef SHADOW_GL3_RENDER_PIPELINE_INCLUDE
#define SHADOW_GL3_RENDER_PIPELINE_INCLUDE

#include "../../RenderPipeline.h"
#include "../GLCommon.h"

namespace sh
{
	namespace video
	{
		class GL3RenderPipeline : public RenderPipeline
		{
			//friend class GLES20Driver;
			friend class GLRenderStateManager;
		public:
			GL3RenderPipeline();
			virtual ~GL3RenderPipeline();

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Unload() override;
			
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const override;

			GLuint GetProgramID() const { return m_programID; }

		private:
			void LoadConstants(const pugi::xml_node &node);
			void LoadShaders(const pugi::xml_node &node);
			void ParseAttributes();
			void ParseUniforms();
			GL3RenderPipeline(const RenderPipelineDescription& description);

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