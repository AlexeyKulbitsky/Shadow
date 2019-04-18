#ifndef SHADOW_VERTEX_INPUT_DECLARATION_INCLUDE
#define SHADOW_VERTEX_INPUT_DECLARATION_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/VertexDeclaration.h"

namespace sh
{

namespace video
{

    struct SHADOW_API InputAttribute
    {
        AttributeSemantic semantic;
    };

    class SHADOW_API VertexInputDeclaration
    {
    public:
        virtual void Init();
        virtual VertexInputDeclarationPtr Clone();
        virtual void Assemble(VertexDeclaration& declaration);
        virtual void SetShaderProgram(ShaderProgram* shaderProgram);
        virtual uint32_t GetAttributesCount() const;
        virtual const InputAttribute& GetAttribute(uint32_t index) const = 0;

        static VertexInputDeclarationPtr Create();
    };

} // video

} // sh

#endif
