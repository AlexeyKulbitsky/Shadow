#ifndef SHADOW_IMAGE_WIDGET_INCLUDE
#define SHADOW_IMAGE_WIDGET_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class ImageWidget : public Widget
	{
	public:
		ImageWidget();
		~ImageWidget();

		void SetTexture(const video::TexturePtr& texture);
		virtual void Render(video::Painter* painter) override;
	};

} // gui

} // sh

#endif