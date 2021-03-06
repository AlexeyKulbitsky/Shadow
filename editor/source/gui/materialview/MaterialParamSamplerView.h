#ifndef SHADOW_EDITOR_MATERIAL_PARAM_SAMPLER_VIEW_INCLUDE
#define SHADOW_EDITOR_MATERIAL_PARAM_SAMPLER_VIEW_INCLUDE

#include <Shadow.h>

class MaterialParamSamplerView : public sh::gui::Widget
{
public:
	MaterialParamSamplerView(sh::video::MaterialSamplerParam* param);

	sh::Event<void> paramChanged;

private:
	void TextureChanged(sh::u32 index);
	void TilingUChanged(sh::u32 tiling);
	void TilingVChanged(sh::u32 tiling);
	void MagFilterChanged(sh::u32 filtering);
	void MinFilterChanged(sh::u32 filtering);
	void MipFilterChanged(sh::u32 filtering);

private:
	std::vector<std::weak_ptr<sh::io::FileInfo>> m_textureInfos;

	sh::video::MaterialSamplerParam* m_param = nullptr;
};

#endif
