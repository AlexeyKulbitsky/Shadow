#include "ColorPicker.h"

ColorPicker::ColorPicker()
	:sh::gui::Window(sh::math::Recti(0, 100, 250, 500))	
{
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	SetLayout(layout);

	SetClosable(true);

	sh::gui::ImageWidgetPtr paletteWidget(new sh::gui::ImageWidget());
	auto paletteTexture = sh::Device::GetInstance()->GetResourceManager()->GetTexture("HSVPalette.png");
	paletteWidget->SetTexture(paletteTexture);
	m_layout->AddWidget(paletteWidget);
}