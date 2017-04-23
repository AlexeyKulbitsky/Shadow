#ifndef SHADOW_DEPTH_STENCIL_STATE_INCLUDE
#define SHADOW_DEPTH_STENCIL_STATE_INCLUDE

#include "../Globals.h"

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

		////////////////////////////////////////////////////////////

		class SHADOW_API DepthStencilState
		{
		public:
			DepthStencilState();

			// Depth part
			bool				enableDepthTest = true;
			CompareFunction		depthCompareFunction = COMP_FUNC_LESS_OR_EQUAL;
			WriteMask			depthWriteMask = WM_ALL;

			// Stencil part
			bool				enableStencilTest = false;
			u8					stencilReadMask = 0xFF;
			u8					stencilWriteMask = 0xFF;
			Face				frontFace;
			Face				backFace;
			u32					reference = 0;

			void Load(const pugi::xml_node &node);
		};

		////////////////////////////////////////////////////////////

		inline DepthStencilState::DepthStencilState()
			: enableDepthTest(true)
			, depthCompareFunction(COMP_FUNC_LESS_OR_EQUAL)
			, depthWriteMask(WM_ALL)
			, enableStencilTest(false)
			, stencilReadMask(0xFF)
			, stencilWriteMask(0xFF)
			, reference(0)
		{

		}
	}
}

#endif