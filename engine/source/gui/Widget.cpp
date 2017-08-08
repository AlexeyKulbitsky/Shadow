#include "Widget.h"
#include "Layout.h"
#include "GuiManager.h"

namespace sh
{

namespace gui
{

	Widget::Widget()
	{
		m_rect.lowerRightCorner = math::Vector2u(10U, 10U);
		m_batchData.resize(4 * 9);
		UpdateColor(math::Vector4f(0.0f));
	}

	Widget::~Widget()
	{
	}

	void Widget::SetLayout(const LayoutPtr& layout)
	{
		m_layout = layout;
		m_layout->SetParent(this);
		UpdateLayout();
	}

	void Widget::Load(const pugi::xml_node& node)
	{
	}

	void Widget::GetGeometry(GuiBatchData& data)
	{
		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 1);
		data.indices.push_back(data.verticesCount + 2);

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 2);
		data.indices.push_back(data.verticesCount + 3);
		data.verticesCount += 4;


		if (!m_layout)
			return;
		m_layout->GetGeometry(data);
	}

	void Widget::GetTextGeometry(GuiBatchData& data)
	{
		if (!m_layout)
			return;

		m_layout->GetTextGeometry(data);
	}

	void Widget::SetPosition(u32 x, u32 y)
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
		UpdatePosition();
		UpdateLayout();
	}

	void Widget::SetSize(const math::Vector2u& size)
	{
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		UpdatePosition();
		UpdateLayout();
	}

	void Widget::SetWidth(u32 width)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + width, pos.y + size.y);
		UpdatePosition();
		UpdateLayout();
	}

	void Widget::SetHeight(u32 height)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + height);
		UpdatePosition();

		UpdateLayout();
	}

	bool Widget::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		if (!m_layout)
			return false;

		return m_layout->ProcessInput(x, y, type);
	}

	bool Widget::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		if (!m_layout)
			return false;

		return m_layout->ProcessKeyboardInput(type, code);
	}

	void Widget::SetMargins(u32 top, u32 right, u32 bottom, u32 left)
	{
		m_topMargin = top;
		m_rightMargin = right;
		m_bottomMargin = bottom;
		m_leftMargin = left;

		UpdateLayout();
	}

	void Widget::UpdateLayout()
	{
		if (m_layout)
		{
			auto upperLeft = m_rect.upperLeftCorner;
			auto lowerRight = m_rect.lowerRightCorner;

			math::Rectu finalRect;
			finalRect.upperLeftCorner.x = upperLeft.x + m_leftMargin;
			finalRect.upperLeftCorner.y = upperLeft.y + m_topMargin;
			finalRect.lowerRightCorner.x = lowerRight.x - m_rightMargin;
			finalRect.lowerRightCorner.y = lowerRight.y - m_bottomMargin;

			m_layout->Resize(finalRect);
		}
	}

	void Widget::UpdatePosition()
	{
		math::Vector4f leftUp((float)m_rect.upperLeftCorner.x, (float)m_rect.upperLeftCorner.y, 0.0f, 1.0f);

		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);

		m_batchData[0] = leftUp.x; m_batchData[1] = leftUp.y; m_batchData[2] = 0.0f;
		m_batchData[9] = leftUp.x; m_batchData[10] = rightDown.y; m_batchData[11] = 0.0f;
		m_batchData[18] = rightDown.x; m_batchData[19] = rightDown.y; m_batchData[20] = 0.0f;
		m_batchData[27] = rightDown.x; m_batchData[28] = leftUp.y; m_batchData[29] = 0.0f;
	}

	void Widget::UpdateUV(const math::Vector2f& uvLeftUp, const math::Vector2f& uvRightDown)
	{
		m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y;
		m_batchData[12] = uvLeftUp.x; m_batchData[13] = uvRightDown.y;
		m_batchData[21] = uvRightDown.x; m_batchData[22] = uvRightDown.y;
		m_batchData[30] = uvRightDown.x; m_batchData[31] = uvLeftUp.y;
	}

	void Widget::UpdateColor(const math::Vector4f& color)
	{
		m_batchData[5] = color.x; m_batchData[6] = color.y; m_batchData[7] = color.z; m_batchData[8] = color.w;
		m_batchData[14] = color.x; m_batchData[15] = color.y; m_batchData[16] = color.z; m_batchData[17] = color.w;
		m_batchData[23] = color.x; m_batchData[24] = color.y; m_batchData[25] = color.z; m_batchData[26] = color.w;
		m_batchData[32] = color.x; m_batchData[33] = color.y; m_batchData[34] = color.z; m_batchData[35] = color.w;
	}


} // gui

} // sh