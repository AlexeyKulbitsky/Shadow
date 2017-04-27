#include "Button.h"

#include "GuiManager.h"

#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../video/VertexDeclaration.h"
#include "../video/Driver.h"
#include "../Device.h"




namespace sh
{

namespace gui
{
	Button::Button(const math::Rectf rect)
	{
		m_rect = rect;
	}

	Button::Button()
	{
	}

	void Button::Load(const pugi::xml_node& node)
	{
		pugi::xml_node rect = node.child("rect");
		SH_ASSERT(rect, "Can not find Rect node for button!");
		f32 x1 = rect.attribute("x1").as_float();
		f32 y1 = rect.attribute("y1").as_float();
		f32 x2 = rect.attribute("x2").as_float();
		f32 y2 = rect.attribute("y2").as_float();

		m_rect.Set(x1, y1, x2, y2);

		pugi::xml_node pressed = node.child("pressed");
		f32 u1 = pressed.attribute("u1").as_float();
		f32 v1 = pressed.attribute("v1").as_float();
		f32 u2 = pressed.attribute("u2").as_float();
		f32 v2 = pressed.attribute("v2").as_float();
		m_pressedRect.Set(u1 / 256, v1 / 128, u2 / 256, v2 / 128);

		pugi::xml_node released = node.child("released");
		u1 = released.attribute("u1").as_float();
		v1 = released.attribute("v1").as_float();
		u2 = released.attribute("u2").as_float();
		v2 = released.attribute("v2").as_float();
		m_releasedRect.Set(u1 / 256, v1 / 128, u2 / 256, v2 / 128);

		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2f extends(viewPort.z, viewPort.w);

		const math::Matrix4f& ortho = sh::Device::GetInstance()->GetSceneManager()->GetCamera()->Get2DProjectionMatrix();

		math::Vector4f leftUp(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y, 0.0f, 1.0f);
		leftUp = ortho * leftUp;
		math::Vector4f rightDown(m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y, 0.0f, 1.0f);
		rightDown = ortho * rightDown;

		math::Vector2f uvLeftUp = m_releasedRect.upperLeftCorner;
		math::Vector2f uvRightDown = m_releasedRect.lowerRightCorner;

		
		std::vector<float> vertices = 
		{
			leftUp.x, leftUp.y, 0.0f,			uvLeftUp.x, uvLeftUp.y,			1.0f, 1.0f, 1.0f,
			leftUp.x, rightDown.y, 0.0f,		uvLeftUp.x, uvRightDown.y,		1.0f, 1.0f, 1.0f,
			rightDown.x, rightDown.y, 0.0f,		uvRightDown.x, uvRightDown.y,	1.0f, 1.0f, 1.0f,
			rightDown.x, leftUp.y, 0.0f,		uvRightDown.x, uvLeftUp.y,		1.0f, 1.0f, 1.0f
		};
		

		m_batchData = std::move(vertices);
	}

	void Button::SetPosition(u32 x, u32 y)
	{
		
	}

	void Button::SetWidth(u32 width)
	{

	}

	void Button::SetHeight(u32 height)
	{

	}

	bool Button::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		bool inside = m_rect.IsPointInside(float(x), float(y));
		if (inside)
		{
			switch (type)
			{
				case MouseEventType::ButtonPressed:
				{
					math::Vector2f uvLeftUp = m_pressedRect.upperLeftCorner;
					math::Vector2f uvRightDown = m_pressedRect.lowerRightCorner;

					m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
					m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
					m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
					m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 

					// color
					m_batchData[5] = 1.0f; m_batchData[6] = 1.0f; m_batchData[7] = 1.0f;
					m_batchData[13] = 1.0f; m_batchData[14] = 1.0f; m_batchData[15] = 1.0f; 
					m_batchData[21] = 1.0f; m_batchData[22] = 1.0f; m_batchData[23] = 1.0f; 
					m_batchData[29] = 1.0f; m_batchData[30] = 1.0f;  m_batchData[31] = 1.0f; 

					OnPress();
				}
					break;
				case MouseEventType::ButtonReleased:
				{
					math::Vector2f uvLeftUp = m_releasedRect.upperLeftCorner;
					math::Vector2f uvRightDown = m_releasedRect.lowerRightCorner;

					m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
					m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
					m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
					m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 

					// color
					m_batchData[5] = 1.0f; m_batchData[6] = 1.0f; m_batchData[7] = 1.0f;
					m_batchData[13] = 1.0f; m_batchData[14] = 1.0f; m_batchData[15] = 1.0f; 
					m_batchData[21] = 1.0f; m_batchData[22] = 1.0f; m_batchData[23] = 1.0f; 
					m_batchData[29] = 1.0f; m_batchData[30] = 1.0f;  m_batchData[31] = 1.0f; 

					OnRelease();
				}
					break;
				case MouseEventType::Moved:
				{
					math::Vector2f uvLeftUp = m_releasedRect.upperLeftCorner;
					math::Vector2f uvRightDown = m_releasedRect.lowerRightCorner;

					m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
					m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
					m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
					m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 

					// color
					m_batchData[5] = 1.0f; m_batchData[6] = 1.0f; m_batchData[7] = 0.0f;
					m_batchData[13] = 1.0f; m_batchData[14] = 1.0f; m_batchData[15] = 0.0f; 
					m_batchData[21] = 1.0f; m_batchData[22] = 1.0f; m_batchData[23] = 0.0f; 
					m_batchData[29] = 1.0f; m_batchData[30] = 1.0f;  m_batchData[31] = 0.0f; 

					OnHover();
				}
				break;
				default:
					break;
			}
		}
		else
		{
			math::Vector2f uvLeftUp = m_releasedRect.upperLeftCorner;
					math::Vector2f uvRightDown = m_releasedRect.lowerRightCorner;

					m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
					m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
					m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
					m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 

					// color
					m_batchData[5] = 1.0f; m_batchData[6] = 1.0f; m_batchData[7] = 1.0f;
					m_batchData[13] = 1.0f; m_batchData[14] = 1.0f; m_batchData[15] = 1.0f; 
					m_batchData[21] = 1.0f; m_batchData[22] = 1.0f; m_batchData[23] = 1.0f; 
					m_batchData[29] = 1.0f; m_batchData[30] = 1.0f;  m_batchData[31] = 1.0f; 
		}
		
		return false;
	}

} // gui

} // sh