#include "Button.h"

#include "GuiManager.h"

#include "../scene/SceneManager.h"
#include "../video/VertexDeclaration.h"
#include "../video/Driver.h"
#include "../Device.h"


#include <Windows.h>
#include <Commdlg.h>
#include <tchar.h>

namespace sh
{

namespace gui
{
	Button::Button(const math::Rectf rect)
	{
		m_rect = rect;

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
		auto& vertexBuffer = GuiManager::GetInstance()->m_vertexBuffer;
		const void* verticesPointer = vertices.data();
		size_t verticesDataSize = vertices.size() * sizeof(float);
		vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
		vertexBuffer->SetVerticesCount(4);

		// Create index buffer
		auto& indexBuffer = GuiManager::GetInstance()->m_indexBuffer;
		const void* indicesPointer = indices.data();
		size_t indicesDataSize = indices.size() * sizeof(unsigned int);
		indexBuffer->SetData(0, indicesDataSize, indicesPointer);
		indexBuffer->SetIndicesCount(indices.size());
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
		bool openDialog = false;
		if (inside)
		{
			math::Vector3f color(0.0f);
			const auto& decl = GuiManager::GetInstance()->m_vertexBuffer->GetVertexDeclaration();
			u32 offset = decl->GetAttribute(AttributeSemantic::COLOR)->offset;
			u32 stride = decl->GetStride();

			switch (type)
			{
				case MouseEventType::ButtonPressed:
				{
					color = math::Vector3f(0.0f, 0.0f, 0.0f);
					for (u32 i = 0U; i < 4; ++i)
					{
						GuiManager::GetInstance()->m_vertexBuffer->SetData(stride * i + offset, sizeof(color), &color.x);
					}
					pressed();
				}
					break;
				case MouseEventType::ButtonReleased:
				{
					color = math::Vector3f(0.5f, 0.5f, 0.5f);
					for (u32 i = 0U; i < 4; ++i)
					{
						GuiManager::GetInstance()->m_vertexBuffer->SetData(stride * i + offset, sizeof(color), &color.x);
					}
					released();
					openDialog = true;
				}
					break;
				default:
					break;
			}
		}
		else
		{
			return false;
		}

		if (!openDialog)
			return false;

		HWND hWnd = (HWND)Device::GetInstance()->GetWinId();

	
		char szFileName[MAX_PATH] = "";

		OPENFILENAME ofn;
		ZeroMemory( &ofn , sizeof( ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = 
			"XML files (*.xml)\0*.xml\0"
			"All files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Open scene";
		ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "xml";
		if (GetOpenFileName(&ofn))
		{
			sh::scene::SceneManager* sceneMgr = sh::Device::GetInstance()->GetSceneManager();		
			sceneMgr->LoadScene(ofn.lpstrFile);
		}
		else
		{
			int a = 0;
			a++;
		}

		return true;
	}

} // gui

} // sh