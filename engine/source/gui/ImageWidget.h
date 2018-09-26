#ifndef SHADOW_IMAGE_WIDGET_INCLUDE
#define SHADOW_IMAGE_WIDGET_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class SHADOW_API ImageWidget : public Widget
	{
        SHADOW_OBJECT(ImageWidget)
	public:
		ImageWidget();
		~ImageWidget();
        
        static void RegisterObject();

		void SetTexture(const video::TexturePtr& texture);
		const video::TexturePtr& GetTexture() const { return m_texture; }
		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override {}
		virtual void UpdateLayout() override;

	private:
		video::TexturePtr m_texture;
		bool m_dirty = false;
	};

} // gui

} // sh

#endif
