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

			void Assemble(VertexDecalaration& declatarion)
			{
				for (auto attribute : attributes)
				{
					Attribute* attr = declatarion.GetAttribute(attribute.semantic);
					attribute.pointer = (const GLvoid*)(attr->offset);
				}
			}

			const std::vector<GLES20VertexAttribute>& GetAttributes() const { return attributes; }
			std::vector<GLES20VertexAttribute>& GetAttributes() { return attributes; }

			GLsizei GetStride() const { return stride; }

		//private:
			std::vector<GLES20VertexAttribute> attributes;
			GLsizei stride = 0;
		};
	}
}

#endif