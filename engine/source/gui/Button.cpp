#include "Button.h"

#include "GuiManager.h"

#include "../scene/SceneManager.h"
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

		///*
		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2f extends(viewPort.z, viewPort.w);

		math::Vector2f leftUp;
		leftUp.x = rect.upperLeftCorner.x * 2.0f / extends.x - 1.0f;
		leftUp.y = (extends.y - rect.upperLeftCorner.y) * 2.0f / extends.y - 1.0f;

		math::Vector2f rightDown;
		rightDown.x = rect.lowerRightCorner.x * 2.0f / extends.x - 1.0f;
		rightDown.y = (extends.y - rect.lowerRightCorner.y) * 2.0f / extends.y - 1.0f;

		std::vector<float> vertices = 
		{
			leftUp.x, leftUp.y, 0.0f,			0.0f, 0.0f,		0.5f, 0.5f, 0.5f,
			leftUp.x, rightDown.y, 0.0f,		1.0f, 0.0f,		0.5f, 0.5f, 0.5f,
			rightDown.x, rightDown.y, 0.0f,		1.0f, 1.0f,		0.5f, 0.5f, 0.5f,
			rightDown.x, leftUp.y, 0.0f,		0.0f, 1.0f,		0.5f, 0.5f, 0.5f
		};

		std::vector<u32> indices = 
		{
			0, 1, 2,
			0, 2, 3
		};

		
		// Create vertex buffer
		auto& vertexBuffer = GuiManager::GetInstance()->m_mainBatch.vertexBuffer;
		const void* verticesPointer = vertices.data();
		size_t verticesDataSize = vertices.size() * sizeof(float);
		vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
		vertexBuffer->SetVerticesCount(4);

		// Create index buffer
		auto& indexBuffer = GuiManager::GetInstance()->m_mainBatch.indexBuffer;
		const void* indicesPointer = indices.data();
		size_t indicesDataSize = indices.size() * sizeof(unsigned int);
		indexBuffer->SetData(0, indicesDataSize, indicesPointer);
		indexBuffer->SetIndicesCount(indices.size());
		//*/
	}

	Button::Button()
	{
		m_batchData.reserve(100U);


		std::vector<u32> indices = 
		{
			0, 1, 2,
			0, 2, 3
		};

		// Create index buffer
		auto& indexBuffer = GuiManager::GetInstance()->m_mainBatch.indexBuffer;
		const void* indicesPointer = indices.data();
		size_t indicesDataSize = indices.size() * sizeof(unsigned int);
		indexBuffer->SetData(0, indicesDataSize, indicesPointer);
		indexBuffer->SetIndicesCount(indices.size());
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

		math::Vector2f leftUp;
		leftUp.x = m_rect.upperLeftCorner.x * 2.0f / extends.x - 1.0f;
		leftUp.y = (extends.y - m_rect.upperLeftCorner.y) * 2.0f / extends.y - 1.0f;

		math::Vector2f rightDown;
		rightDown.x = m_rect.lowerRightCorner.x * 2.0f / extends.x - 1.0f;
		rightDown.y = (extends.y - m_rect.lowerRightCorner.y) * 2.0f / extends.y - 1.0f;

		math::Vector2f uvLeftUp = m_releasedRect.upperLeftCorner;
		math::Vector2f uvRightDown = m_releasedRect.lowerRightCorner;

		std::vector<float> vertices = 
		{
			leftUp.x, leftUp.y, 0.0f,			uvLeftUp.x, uvLeftUp.y,		0.5f, 0.5f, 0.5f,
			leftUp.x, rightDown.y, 0.0f,		uvLeftUp.x, uvRightDown.y,		0.5f, 0.5f, 0.5f,
			rightDown.x, rightDown.y, 0.0f,		uvRightDown.x, uvRightDown.y,		0.5f, 0.5f, 0.5f,
			rightDown.x, leftUp.y, 0.0f,		uvRightDown.x, uvLeftUp.y,		0.5f, 0.5f, 0.5f
		};

		m_batchData = vertices;

		auto& vertexBuffer = GuiManager::GetInstance()->m_mainBatch.vertexBuffer;
		const void* verticesPointer = m_batchData.data();
		size_t verticesDataSize = m_batchData.size() * sizeof(float);
		vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
		vertexBuffer->SetVerticesCount(4);
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
			/*
			math::Vector3f color(0.0f);
			const auto& decl = GuiManager::GetInstance()->m_mainBatch.vertexBuffer->GetVertexDeclaration();
			u32 offset = decl->GetAttribute(AttributeSemantic::COLOR)->offset;
			u32 stride = decl->GetStride();
			*/
			switch (type)
			{
				case MouseEventType::ButtonPressed:
				{
					/*
					color = math::Vector3f(0.0f, 0.0f, 0.0f);
					for (u32 i = 0U; i < 4; ++i)
					{
						GuiManager::GetInstance()->m_mainBatch.vertexBuffer->SetData(stride * i + offset, sizeof(color), &color.x);
					}
					*/

					
					math::Vector2f uvLeftUp = m_pressedRect.upperLeftCorner;
					math::Vector2f uvRightDown = m_pressedRect.lowerRightCorner;

					m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
					m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
					m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
					m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 

					auto& vertexBuffer = GuiManager::GetInstance()->m_mainBatch.vertexBuffer;
					const void* verticesPointer = m_batchData.data();
					size_t verticesDataSize = m_batchData.size() * sizeof(float);
					vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
					vertexBuffer->SetVerticesCount(4);

					pressed();
				}
					break;
				case MouseEventType::ButtonReleased:
				{
					/*
					color = math::Vector3f(0.5f, 0.5f, 0.5f);
					for (u32 i = 0U; i < 4; ++i)
					{
						GuiManager::GetInstance()->m_mainBatch.vertexBuffer->SetData(stride * i + offset, sizeof(color), &color.x);
					}
					*/

					math::Vector2f uvLeftUp = m_releasedRect.upperLeftCorner;
					math::Vector2f uvRightDown = m_releasedRect.lowerRightCorner;

					m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
					m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
					m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
					m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 

					auto& vertexBuffer = GuiManager::GetInstance()->m_mainBatch.vertexBuffer;
					const void* verticesPointer = m_batchData.data();
					size_t verticesDataSize = m_batchData.size() * sizeof(float);
					vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
					vertexBuffer->SetVerticesCount(4);

					released();
				}
					break;
				default:
					break;
			}
		}
		
		return false;
	}

} // gui

} // sh