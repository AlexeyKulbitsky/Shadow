#ifndef SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE
#define SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE

#include "../GLContext/EGLContextManager.h"
#include "../VertexDeclaration.h"


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
			void AddAttribute(const GLES20VertexAttribute& attribute)
			{
				attributes.push_back(attribute);
				attributes[attributes.size() - 1].pointer = reinterpret_cast<const void*>(stride);
				stride += attribute.size * sizeof(float);
			}

			const std::vector<GLES20VertexAttribute>& GetAttributes() const { return attributes; }
			std::vector<GLES20VertexAttribute>& GetAttributes() { return attributes; }
			GLsizei GetStride() const { return stride; }

			GLES20VertexDeclaration& operator=(const GLES20VertexDeclaration& other);

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Init() override;
			virtual VertexInputDeclarationPtr Clone() override;
			virtual void Assemble(VertexDeclaration& declatarion) override;
			virtual u32 GetAttributesCount() const override { return attributes.size(); }
			virtual const InputAttribute& GetAttribute(u32 index) const { return attributes[index]; }

		private:
			std::vector<GLES20VertexAttribute> attributes;
			s32 stride = 0;
			GLES20ShaderProgram* m_shaderProgram;
		};
	}
}

#endif