#include "GLES20CommandBuffer.h"
#include "../RenderCommand.h"
#include "../GLContext/EGLContextManager.h"
#include "GLES20VertexBuffer.h"
#include "GLES20IndexBuffer.h"
#include "GLES20RenderState.h"
#include "GLES20RenderCommand.h"
#include "GLES20ShaderProgram.h"

namespace sh
{
	namespace video
	{
		void GLES20CommandBuffer::Submit(RenderCommand* renderCommand)
		{
			GLES20RenderCommand* glesRenderCommand = static_cast<GLES20RenderCommand*>(renderCommand);
			GLES20VertexBuffer* vertexBuffer = glesRenderCommand->GetGLVertexBuffer();
			GLES20IndexBuffer* indexBuffer = glesRenderCommand->GetGLIndexBuffer();
			GLES20RenderState* renderState = glesRenderCommand->GetGLRenderState();
			GLES20ShaderProgram* shaderProgram = glesRenderCommand->GetGLShaderProgram();
			GLES20VertexDeclaration* vertexDeclaration = glesRenderCommand->GetGLVertexInputDeclaration();

			// Bind current shader
			shaderProgram->BindProgram();

			// Send uniforms to shader
			
		
			// Bind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetGLId());

			// Send attributes to shader
			for (auto attribute : vertexDeclaration->GetAttributes())
			{
				glEnableVertexAttribArray(attribute.index);
				glVertexAttribPointer(attribute.index, attribute.size, attribute.type, false, vertexDeclaration->GetStride(), attribute.pointer);
			}

			// Render with indices
			if (renderCommand->IsUseIndices())
			{
				// Bind indices buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetGLId());
				// Draw with indices
				glDrawElements(glesRenderCommand->GetGLTopology(), indexBuffer->GetIndicesCount(), indexBuffer->GetGLIndexType(), 0);
				// Unbind indices buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0U);
			}
			else
			{
				//glDrawArrays(glesRenderCommand->GetGLTopology(), 0, 3);
			}

			// Unbind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0U);

			// Unbind current shader
			shaderProgram->UnbindProgram();
		}
	}
}