#ifndef SHADOW_RENDER_BATCH_INCLUDE
#define SHADOW_RENDER_BATCH_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderCommand;
		class RenderState;
		class ShaderProgram;
		struct DepthStencilState;
		struct RasterizationState;
		struct BlendingState;

		class RenderBatch
		{
		public:
			/*
			This class is used to hold render commands with the same material
			*/
			RenderBatch() {}
			~RenderBatch() {}
			virtual void SetDepthStencilState(DepthStencilState* renderState);
			virtual void SetRasterizationState(RasterizationState* renderState);
			virtual void SetBlendingState(BlendingState* renderState);
			virtual void SetShaderProgram(ShaderProgram* shaderProgram);
			virtual void AddCommand(RenderCommand* renderCommand);
			virtual void RemoveCommand(RenderCommand* renderCommand);
			virtual void Submit();

			void SetTechniqueName(const String& name) { m_techniqueName = name; }
			const String& GetTechniqueName() const { return m_techniqueName; }

		protected:
			String m_techniqueName;
			ShaderProgram* m_program;
			BlendingState* m_blendingState;
			DepthStencilState* m_depthStencilState;
			RasterizationState* m_rasterizationState;
			std::vector<RenderCommand*> m_commands;
		};
	}
}

#endif