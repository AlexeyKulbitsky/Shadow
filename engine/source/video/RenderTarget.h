#ifndef SHADOW_RENDER_TARGET_INCLUDE
#define SHADOW_RENDER_TARGET_INCLUDE

namespace sh
{
	namespace video
	{
		class RenderTarget
		{
		public:
			virtual void Bind();
			virtual void Unbind();
		};
	}
}

#endif
