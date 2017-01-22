#ifndef SHADOW_RENDER_STATE_INCLUDE
#define SHADOW_RENDER_STATE_INCLUDE

#include "../Globals.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"

namespace sh
{
	namespace video
	{
		
		enum class SHADOW_API BlendFactor
		{
			ZERO = 0,
			ONE,
			SRC_COLOR,
			INV_SRC_COLOR,
			DST_COLOR,
			INV_DST_COLOR,
			SRC_ALPHA,
			INV_SRC_ALPHA,
			DST_ALPHA,
			INV_DST_ALPHA,
			SRC_ALPHA_SATURATED,

			COUNT
		};

		static const char* const g_blendFactorMap[] = 
		{
			"Zero",
			"One",
			"SrcColor",
			"InvSrcColor",
			"DstColor",
			"InvDstColor",
			"SrcAlpha",
			"InvSrcAlpha",
			"DstAlpha",
			"InvDstAlpha",
			"SrcAlphaSturated"
		};

		////////////////////////////////////////////////////////////

		enum class SHADOW_API BlendOperation
		{
			ADD = 0,
			SUBSTRACT,
			REVERSE_SUBSTRACT,
			MIN,
			MAX,

			COUNT
		};

		static const char* const g_blendOperationMap[] = 
		{
			"Add",
			"Substract",
			"ReverseSubstract",
			"Min",
			"Max"
		};

		////////////////////////////////////////////////////////////

		enum class SHADOW_API BlendCombination
		{
			SOLID = 0,
			ALPHA,
			ADDITIVE,
			MULTIPLY,
			BURN,
			SCREEN,
			SUBSTRACTIVE,

			COUNT
		};

		static const char* const g_blendCombinationMap[] = 
		{
			"Solid",
			"Alpha",
			"Additive",
			"Multiply",
			"Burn",
			"Screen",
			"Substractive"
		};

		////////////////////////////////////////////////////////////////////////////////////

		
		struct SHADOW_API BlendingState
		{
			BlendingState();
			BlendingState(BlendCombination combination);
			BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha);
			BlendingState(BlendFactor src, BlendFactor dst);
			BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha, BlendOperation operation);
			BlendingState(BlendFactor src, BlendFactor dst, BlendOperation operation);
			BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha, BlendOperation operationColor, BlendOperation operationAlpha);
			BlendingState(BlendFactor src, BlendFactor dst, BlendOperation operationColor, BlendOperation operationAlpha);


			BlendFactor srcColor = BlendFactor::ONE;			
			BlendFactor dstColor = BlendFactor::ZERO;
			BlendOperation operationColor = BlendOperation::ADD;

			BlendFactor srcAlpha = BlendFactor::ONE;
			BlendFactor dstAlpha = BlendFactor::ZERO;
			BlendOperation operationAlpha = BlendOperation::ADD;
		};

		inline BlendingState::BlendingState()
			: srcColor(BlendFactor::ONE)
			, dstColor(BlendFactor::ZERO)
			, operationColor(BlendOperation::ADD)
			, srcAlpha(BlendFactor::ONE)			
			, dstAlpha(BlendFactor::ZERO)
			, operationAlpha(BlendOperation::ADD)
		{}

		inline BlendingState::BlendingState(BlendCombination combination)
		{
			switch (combination)
			{
				case BlendCombination::SOLID: srcColor = srcAlpha = BlendFactor::ONE; dstColor = dstAlpha = BlendFactor::ZERO; break;
				case BlendCombination::ALPHA: srcColor = srcAlpha = BlendFactor::SRC_ALPHA; dstColor = dstAlpha = BlendFactor::INV_SRC_ALPHA; break;
				case BlendCombination::ADDITIVE: srcColor = srcAlpha = BlendFactor::ONE; dstColor = dstAlpha = BlendFactor::ONE; break;
				case BlendCombination::MULTIPLY: srcColor = srcAlpha = BlendFactor::ZERO; dstColor = dstAlpha = BlendFactor::SRC_COLOR; break;
				case BlendCombination::BURN: srcColor = srcAlpha = BlendFactor::DST_COLOR; dstColor = dstAlpha = BlendFactor::ONE; break;
				case BlendCombination::SCREEN: srcColor = srcAlpha = BlendFactor::ONE; dstColor = dstAlpha = BlendFactor::INV_SRC_COLOR; break;
				case BlendCombination::SUBSTRACTIVE: srcColor = srcAlpha = BlendFactor::ZERO; dstColor = dstAlpha = BlendFactor::INV_SRC_COLOR; break;
				default: break;
			}
			operationColor = BlendOperation::ADD;
			operationAlpha = BlendOperation::ADD;
		}

		inline BlendingState::BlendingState(BlendFactor _srcColor, BlendFactor _srcAlpha, BlendFactor _dstColor, BlendFactor _dstAlpha)
			: srcColor(_srcColor)
			, dstColor(_dstColor)
			, operationColor(BlendOperation::ADD)
			, srcAlpha(_srcAlpha)			
			, dstAlpha(_dstAlpha)
			, operationAlpha(BlendOperation::ADD)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _src, BlendFactor _dst)
			: srcColor(_src)
			, dstColor(_dst)
			, operationColor(BlendOperation::ADD)
			, srcAlpha(_src)			
			, dstAlpha(_dst)
			, operationAlpha(BlendOperation::ADD)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _srcColor, BlendFactor _srcAlpha, BlendFactor _dstColor, BlendFactor _dstAlpha, BlendOperation _operation)
			: srcColor(_srcColor)
			, dstColor(_dstColor)
			, operationColor(_operation)
			, srcAlpha(_srcAlpha)			
			, dstAlpha(_dstAlpha)
			, operationAlpha(_operation)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _src, BlendFactor _dst, BlendOperation _operation)
			: srcColor(_src)
			, dstColor(_dst)
			, operationColor(_operation)
			, srcAlpha(_src)			
			, dstAlpha(_dst)
			, operationAlpha(_operation)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _srcColor, BlendFactor _srcAlpha, BlendFactor _dstColor, BlendFactor _dstAlpha, BlendOperation _operationColor, BlendOperation _operationAlpha)
			: srcColor(_srcColor)
			, dstColor(_dstColor)
			, operationColor(_operationColor)
			, srcAlpha(_srcAlpha)
			, dstAlpha(_dstAlpha)
			, operationAlpha(_operationAlpha)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _src, BlendFactor _dst, BlendOperation _operationColor, BlendOperation _operationAlpha)
			: srcColor(_src)
			, dstColor(_dst)
			, operationColor(_operationColor)
			, srcAlpha(_src)
			, dstAlpha(_dst)
			, operationAlpha(_operationAlpha)
		{
		}

		////////////////////////////////////////////////////////////////////////////////////

		

		////////////////////////////////////////////////////////////////////////////////////

		

		

		////////////////////////////////////////////////////////////////////////////////////

		class SHADOW_API RenderState
		{
		public:
			RenderState();
			virtual ~RenderState();
			virtual void Load(const pugi::xml_node &node);
			virtual void Apply(){}
			virtual RenderState* Clone();

			BlendingState blendingState;
			RasterizationState rasterizerState;
			DepthStencilState depthStencilState;
		};
	}
}

#endif