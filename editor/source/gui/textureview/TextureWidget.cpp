#include "TextureWidget.h"

TextureWidget::TextureWidget()
	: sh::gui::Window(sh::math::Rect(100, 100, 350, 500))
{
	SetText("Texture view");

	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	layout->SetMargins(2, 2, 2, 2);
	layout->SetSpacing(2);

	m_imageWidget.reset(new sh::gui::ImageWidget());
	layout->AddWidget(m_imageWidget);

	SetLayout(layout);
}

void TextureWidget::SetTexture(const sh::video::TexturePtr& texture)
{
	if (texture)
		m_imageWidget->SetTexture(texture);
}