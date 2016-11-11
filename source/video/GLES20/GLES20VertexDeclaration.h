#ifndef SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE
#define SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE

#include "../GLContext/EGLContextManager.h"
#include "../VertexDeclaration.h"
#include <vector>

namespace sh
{
	namespace video
	{
		struct GLES20VertexAttribute
		{
			GLuint index;
			GLint size;
			GLenum type;
			const GLvoid* pointer;

			AttributeSemantic semantic;
			std::string name;
		};

		class GLES20VertexDeclaration
		{
		public:
			void AddAttribute(const GLES20VertexAttribute& attribute)
			{
				attributes.push_back(attribute);
			}

			void Assemble(VertexDeclaration& declatarion)
			{
				for (size_t i = 0; i < attributes.size(); ++i)
				{
					Attribute* attr = declatarion.GetAttribute(attributes[i].semantic);
					const GLvoid* tempPointer = (const GLvoid*)(attr->offset);
					attributes[i].pointer = tempPointer;
				}
				stride = declatarion.GetStride();
			}

			const std::vector<GLES20VertexAttribute>& GetAttributes() const { return attributes; }
			std::vector<GLES20VertexAttribute>& GetAttributes() { return attributes; }

			GLsizei GetStride() const { return stride; }


			GLES20VertexDeclaration& operator=(const GLES20VertexDeclaration& other)
			{
				attributes = other.attributes;
				stride = other.stride;
				return *this;
			}

		private:
			std::vector<GLES20VertexAttribute> attributes;
			GLsizei stride = 0;
		};
	}
}

#endif