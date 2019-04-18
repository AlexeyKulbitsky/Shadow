#include "video/DepthStencilState.h"


namespace sh
{

namespace video
{
     
    DepthStencilState::DepthStencilState()
        : enableDepthTest(true)
        , depthCompareFunction(COMP_FUNC_LESS_OR_EQUAL)
        , depthWriteMask(WM_ALL)
        , enableStencilTest(false)
        , stencilReadMask(0xFF)
        , stencilWriteMask(0xFF)
        , reference(0)
    {

    }

} // video

} // sh