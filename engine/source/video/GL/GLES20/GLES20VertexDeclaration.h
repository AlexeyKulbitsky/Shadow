#ifndef SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE
#define SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE

#include "../../VertexDeclaration.h"
#include "../GLCommon.h"


namespace sh
{
	namespace video
	{
		class ShaderProgram;
		class GLES20ShaderProgram;

		struct GLES20VertexAttribute : public InputAttribute
		{
			u32 index;
			s32 size;
			u32 type;
			const void* pointer;

			//AttributeSemantic semantic;
			std::string name;
		};

		class GLES20VertexDeclaration : public VertexInputDeclaration
		{
		public:
			void AddAttribute(const GLES20VertexAttribute& attribute);

			const std::vector<GLES20VertexAttribute>& GetAttributes() const { return m_attributes; }
			std::vector<GLES20VertexAttribute>& GetAttributes() { return m_attributes; }
			GLsizei GetStride() const { return m_stride; }

			GLES20VertexDeclaration& operator=(const GLES20VertexDeclaration& other);

			virtual void Init() override;
			virtual VertexInputDeclarationPtr Clone() override;
			virtual void Assemble(VertexDeclaration& declatarion) override;
			virtual u32 GetAttributesCount() const override { return m_attributes.size(); }
			virtual const InputAttribute& GetAttribute(u32 index) const { return m_attributes[index]; }

		private:
			std::vector<GLES20VertexAttribute> m_attributes;
			s32 m_stride = 0;
		};
	}
}

#endif