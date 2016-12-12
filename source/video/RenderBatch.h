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

		class RenderBatch
		{
		public:
			/*
			This class is used to hold render commands with the same material
			*/
			RenderBatch() {}
			~RenderBatch() {}
			virtual void SetRenderState(RenderState* renderState);
			virtual void SetShaderProgram(ShaderProgram* shaderProgram);
			virtual void AddCommand(RenderCommand* renderCommand);
			virtual void RemoveCommand(RenderCommand* renderCommand);
			virtual void Submit();

			void SetTechniqueName(const String& name) { m_techniqueName = name; }
			const String& GetTechniqueName() const { return m_techniqueName; }

		protected:
			String m_techniqueName;
			ShaderProgram* m_program;
			RenderState* m_state;
			std::vector<RenderCommand*> m_commands;
		};
	}
}

#endif