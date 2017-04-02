#ifndef SHADOW_DEPTH_STENCIL_STATE_INCLUDE
#define SHADOW_DEPTH_STENCIL_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		/*
		enum class SHADOW_API CompareFunction
		{
			LESS = 0,
			LESS_OR_EQUAL,
			EQUAL,
			GREATER_OR_EQUAL,
			GREATER,
			NOT_EQUAL,
			ALWAYS,
			NEVER,

			COUNT
		};

		static const char* const g_compareFunctionMap[] = 
		{
			"Less",
			"LessOrEqual",
			"Equal",
			"GreaterOrEqual",
			"Greater",
			"NotEqual",
			"Always",
			"Never"
		};

		////////////////////////////////////////////////////////////

		enum class SHADOW_API WriteMask
		{
			ZERO = 0,
			ALL,

			COUNT
		};

		static const char* const g_writeMaskMap[] = 
		{
			"Zero",
			"All"
		};

		////////////////////////////////////////////////////////////
		
		enum class SHADOW_API StencilOperation
		{
			KEEP = 0,
			CLEAR,
			REPLACE,
			INVERT,
			INCREMENT,
			DECREMENT,
			INCREMENT_WRAP,
			DECREMENT_WRAP,

			COUNT
		};

		static const char* const g_stencilOperationMap[] = 
		{
			"Keep",
			"Clear",
			"Replace",
			"Invert",
			"Increment",
			"Decrement",
			"IncrementWrap",
			"DecrementWrap"
		};
		*/
		////////////////////////////////////////////////////////////

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