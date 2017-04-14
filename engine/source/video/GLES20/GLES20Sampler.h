#ifndef SHADOW_GLES20_SAMPLER_INCLUDE
#define SHADOW_GLES20_SAMPLER_INCLUDE

#include "../Sampler.h"

namespace sh
{
	namespace video
	{
		class GLES20Sampler : public Sampler
		{
			friend class GLES20Driver;
			friend class GLES20RenderStateManager;
		public:
		
		private:
			GLES20Sampler(const SamplerDescription& description);
		};
	}
}

#endif
