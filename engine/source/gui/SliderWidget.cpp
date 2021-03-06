#include "SliderWidget.h"
#include "Button.h"
#include "GuiManager.h"
#include "Style.h"

#include "../video/Painter.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../serialization/ObjectFactory.h"

namespace sh
{

namespace gui
{

	SliderWidget::SliderWidget()
	{
		/*m_button.reset(new Button());
		m_button->SetWidth(10);

		m_button->OnPress.Connect(std::bind(&SliderWidget::OnButtonPressed, this));
		m_button->OnRelease.Connect(std::bind(&SliderWidget::OnButtonReleased, this));*/
		SetMaximumHeight(20);

		m_sliderRect.upperLeftCorner = m_rect.upperLeftCorner;
		m_sliderRect.lowerRightCorner.x = m_rect.upperLeftCorner.x + 10;
		m_sliderRect.lowerRightCorner.y = m_rect.lowerRightCorner.y;
        m_name = "SliderWidget";
	}
    
    void SliderWidget::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<SliderWidget>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<SliderWidget, Widget>();
        S_ACCESSOR_PROPERTY("Value", GetValue, SetValue);
        S_ACCESSOR_PROPERTY("MinValue", GetMinValue, SetMinValue);
        S_ACCESSOR_PROPERTY("MaxValue", GetMaxValue, SetMaxValue);
    }

	SliderWidget::~SliderWidget()
	{
	}

	bool SliderWidget::ProcessEvent(sh::gui::GUIEvent& ev)
	{
		bool inside = m_sliderRect.IsPointInside(ev.x, ev.y);
		switch (ev.type)
		{
			case EventType::PointerDown:
			{
				if (inside)
				{
					m_dragStarted = true;
					m_dragPointOffset = ev.x - m_sliderRect.upperLeftCorner.x;
					GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
					return true;
				}
			}
			break;
			case EventType::PointerUp:
			{
				if (IsInFocus())
				{
					m_dragStarted = false;
					GuiManager::GetInstance()->SetFocusWidget(nullptr);
					return true;
				}
			}
			break;
			case EventType::PointerMove:
			{
				if (m_dragStarted)
				{
					int xDelta = ev.x - m_rect.upperLeftCorner.x - m_dragPointOffset;

					int xPos = m_rect.upperLeftCorner.x + xDelta;
					if (xPos < m_rect.upperLeftCorner.x)
						xPos = m_rect.upperLeftCorner.x;
					if (xPos + m_sliderRect.GetWidth() > m_rect.lowerRightCorner.x)
						xPos = m_rect.lowerRightCorner.x - m_sliderRect.GetWidth();

					m_sliderRect.upperLeftCorner.x = xPos;
					m_sliderRect.lowerRightCorner.x = xPos + 10;

					float aspect = static_cast<float>(xPos - m_rect.upperLeftCorner.x) / 
						static_cast<float>(m_rect.lowerRightCorner.x - m_rect.upperLeftCorner.x - m_sliderRect.GetWidth());
					const float value = m_valueEdges.x + (m_valueEdges.y - m_valueEdges.x) * aspect;

					if (math::Abs(value - m_value) > 0.0001f)
					{
						m_value = value;
						valueChanged(m_value);
					}

					return true;
				}
			}
			break;
			default:
				break;
		}
		return false;
	}

	void SliderWidget::Render(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetTextMaterial());

		const auto height = m_rect.GetHeight();
		const int barHeight = 5;
		video::Painter::Vertex barUpperLeft(
			math::Vector2Int(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y + (height - barHeight) / 2),
			math::Vector2(0.0f),
			math::Vector4(0.8f, 0.8f, 0.8f, 1.0f));
		video::Painter::Vertex barDownRight(
			math::Vector2Int(m_rect.lowerRightCorner.x, m_rect.upperLeftCorner.y + (height - barHeight) / 2 + barHeight),
			math::Vector2(0.0f),
			math::Vector4(0.8f, 0.8f, 0.8f, 1.0f));
		painter->DrawRect(barUpperLeft, barDownRight);

		video::Painter::Vertex sliderUpperLeft(
			m_sliderRect.upperLeftCorner,
			math::Vector2(0.0f),
			math::Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		video::Painter::Vertex sliderDownRight(
			m_sliderRect.lowerRightCorner,
			math::Vector2(0.0f),
			math::Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		painter->DrawRect(sliderUpperLeft, sliderDownRight);

		//m_button->Render(painter);
	}

	void SliderWidget::RenderBackground(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetTextMaterial());

		const auto height = m_rect.GetHeight();
		const int barHeight = 5;
		video::Painter::Vertex barUpperLeft(
			math::Vector2Int(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y + (height - barHeight) / 2),
			math::Vector2(0.0f),
			math::Vector4(0.8f, 0.8f, 0.8f, 1.0f));
		video::Painter::Vertex barDownRight(
			math::Vector2Int(m_rect.lowerRightCorner.x, m_rect.upperLeftCorner.y + (height - barHeight) / 2 + barHeight),
			math::Vector2(0.0f),
			math::Vector4(0.8f, 0.8f, 0.8f, 1.0f));
		painter->DrawRect(barUpperLeft, barDownRight);

		video::Painter::Vertex sliderUpperLeft(
			m_sliderRect.upperLeftCorner,
			math::Vector2(0.0f),
			math::Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		video::Painter::Vertex sliderDownRight(
			m_sliderRect.lowerRightCorner,
			math::Vector2(0.0f),
			math::Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		painter->DrawRect(sliderUpperLeft, sliderDownRight);
	}

	void SliderWidget::RenderText(video::Painter* painter)
	{
	}

	void SliderWidget::UpdateLayout()
	{
		const float aspect = (m_value - m_valueEdges.x) / (m_valueEdges.y - m_valueEdges.x);
		const int delta = static_cast<int>(static_cast<float>(m_rect.GetWidth() - 10) * aspect);

		m_sliderRect.upperLeftCorner.x = m_rect.upperLeftCorner.x + delta;
		m_sliderRect.upperLeftCorner.y = m_rect.upperLeftCorner.y;
		m_sliderRect.lowerRightCorner.x = m_sliderRect.upperLeftCorner.x + 10;
		m_sliderRect.lowerRightCorner.y = m_rect.lowerRightCorner.y;
	}

	void SliderWidget::SetValue(float value)
	{
		m_value = math::Clamp(value, m_valueEdges.x, m_valueEdges.y);
		UpdateLayout();
	}

	void SliderWidget::SetMinValue(float value)
	{
		m_valueEdges.x = value;
		m_value = math::Clamp(m_value, m_valueEdges.x, m_valueEdges.y);
		UpdateLayout();
	}

	void SliderWidget::SetMaxValue(float value)
	{
		m_valueEdges.y = value;
		m_value = math::Clamp(m_value, m_valueEdges.x, m_valueEdges.y);
		UpdateLayout();
	}

	void SliderWidget::SetMinMaxValues(float min, float max)
	{
		m_valueEdges.x = min;
		m_valueEdges.y = max;
		m_value = math::Clamp(m_value, m_valueEdges.x, m_valueEdges.y);
		UpdateLayout();
	}

	void SliderWidget::SetMinMaxValues(const math::Vector2& values)
	{
		m_valueEdges = values;
		m_value = math::Clamp(m_value, m_valueEdges.x, m_valueEdges.y);
		UpdateLayout();
	}

} // gui

} // sh
