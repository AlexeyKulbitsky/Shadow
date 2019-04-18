#ifndef SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE
#define SHADOW_GLES20_VERTEX_DECLARATION_INCLUDE

#include "video/VertexInputDeclaration.h"
#include "video/GL/GLCommon.h"

namespace sh
{

namespace video
{

    struct GLES20VertexInputAttribute : public InputAttribute
    {
        uint32_t index;
        int32_t size;
        uint32_t type;
        const void* pointer;
        std::string name;
    };

    class GLES20VertexInputDeclaration : public VertexInputDeclaration
    {
    public:
        void AddAttribute(const GLES20VertexInputAttribute& attribute);

        const std::vector<GLES20VertexInputAttribute>& GetAttributes() const;
        std::vector<GLES20VertexInputAttribute>& GetAttributes();
        GLsizei GetStride() const;

        GLES20VertexInputDeclaration& operator=(const GLES20VertexInputDeclaration& other);

        void Init() override final;
        VertexInputDeclarationPtr Clone() override final;
        void Assemble(VertexDeclaration& declatarion) override final;
        uint32_t GetAttributesCount() const override final;
        const InputAttribute& GetAttribute(uint32_t index) const override final;

    private:
        std::vector<GLES20VertexInputAttribute> m_attributes;
        int32_t m_stride = 0;
    };

} // video

} // sh

#endif