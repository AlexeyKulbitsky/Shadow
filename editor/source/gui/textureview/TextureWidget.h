#include <Shadow.h>

class TextureWidget : public sh::gui::Window
{
public:
	TextureWidget();
	void SetTexture(const sh::video::TexturePtr& texture);

private:
	sh::gui::ImageWidgetPtr m_imageWidget;
};