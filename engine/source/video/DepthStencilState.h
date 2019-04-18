#ifndef SHADOW_DEPTH_STENCIL_STATE_INCLUDE
#define SHADOW_DEPTH_STENCIL_STATE_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{
		
    struct SHADOW_API Face
    {
        StencilOperation fail = STENCIL_OP_KEEP;
        StencilOperation depthFail = STENCIL_OP_KEEP;
        StencilOperation pass = STENCIL_OP_KEEP;
        CompareFunction compare = COMP_FUNC_ALWAYS;
    };

    class SHADOW_API DepthStencilState
    {
    public:
        DepthStencilState();

        // Depth part
        bool enableDepthTest = true;
        CompareFunction depthCompareFunction = COMP_FUNC_LESS_OR_EQUAL;
        WriteMask depthWriteMask = WM_ALL;

        // Stencil part
        bool enableStencilTest = false;
        uint8_t stencilReadMask = 0xFF;
        uint8_t stencilWriteMask = 0xFF;
        Face frontFace;
        Face backFace;
        uint32_t reference = 0U;
    };

} // video

} // sh

#endif