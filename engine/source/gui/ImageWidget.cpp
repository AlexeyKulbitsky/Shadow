#include "ImageWidget.h"

#include "../video/Material.h"
#include "../Device.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"

namespace sh
{

namespace gui
{

	ImageWidget::ImageWidget()
	{
		m_material.reset(new video::Material());
		m_material->SetRenderTechnique("ui_base.rt");
	}

	ImageWidget::~ImageWidget()
	{

	}

	void ImageWidget::SetTexture(const video::TexturePtr& texture)
	{
		const auto& samplerParams = m_material->GetParams()->GetSamplerParams();
		SH_ASSERT(samplerParams.size() == 1, "There must be only one texture in shader for rendering Image Widget!");
		if (samplerParams.size() != 1)
			return;
		samplerParams[0].GetSampler()->Set(texture);
	}

	void ImageWidget::Render(video::Painter* painter)
	{
		painter->SetMaterial(m_material);
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 math::Vector2f(0.0f, 0.0f), 
										 math::Vector4f(1.0f));
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										  math::Vector2f(1.0f, 1.0f),
										 math::Vector4f(1.0f));
		painter->DrawRect(upperLeft, downRight);
	}

} // gui

} // sh