#ifndef SHADOW_GLES20_RENDER_COMMAND_INCLUDE
#define SHADOW_GLES20_RENDER_COMMAND_INCLUDE

#include "../RenderCommand.h"
#include "GLES20VertexBuffer.h"
#include "GLES20IndexBuffer.h"
#include "GLES20RenderState.h"
#include "GLES20ShaderProgram.h"
#include "GLES20VertexDeclaration.h"
#include "GLES20UniformBuffer.h"

namespace sh
{
	namespace video
	{		
		class GLES20RenderCommand : public RenderCommand
		{
		public:
			virtual void Init() override;

			virtual void SetUseIndices(bool useIndices) override;
			virtual void SetVertexBuffer(VertexBuffer* vertexBuffer) override;
			virtual void SetIndexBuffer(IndexBuffer* indexBuffer) override;
			virtual void SetUniformBuffer(UniformBuffer* uniformBuffer) override;
			virtual void SetTopology(Topology topology) override;
			virtual void SetVertexInputDeclaration(VertexInputDeclaration* declaration) override;

			virtual bool IsUseIndices() const override { return m_useIndices; }
			virtual VertexBuffer* GetVertexBuffer() override { return m_vertexBuffer; }
			virtual IndexBuffer* GetIndexBuffer() override { return m_indexBuffer; }
			virtual UniformBuffer* GetUniformBuffer() override { return m_uniformBuffer; }
			virtual Topology GetTopology() const override { return m_topology; }
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const override { return m_inputDeclaration; }

			// GLES20 - specific interface
			GLES20VertexBuffer* GetGLVertexBuffer() { return m_vertexBuffer; }
			GLES20IndexBuffer* GetGLIndexBuffer() { return m_indexBuffer; }
			GLES20UniformBuffer* GetGLUniformBuffer() { return m_uniformBuffer; }
			GLES20VertexDeclaration* GetGLVertexInputDeclaration() { return m_inputDeclaration; }
			u32 GetGLTopology() const { return m_glTopology; }

		private:
			bool m_useIndices;
			GLES20VertexBuffer* m_vertexBuffer;
			GLES20IndexBuffer* m_indexBuffer;
			GLES20UniformBuffer* m_uniformBuffer;
			GLES20VertexDeclaration* m_inputDeclaration;
			Topology m_topology;
			u32 m_glTopology;
		};
	}
}

#endif