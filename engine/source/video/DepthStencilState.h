#ifndef SHADOW_DEPTH_STENCIL_STATE_INCLUDE
#define SHADOW_DEPTH_STENCIL_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
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

		////////////////////////////////////////////////////////////

		struct SHADOW_API Face
		{
			StencilOperation fail = StencilOperation::KEEP;
			StencilOperation depthFail = StencilOperation::KEEP;
			StencilOperation pass = StencilOperation::KEEP;
			CompareFunction compare = CompareFunction::ALWAYS;
		};

		////////////////////////////////////////////////////////////

		struct SHADOW_API DepthStencilState
		{
			DepthStencilState* Clone();

			// Depth part
			bool				enableDepthTest = true;
			CompareFunction		depthCompareFunction = CompareFunction::LESS_OR_EQUAL;
			WriteMask			depthWriteMask = WriteMask::ALL;

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

		inline DepthStencilState* DepthStencilState::Clone()
		{
			DepthStencilState* result = new DepthStencilState();
			(*result) = (*this);
			return result;
		}
	}
}

#endif