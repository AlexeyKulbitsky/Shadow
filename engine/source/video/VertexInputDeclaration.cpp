#include "video/VertexInputDeclaration.h"
#include "video/Managers/RenderStateManager.h"

namespace sh
{

namespace video
{
    void VertexInputDeclaration::Init() 
    {
    }

    VertexInputDeclarationPtr VertexInputDeclaration::Clone() 
    { 
        return nullptr; 
    }

    void VertexInputDeclaration::Assemble(VertexDeclaration&) 
    {
    }

    void VertexInputDeclaration::SetShaderProgram(ShaderProgram*) 
    {
    }

    uint32_t VertexInputDeclaration::GetAttributesCount() const 
    { 
        return 0U;                                                                                               
    }

    VertexInputDeclarationPtr VertexInputDeclaration::Create()
    {
        return RenderStateManager::GetInstance()->CreateVertexDeclaration();
    }

} // video

} // sh