#ifndef SHADOW_GL3_VERTEX_DECLARATION_INCLUDE
#define SHADOW_GL3_VERTEX_DECLARATION_INCLUDE

#include "../../VertexDeclaration.h"
#include "../GLCommon.h"


namespace sh
{
	namespace video
	{
		class ShaderProgram;
		class GLES20ShaderProgram;

		struct GL3VertexAttribute : public InputAttribute
		{
			u32 index;
			s32 size;
			u32 type;
			const void* pointer;

			//AttributeSemantic semantic;
			std::string name;
		};

		class GL3VertexDeclaration : public VertexInputDeclaration
		{
		public:
			void AddAttribute(const GL3VertexAttribute& attribute);

			const std::vector<GL3VertexAttribute>& GetAttributes() const { return m_attributes; }
			std::vector<GL3VertexAttribute>& GetAttributes() { return m_attributes; }
			GLsizei GetStride() const { return m_stride; }

			GL3VertexDeclaration& operator=(const GL3VertexDeclaration& other);

			virtual void Init() override;
			virtual VertexInputDeclarationPtr Clone() override;
			virtual void Assemble(VertexDeclaration& declatarion) override;
			virtual u32 GetAttributesCount() const override { return m_attributes.size(); }
			virtual const InputAttribute& GetAttribute(u32 index) const { return m_attributes[index]; }

		private:
			std::vector<GL3VertexAttribute> m_attributes;
			s32 m_stride = 0;
		};
	}
}

#endif
