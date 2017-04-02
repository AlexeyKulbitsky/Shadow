#ifndef SHADOW_BLENDING_STATE_INCLUDE
#define SHADOW_BLENDING_STATE_INCLUDE

#include "../Globals.h"

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
			void Load(const pugi::xml_node &node);

			bool enabled = false;

			BlendFactor srcColor = BF_ONE;
			BlendFactor dstColor = BF_ZERO;
			BlendOperation operationColor = BO_ADD;

			BlendFactor srcAlpha = BF_ONE;
			BlendFactor dstAlpha = BF_ZERO;
			BlendOperation operationAlpha = BO_ADD;
		};

		inline BlendingState::BlendingState()
			: srcColor(BF_ONE)
			, dstColor(BF_ZERO)
			, operationColor(BO_ADD)
			, srcAlpha(BF_ONE)
			, dstAlpha(BF_ZERO)
			, operationAlpha(BO_ADD)
		{}

		inline BlendingState::BlendingState(BlendCombination combination)
		{
			switch (combination)
			{
			case BlendCombination::SOLID: srcColor = srcAlpha = BF_ONE; dstColor = dstAlpha = BF_ZERO; break;
			case BlendCombination::ALPHA: srcColor = srcAlpha = BF_SRC_ALPHA; dstColor = dstAlpha = BF_INV_SRC_ALPHA; break;
			case BlendCombination::ADDITIVE: srcColor = srcAlpha = BF_ONE; dstColor = dstAlpha = BF_ONE; break;
			case BlendCombination::MULTIPLY: srcColor = srcAlpha = BF_ZERO; dstColor = dstAlpha = BF_SRC_COLOR; break;
			case BlendCombination::BURN: srcColor = srcAlpha = BF_DST_COLOR; dstColor = dstAlpha = BF_ONE; break;
			case BlendCombination::SCREEN: srcColor = srcAlpha = BF_ONE; dstColor = dstAlpha = BF_INV_SRC_COLOR; break;
			case BlendCombination::SUBSTRACTIVE: srcColor = srcAlpha = BF_ZERO; dstColor = dstAlpha = BF_INV_SRC_COLOR; break;
			default: break;
			}
			operationColor = BO_ADD;
			operationAlpha = BO_ADD;
		}

		inline BlendingState::BlendingState(BlendFactor _srcColor, BlendFactor _srcAlpha, BlendFactor _dstColor, BlendFactor _dstAlpha)
			: srcColor(_srcColor)
			, dstColor(_dstColor)
			, operationColor(BO_ADD)
			, srcAlpha(_srcAlpha)
			, dstAlpha(_dstAlpha)
			, operationAlpha(BO_ADD)
		{
		}

		inline BlendingState::BlendingState(BlendFactor _src, BlendFactor _dst)
			: srcColor(_src)
			, dstColor(_dst)
			, operationColor(BO_ADD)
			, srcAlpha(_src)
			, dstAlpha(_dst)
			, operationAlpha(BO_ADD)
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
	}
}

#endif
