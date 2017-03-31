#ifndef SHADOW_RENDER_BATCH_INCLUDE
#define SHADOW_RENDER_BATCH_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderBatch
		{
		public:
			/*
			This class is used to hold render commands with the same material
			*/
			RenderBatch() {}
			virtual ~RenderBatch() {}
			virtual void SetDepthStencilState(const DepthStencilStatePtr& renderState);
			virtual void SetRasterizationState(const RasterizationStatePtr& renderState);
			virtual void SetBlendingState(const BlendingStatePtr& renderState);
			virtual void SetShaderProgram(const ShaderProgramPtr& shaderProgram);
			virtual void SetUniformBuffer(const UniformBufferPtr& uniformBuffer);
			virtual void AddCommand(const RenderCommandPtr& renderCommand);
			virtual void RemoveCommand(const RenderCommandPtr& renderCommand);
			virtual void AddMesh(const scene::MeshPtr& mesh);
			virtual void Submit();
			virtual void Clear();

			void SetTechniqueName(const String& name) { m_techniqueName = name; }
			const String& GetTechniqueName() const { return m_techniqueName; }

		protected:
			String m_techniqueName;
			ShaderProgramPtr m_program;
			BlendingStatePtr m_blendingState;
			DepthStencilStatePtr m_depthStencilState;
			RasterizationStatePtr m_rasterizationState;
			UniformBufferPtr m_uniformBuffer;
			std::vector<RenderCommandPtr> m_commands;
			std::vector<scene::MeshPtr> m_meshes;
		};
	}
}

#endif