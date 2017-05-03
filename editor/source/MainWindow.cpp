#include "MainWindow.h"

#include <Windows.h>
#include <Commdlg.h>
#include <tchar.h>

using namespace std::placeholders;

MainWindow::MainWindow()
{
	sh::Device::GetInstance()->mouseEvent.Connect(std::bind(&MainWindow::OnMouseEvent, this, _1, _2, _3, _4));
	sh::Device::GetInstance()->keyboardEvent.Connect(std::bind(&MainWindow::OnKeyboardEvent, this, _1, _2));

	//m_defaultGizmo.reset(new Gizmo());
	m_moveGizmo.reset(new MoveGizmo());

	m_gizmo = m_moveGizmo;

	auto fileSystem = sh::Device::GetInstance()->GetFileSystem();

	const auto& fileInfo = fileSystem->FindFile("editor_gui.xml");

	auto guiMgr = sh::gui::GuiManager::GetInstance();

	if (fileInfo.name != "")
	{
		guiMgr->LoadGui(fileInfo.absolutePath.c_str());
	}

	auto font = sh::FontManager::GetInstance()->GenerateFont("VeraMono.ttf");
	guiMgr->SetFont(font);

	auto releasedSprite = sh::gui::SpriteManager::GetInstance()->GetSprite("DefaultButton");
	auto pressedSprite = sh::gui::SpriteManager::GetInstance()->GetSprite("PressedButton");
	auto redSprite = sh::gui::SpriteManager::GetInstance()->GetSprite("RedButton");
	auto moveGizmoSprite = sh::gui::SpriteManager::GetInstance()->GetSprite("MoveGizmo");

	m_menuBar.reset(new sh::gui::MenuBar(releasedSprite));
	sh::gui::ButtonPtr menuButton(new sh::gui::Button(
		sh::math::Rectu(0, 0, 50, 15), 
		releasedSprite,
		pressedSprite,
		redSprite));
	menuButton->SetToggleable(true);
	const auto& fileMenu = m_menuBar->AddMenu("File", menuButton);

	sh::gui::ButtonPtr openSceneButton(new sh::gui::Button(
		sh::math::Rectu(0, 0, 50, 15), 
		releasedSprite,
		pressedSprite,
		redSprite));
	openSceneButton->SetText("Open scene...");
	openSceneButton->OnRelease.Connect(std::bind(&MainWindow::OpenScene, this));
	fileMenu->AddItem(openSceneButton);

	sh::gui::ButtonPtr saveSceneButton(new sh::gui::Button(
		sh::math::Rectu(0, 0, 50, 15), 
		releasedSprite,
		pressedSprite,
		redSprite));
	saveSceneButton->SetText("Save scene...");
	saveSceneButton->OnRelease.Connect(std::bind(&MainWindow::SaveScene, this));
	fileMenu->AddItem(saveSceneButton);

	sh::gui::ButtonPtr exitButton(new sh::gui::Button(
		sh::math::Rectu(0, 0, 50, 15), 
		releasedSprite,
		pressedSprite,
		redSprite));
	exitButton->SetText("Exit");
	exitButton->OnRelease.Connect(std::bind(&MainWindow::Close, this));
	fileMenu->AddItem(exitButton);


	m_toolBar.reset(new sh::gui::ToolBar());
	sh::gui::ButtonPtr moveGIzmo(new sh::gui::Button(
		sh::math::Rectu(0, 0, 50, 15), 
		moveGizmoSprite,
		pressedSprite,
		redSprite));
	m_toolBar->AddItem(moveGIzmo);

	sh::gui::LineEditPtr lineEdit(new sh::gui::LineEdit(
		sh::math::Rectu(0, 50, 150, 65), 
		releasedSprite,
		pressedSprite));

	sh::gui::FloatLineEditPtr floatLineEdit(new sh::gui::FloatLineEdit(
		sh::math::Rectu(0, 65, 150, 80), 
		releasedSprite,
		pressedSprite));

	guiMgr->AddChild(lineEdit);
	guiMgr->AddChild(floatLineEdit);
	guiMgr->AddChild(m_toolBar);
	guiMgr->AddChild(m_menuBar);
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::Update()
{
	auto driver = sh::Device::GetInstance()->GetDriver();
	auto sceneMgr = sh::Device::GetInstance()->GetSceneManager();

	driver->BeginRendering();

	sceneMgr->Update();
	m_gizmo->Render();
			
	sh::gui::GuiManager::GetInstance()->Render();

	driver->EndRendering();
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::OpenScene()
{
	HWND hWnd = (HWND)sh::Device::GetInstance()->GetWinId();

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
}

void MainWindow::SaveScene()
{

}

void MainWindow::Close()
{

}

void MainWindow::OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code)
{
	if (sh::gui::GuiManager::GetInstance()->ProcessInput(x, y, type))
		return;

	switch (type)
	{
		case sh::MouseEventType::ButtonPressed:
		{
			if (code == sh::MouseCode::ButtonLeft)
			{
				if (m_gizmo->IsEnabled())
				{
					m_gizmo->OnMousePressed(x, y);
				}
			}
		}
			break;
		case sh::MouseEventType::ButtonReleased:
		{
			if (code == sh::MouseCode::ButtonLeft)
			{
				if (!m_gizmo->IsEnabled())
				{
					const auto& picker = sh::Device::GetInstance()->GetSceneManager()->GetPicker();
					auto result = picker->TryToPick(x, y, 640, 480);
					m_gizmo->SetEntity(result);
				}
				else
				{
					m_gizmo->OnMouseReleased(x, y);
				}
			}
		}
			break;
		case sh::MouseEventType::Moved:
		{
			if (code == sh::MouseCode::ButtonLeft)
			{
				if (m_gizmo->IsEnabled())
				{
					m_gizmo->OnMouseMoved(x, y);
				}
			}
			
		}
			break;
		default:
			break;
	}
}

void MainWindow::OnKeyboardEvent(sh::KeyboardEventType type, sh::KeyCode code)
{
	if (sh::gui::GuiManager::GetInstance()->ProcessKeyboardInput(type, code))
		return;
}