#ifndef SHADOW_RENDER_STATE_INCLUDE
#define SHADOW_RENDER_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		enum class FrontFace
		{
			CLOCKWISE = 0,
			COUNTER_CLOCKWISE,

			COUNT
		};

		enum class CullFace
		{
			FRONT = 0,
			BACK,
			FRONT_AND_BACK,
			NONE,

			COUNT
		};

		enum class PolygonMode
		{
			FILL = 0,
			POINT,
			LINE,

			COUNT
		};

		enum class CompareFunction
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

		enum class StencilOperation
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

		enum class BlendFactor
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

		enum class BlendOperation
		{
			ADD = 0,
			SUBSTRACT,

			COUNT
		};

		enum class BlendCombination
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

		////////////////////////////////////////////////////////////////////////////////////

		
		struct BlendingState
		{
			BlendingState();
			BlendingState(BlendCombination combination);
			BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha);
			BlendingState(BlendFactor src, BlendFactor dst);
			BlendingState(BlendFactor srcColor, BlendFactor srcAlpha, BlendFactor dstColor, BlendFactor dstAlpha, BlendOperation operation);
			BlendingState(BlendFactor src, BlendFactor dst, BlendOperation operation);


			BlendFactor srcColor;
			BlendFactor srcAlpha;
			BlendFactor dstColor;
			BlendFactor dstAlpha;
			BlendOperation operation;
		};

		inline BlendingState::BlendingState()
			: srcColor(BlendFactor::ONE)
			, srcAlpha(BlendFactor::ONE)
			, dstColor(BlendFactor::ZERO)
			, dstAlpha(BlendFactor::ZERO)
			, operation(BlendOperation::ADD) 
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
			operation = BlendOperation::ADD;
		}

		inline BlendingState::BlendingState(BlendFactor _srcColor, BlendFactor _srcAlpha, BlendFactor _dstColor, BlendFactor _dstAlpha)
			: srcColor(_srcColor)
			, srcAlpha(_srcAlpha)
			, dstColor(_dstColor)
			, dstAlpha(_dstAlpha)
			, operation(BlendOperation::ADD)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _src, BlendFactor _dst)
			: srcColor(_src)
			, srcAlpha(_src)
			, dstColor(_dst)
			, dstAlpha(_dst)
			, operation(BlendOperation::ADD)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _srcColor, BlendFactor _srcAlpha, BlendFactor _dstColor, BlendFactor _dstAlpha, BlendOperation _operation)
			: srcColor(_srcColor)
			, srcAlpha(_srcAlpha)
			, dstColor(_dstColor)
			, dstAlpha(_dstAlpha)
			, operation(_operation)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _src, BlendFactor _dst, BlendOperation _operation)
			: srcColor(_src)
			, srcAlpha(_src)
			, dstColor(_dst)
			, dstAlpha(_dst)
			, operation(_operation)
		{
		}

		////////////////////////////////////////////////////////////////////////////////////

		struct RaterizerState
		{
			CullFace    cullFace = CullFace::BACK;
			FrontFace	frontFace = FrontFace::COUNTER_CLOCKWISE;
			bool		polygonOffsetEnabled = false;
			float		polygonOffsetFactor = 0.0f; //OffsetFactor
			float		polygonOffsetUnits = 0.0f; //DepthBias
		};

		////////////////////////////////////////////////////////////////////////////////////

		struct DepthStencilState
		{
			bool				enableDepthTest = true;
			CompareFunction		depthCompareFunction = CompareFunction::LESS_OR_EQUAL;
			bool				depthWriteEnabled = true;

			bool				enableStencilTest = false;
			CompareFunction		stencilCompareFunction = CompareFunction::ALWAYS;
			StencilOperation	frontStencilPassOp = StencilOperation::KEEP;
			StencilOperation	frontStencilsFailOp = StencilOperation::KEEP;
			StencilOperation	frontStencilzFailOp = StencilOperation::KEEP;
			StencilOperation	backStencilPassOp = StencilOperation::KEEP;
			StencilOperation	backStencilsFailOp = StencilOperation::KEEP;
			StencilOperation	backStencilzFailOp = StencilOperation::KEEP;
			int32_t				stencilRef = 0;
			uint32_t			stencilMask = 0;
		};

		////////////////////////////////////////////////////////////////////////////////////

		class RenderState
		{
		public:
			RenderState();
			virtual ~RenderState();
			virtual void Load(const pugi::xml_node &node);
			virtual void Apply(){}
			virtual RenderState* Clone();

			BlendingState blendingState;
			RaterizerState rasterizerState;
			DepthStencilState depthStencilState;
		};
	}
}

#endif