#include "MaterialParamSamplerView.h"

MaterialParamSamplerView::MaterialParamSamplerView(sh::video::MaterialSamplerParam* param)
	: m_param(param)
{
	m_textureNames.push_back("statue.jpg");
	m_textureNames.push_back("Container.png");

	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();

	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());

	// Sampler 
	sh::gui::HorizontalLayoutPtr samplerLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr samplerLabel(new sh::gui::Label("Texture"));
	sh::gui::ComboBoxPtr samplerComboBox(new sh::gui::ComboBox());

	for (const auto& name : m_textureNames)
	{
		samplerComboBox->AddItem(name);
	}
	samplerComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::TextureChanged, this,
		std::placeholders::_1));

	samplerLayout->AddWidget(samplerLabel);
	samplerLayout->AddWidget(samplerComboBox);
	layout->AddLayout(samplerLayout);

	// U tiling
	sh::gui::HorizontalLayoutPtr tilingULayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr tilingULabel(new sh::gui::Label("Tiling U"));
	sh::gui::ComboBoxPtr tilingUComboBox(new sh::gui::ComboBox());
	tilingUComboBox->AddItem("Repeat");
	tilingUComboBox->AddItem("Mirrored repeat");
	tilingUComboBox->AddItem("Clamp to edge");
	tilingULayout->AddWidget(tilingULabel);
	tilingULayout->AddWidget(tilingUComboBox);
	layout->AddLayout(tilingULayout);

	// V tiling
	sh::gui::HorizontalLayoutPtr tilingVLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr tilingVLabel(new sh::gui::Label("Tiling V"));
	sh::gui::ComboBoxPtr tilingVComboBox(new sh::gui::ComboBox());
	tilingVComboBox->AddItem("Repeat");
	tilingVComboBox->AddItem("Mirrored repeat");
	tilingVComboBox->AddItem("Clamp to edge");
	tilingVLayout->AddWidget(tilingVLabel);
	tilingVLayout->AddWidget(tilingVComboBox);
	layout->AddLayout(tilingVLayout);

	// Min filter
	sh::gui::HorizontalLayoutPtr minFilterLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr minFilterLabel(new sh::gui::Label("Min filter"));
	sh::gui::ComboBoxPtr minFilterComboBox(new sh::gui::ComboBox());
	minFilterComboBox->AddItem("Nearest");
	minFilterComboBox->AddItem("Linear");
	minFilterComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::MinFilterChanged, this,
		std::placeholders::_1));
	minFilterLayout->AddWidget(minFilterLabel);
	minFilterLayout->AddWidget(minFilterComboBox);
	layout->AddLayout(minFilterLayout);

	// Mag filter
	sh::gui::HorizontalLayoutPtr magFilterLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr magFilterLabel(new sh::gui::Label("Mag filter"));
	sh::gui::ComboBoxPtr magFilterComboBox(new sh::gui::ComboBox());
	magFilterComboBox->AddItem("Nearest");
	magFilterComboBox->AddItem("Linear");
	magFilterComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::MagFilterChanged, this,
		std::placeholders::_1));
	magFilterLayout->AddWidget(magFilterLabel);
	magFilterLayout->AddWidget(magFilterComboBox);
	layout->AddLayout(magFilterLayout);

	SetLayout(layout);
	SetMaximumHeight(100);
}

void MaterialParamSamplerView::TextureChanged(sh::u32 index)
{
	const auto& name = m_textureNames[index];
	
	sh::ResourceManager* resourceManager = sh::Device::GetInstance()->GetResourceManager();
	auto texture = resourceManager->GetTexture(name);

	auto sampler = m_param->GetSampler();
	sampler->Set(texture);
}

void MaterialParamSamplerView::MagFilterChanged(sh::u32 filtering)
{
	sh::TextureFiltering filter = static_cast<sh::TextureFiltering>(filtering);
	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();
	desc.magFilter = filter;

	auto newSampler = sh::video::Sampler::Create(desc);
	newSampler->Set(texture);
	m_param->SetSampler(newSampler);
}

void MaterialParamSamplerView::MinFilterChanged(sh::u32 filtering)
{
	sh::TextureFiltering filter = static_cast<sh::TextureFiltering>(filtering);
	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();
	desc.minFilter = filter;

	auto newSampler = sh::video::Sampler::Create(desc);
	newSampler->Set(texture);
	m_param->SetSampler(newSampler);
}