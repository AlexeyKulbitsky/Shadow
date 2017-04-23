#include "Button.h"

#include "GuiManager.h"

#include "../scene/SceneManager.h"

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
			leftUp.x, leftUp.y, 0.0f,		0.0f, 0.0f,
			leftUp.x, rightDown.y, 0.0f,		1.0f, 0.0f,
			rightDown.x, rightDown.y, 0.0f,		1.0f, 1.0f,
			rightDown.x, leftUp.y, 0.0f,		0.0f, 1.0f
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

	bool Button::ProcessInput(u32 x, u32 y)
	{
		///*
		HWND hWnd = (HWND)Device::GetInstance()->GetWinId();

		bool inside = m_rect.IsPointInside(float(x), float(y));
		if (!inside)
			return false;
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
		//*/
		return true;
	}

} // gui

} // sh