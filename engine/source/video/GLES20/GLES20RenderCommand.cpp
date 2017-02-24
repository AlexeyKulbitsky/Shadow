#include "GLES20RenderCommand.h"

namespace sh
{
	namespace video
	{
		void GLES20RenderCommand::Init()
		{
			//GLES20VertexDeclaration* shaderInternalVertexDeclaration = m_shaderProgram->GetVertexDeclaration();
			//VertexDeclaration* vertexBufferInternalVertexDeclaration = m_vertexBuffer->GetVertexDeclaration();

			// Connect it with buffer declaration
			//m_inputDeclaration->Assemble(*vertexBufferInternalVertexDeclaration);
		}

		void GLES20RenderCommand::SetUseIndices(bool useIndices)
		{
			m_useIndices = useIndices;
		}

		void GLES20RenderCommand::SetVertexBuffer(VertexBuffer* vertexBuffer)
		{
			m_vertexBuffer = static_cast<GLES20VertexBuffer*>(vertexBuffer);
		}

		void GLES20RenderCommand::SetIndexBuffer(IndexBuffer* indexBuffer)
		{
			m_indexBuffer = static_cast<GLES20IndexBuffer*>(indexBuffer);
		}

		void GLES20RenderCommand::SetTopology(Topology topology)
		{
			m_topology = topology;
			switch (topology)
			{
			case Topology::LINE_LIST:
				m_glTopology = GL_LINES;
				break;
			case Topology::LINE_LOOP:
				m_glTopology = GL_LINE_LOOP;
				break;
			case Topology::LINE_STRIP:
				m_glTopology = GL_LINE_STRIP;
				break;
			case Topology::TRIANGLE_FAN:
				m_glTopology = GL_TRIANGLE_FAN;
				break;
			case Topology::TRIANGLE_LIST:
				m_glTopology = GL_TRIANGLES;
				break;
			case Topology::TRIANGLE_STRIP:
				m_glTopology = GL_TRIANGLE_STRIP;
				break;
			}
		}

		void GLES20RenderCommand::SetVertexInputDeclaration(VertexInputDeclaration* declaration)
		{
			m_inputDeclaration = static_cast<GLES20VertexDeclaration*>(declaration);
		}
	}
}