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
		m_material->SetRenderTechnique("Primitive2D.rt");
	}

	ImageWidget::~ImageWidget()
	{

	}

	void ImageWidget::SetTexture(const video::TexturePtr& texture)
	{
		m_texture = texture;

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
										 math::Vector2(0.0f, 0.0f), 
										 math::Vector4(1.0f));
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										  math::Vector2(1.0f, 1.0f),
										 math::Vector4(1.0f));
		painter->DrawRect(upperLeft, downRight);
	}

	void ImageWidget::RenderBackground(video::Painter* painter)
	{
		painter->SetMaterial(m_material);
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
			math::Vector2(0.0f, 0.0f),
			math::Vector4(1.0f));
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			math::Vector2(1.0f, 1.0f),
			math::Vector4(1.0f));
		painter->DrawRect(upperLeft, downRight);
	}

	void ImageWidget::UpdateLayout()
	{
		Widget::UpdateLayout();
		return;
		if (!m_texture)
			return;

		const auto& textureDesc = m_texture->GetDescription();
		const float aspect = static_cast<float>(textureDesc.width) / static_cast<float>(textureDesc.height);
		const s32 width = static_cast<s32>( static_cast<float>( m_rect.GetHeight() ) * aspect);
		if (width <= m_rect.GetWidth())
		{
			m_rect.lowerRightCorner.x = m_rect.upperLeftCorner.x + width;
			return;
		}
		const s32 height = static_cast<s32>(static_cast<float>(m_rect.GetWidth()) / aspect);
		if (height <= m_rect.GetHeight())
		{
			m_rect.lowerRightCorner.y = m_rect.upperLeftCorner.y + height;
			return;
		}
	}

} // gui

} // sh