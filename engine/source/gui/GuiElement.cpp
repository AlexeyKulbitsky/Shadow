#include "GuiElement.h"

#include "GuiManager.h"

#include "../Device.h"
#include "../video/Driver.h"

namespace sh
{

namespace gui
{
	GuiElement::GuiElement()
	{
		m_batchData.resize(4 * 8);
	}

	void GuiElement::GetGeometry(GuiBatchData& data)
	{
		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 1);
		data.indices.push_back(data.verticesCount + 2);

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 2);
		data.indices.push_back(data.verticesCount + 3);
		data.verticesCount += 4;
	}

	void GuiElement::SetPosition(u32 x, u32 y) 
	{ 
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
		UpdatePosition();
	}

	void GuiElement::SetSize(const math::Vector2u& size) 
	{ 
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		UpdatePosition();
	}

	void GuiElement::SetWidth(u32 width)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + width, pos.y + size.y);
		UpdatePosition();
	}

	void GuiElement::SetHeight(u32 height) 
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + height);
		UpdatePosition();
	}

	void GuiElement::UpdatePosition()
	{
		const auto& viewPort = sh::Device::GetInstance()->GetDriver()->GetViewPort();
		math::Vector4f leftUp((float)m_rect.upperLeftCorner.x, (float)m_rect.upperLeftCorner.y, 0.0f, 1.0f);

		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);

		m_batchData[0] = leftUp.x; m_batchData[1] = leftUp.y; m_batchData[2] = 0.0f;
		m_batchData[8] = leftUp.x; m_batchData[9] = rightDown.y; m_batchData[10] = 0.0f;
		m_batchData[16] = rightDown.x; m_batchData[17] = rightDown.y; m_batchData[18] = 0.0f;
		m_batchData[24] = rightDown.x; m_batchData[25] = leftUp.y; m_batchData[26] = 0.0f;
	}

	void GuiElement::UpdateUV(const math::Vector2f& uvLeftUp, const math::Vector2f& uvRightDown)
	{
		m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
		m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
		m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
		m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 
	}

	void GuiElement::UpdateColor(const math::Vector3f& color)
	{
		m_batchData[5] = color.x; m_batchData[6] = color.y; m_batchData[7] = color.z;
		m_batchData[13] = color.x; m_batchData[14] = color.y; m_batchData[15] = color.z; 
		m_batchData[21] = color.x; m_batchData[22] = color.y; m_batchData[23] = color.z; 
		m_batchData[29] = color.x; m_batchData[30] = color.y;  m_batchData[31] = color.z; 
	}

} // gui

} // sh