#include "MaterialParamSamplerView.h"

MaterialParamSamplerView::MaterialParamSamplerView(sh::video::MaterialSamplerParam* param)
	: m_param(param)
{
	m_textureInfos = sh::io::FileSystem::GetInstance()->GetImageFileInfos();

	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();

	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());

	// Sampler 
	sh::gui::HorizontalLayoutPtr samplerLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr samplerLabel(new sh::gui::Label(m_param->GetName()));
	sh::gui::ComboBoxPtr samplerComboBox(new sh::gui::ComboBox());

	// Add default texture
	samplerComboBox->AddItem("-Default-");

	sh::s32 index = -1;
	for (sh::u32 i = 0U; i < m_textureInfos.size(); ++i)
	{
		if (!m_textureInfos[i].expired())
		{
			samplerComboBox->AddItem(m_textureInfos[i].lock()->name);
			if (texture->GetFileInfo().lock()->name == m_textureInfos[i].lock()->name)
				index = i;
		}
		
	}
	samplerComboBox->SetSelectedItem(index + 1);
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
	tilingUComboBox->SetSelectedItem(static_cast<sh::u32>(desc.tilingU));
	tilingUComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::TilingUChanged, this,
		std::placeholders::_1));
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
	tilingVComboBox->SetSelectedItem(static_cast<sh::u32>(desc.tilingV));
	tilingVComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::TilingVChanged, this,
		std::placeholders::_1));
	tilingVLayout->AddWidget(tilingVLabel);
	tilingVLayout->AddWidget(tilingVComboBox);
	layout->AddLayout(tilingVLayout);

	// Min filter
	sh::gui::HorizontalLayoutPtr minFilterLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr minFilterLabel(new sh::gui::Label("Min filter"));
	sh::gui::ComboBoxPtr minFilterComboBox(new sh::gui::ComboBox());
	minFilterComboBox->AddItem("Nearest");
	minFilterComboBox->AddItem("Linear");
	minFilterComboBox->AddItem("None");
	minFilterComboBox->SetSelectedItem(static_cast<sh::u32>(desc.minFilter));
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
	magFilterComboBox->AddItem("None");
	magFilterComboBox->SetSelectedItem(static_cast<sh::u32>(desc.magFilter));
	magFilterComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::MagFilterChanged, this,
		std::placeholders::_1));
	magFilterLayout->AddWidget(magFilterLabel);
	magFilterLayout->AddWidget(magFilterComboBox);
	layout->AddLayout(magFilterLayout);

	// Mip filter
	sh::gui::HorizontalLayoutPtr mipFilterLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr mipFilterLabel(new sh::gui::Label("Mip filter"));
	sh::gui::ComboBoxPtr mipFilterComboBox(new sh::gui::ComboBox());
	mipFilterComboBox->AddItem("Nearest");
	mipFilterComboBox->AddItem("Linear");
	mipFilterComboBox->AddItem("None");
	mipFilterComboBox->SetSelectedItem(static_cast<sh::u32>(desc.mipFilter));
	mipFilterComboBox->OnItemChanged.Connect(std::bind(&MaterialParamSamplerView::MipFilterChanged, this,
		std::placeholders::_1));
	mipFilterLayout->AddWidget(mipFilterLabel);
	mipFilterLayout->AddWidget(mipFilterComboBox);
	layout->AddLayout(mipFilterLayout);

	SetLayout(layout);
	SetMaximumHeight(100);
}

void MaterialParamSamplerView::TextureChanged(sh::u32 index)
{
	sh::ResourceManager* resourceManager = sh::Device::GetInstance()->GetResourceManager();
	sh::video::TexturePtr texture;
	if (index == 0U)
	{
		texture = resourceManager->GetDefaultTexture();
	}
	else
	{
		const auto& name = m_textureInfos[index - 1].lock()->name;
		texture = resourceManager->GetTexture(name);
	}

	auto sampler = m_param->GetSampler();
	sampler->Set(texture);

	paramChanged();
}

void MaterialParamSamplerView::TilingUChanged(sh::u32 tiling)
{
	sh::TextureTiling tile = static_cast<sh::TextureTiling>(tiling);
	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();
	desc.tilingU = tile;

	auto newSampler = sh::video::Sampler::Create(desc);
	newSampler->Set(texture);
	m_param->SetSampler(newSampler);

	paramChanged();
}

void MaterialParamSamplerView::TilingVChanged(sh::u32 tiling)
{
	sh::TextureTiling tile = static_cast<sh::TextureTiling>(tiling);
	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();
	desc.tilingV = tile;

	auto newSampler = sh::video::Sampler::Create(desc);
	newSampler->Set(texture);
	m_param->SetSampler(newSampler);

	paramChanged();
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

	paramChanged();
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

	paramChanged();
}

void MaterialParamSamplerView::MipFilterChanged(sh::u32 filtering)
{
	sh::TextureFiltering filter = static_cast<sh::TextureFiltering>(filtering);
	auto sampler = m_param->GetSampler();
	auto texture = sampler->GetTexture();
	auto desc = sampler->GetDescription();
	desc.mipFilter = filter;

	auto newSampler = sh::video::Sampler::Create(desc);
	newSampler->Set(texture);
	m_param->SetSampler(newSampler);

	paramChanged();
}