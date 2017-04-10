#ifndef SHADOW_RENDER_TARGET_INCLUDE
#define SHADOW_RENDER_TARGET_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderTarget
		{
		public:
			virtual ~RenderTarget() { }
			virtual void AddColorTexture(const TexturePtr& texture) = 0;
			virtual void AddDepthTexture(const TexturePtr& texture) = 0;
			virtual void Init() = 0;
			virtual void Bind() = 0;
			virtual void Unbind() = 0;

		public:
			static RenderTargetPtr Default;
		};
	}
}

#endif
