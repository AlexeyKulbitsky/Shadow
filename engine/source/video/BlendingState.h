#ifndef SHADOW_BLENDING_STATE_INCLUDE
#define SHADOW_BLENDING_STATE_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{

    class SHADOW_API BlendingState
    {
    public:
        BlendingState();
        BlendingState(BlendCombination combination);
        BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha);
        BlendingState(BlendFactor src, BlendFactor dst);
        BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha, BlendOperation operation);
        BlendingState(BlendFactor src, BlendFactor dst, BlendOperation operation);
        BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha, BlendOperation operationColor, BlendOperation operationAlpha);
        BlendingState(BlendFactor src, BlendFactor dst, BlendOperation operationColor, BlendOperation operationAlpha);

        bool enabled = false;

        BlendFactor srcColor = BF_ONE;
        BlendFactor dstColor = BF_ZERO;
        BlendOperation operationColor = BO_ADD;

        BlendFactor srcAlpha = BF_ONE;
        BlendFactor dstAlpha = BF_ZERO;
        BlendOperation operationAlpha = BO_ADD;
    };

    

} // video

} // sh

#endif
