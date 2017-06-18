#include "MainWindow.h"

#include <Windows.h>
#include <Commdlg.h>
#include <tchar.h>

using namespace std::placeholders;

MainWindow::MainWindow()
{
	sh::Device::GetInstance()->mouseEvent.Connect(std::bind(&MainWindow::OnMouseEvent, this, _1, _2, _3, _4));
	sh::Device::GetInstance()->mouseWheelEvent.Connect(std::bind(&MainWindow::OnMouseWeelEvent, this, _1));
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

	////////////////////////////////////////////////////////////////////////////

	const auto& styleFileInfo = fileSystem->FindFile("editor_style.xml");
	if (styleFileInfo.name != "")
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(styleFileInfo.absolutePath.c_str());
		pugi::xml_node root = doc.first_child();

		sh::gui::StylePtr style(new sh::gui::Style());
		style->Load(root);

		guiMgr->SetStyle(style);
	}

	////////////////////////////////////////////////////////////////////////////


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

	
	sh::gui::WindowPtr window(new sh::gui::Window(
		sh::math::Rectu(100, 100, 300, 300),
		releasedSprite));
	window->SetText("Inspector");

	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	sh::math::Rectu r(400, 200, 500, 300);
	sh::gui::ButtonPtr btn1(new sh::gui::Button(r, releasedSprite, pressedSprite, redSprite));
	sh::gui::HorizontalLayoutPtr childLayout(new sh::gui::HorizontalLayout());
	sh::gui::ButtonPtr btn2(new sh::gui::Button(r, releasedSprite, pressedSprite, redSprite));
	sh::gui::ButtonPtr btn3(new sh::gui::Button(r, releasedSprite, pressedSprite, redSprite));
	sh::gui::ButtonPtr btn4(new sh::gui::Button(r, releasedSprite, pressedSprite, redSprite));
	childLayout->AddWidget(btn2);
	childLayout->AddWidget(btn3);
	childLayout->AddWidget(btn4);
	sh::gui::ButtonPtr btn5(new sh::gui::Button(r, releasedSprite, pressedSprite, redSprite));
	layout->AddWidget(btn1);
	layout->AddLayout(childLayout);
	layout->AddWidget(btn5);

	window->SetLayout(layout);

	guiMgr->AddChild(window);
	guiMgr->AddChild(lineEdit);
	guiMgr->AddChild(floatLineEdit);
	guiMgr->AddChild(m_toolBar);
	guiMgr->AddChild(m_menuBar);
	
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::Update()
{
	sh::Device* device = sh::Device::GetInstance();
	sh::InputManager* inputManager = device->GetInputManager();
	sh::math::Vector2i old = inputManager->GetMousePositionOld();
	sh::math::Vector2i current = inputManager->GetMousePositionCurrent();
	sh::scene::Camera* camera = device->GetSceneManager()->GetCamera();


	if (inputManager->IsMouseButtonPressed(sh::MouseCode::ButtonWheel))
	{
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_MENU))
		{
			const sh::math::Vector4u& viewport = device->GetDriver()->GetViewPort();
			sh::math::Vector3f targetPos(0.0f);
			if (m_cameraTargetEntity)
			{
				sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>( m_cameraTargetEntity->GetComponent(sh::Component::Type::TRANSFORM) );
				targetPos = transformComponent->GetPosition();
			}

			sh::math::Vector2i delta = current - old;
			
			float yAxisAngle = -(float)delta.x * 0.01f;
			sh::math::Quaternionf yAxisRot;
			yAxisRot.SetFromAxisAngle(camera->GetUpVector(), yAxisAngle);

			float xAxisangle = -(float)delta.y * 0.01f;
			sh::math::Quaternionf xAxisRot;
			xAxisRot.SetFromAxisAngle(camera->GetRightVector(), xAxisangle);

			sh::math::Quaternionf deltaRot = yAxisRot * xAxisRot;

			sh::math::Vector3f baseVec = camera->GetPosition() - targetPos;
			float length = baseVec.GetLength();
			baseVec.Normalize();
			sh::math::Vector3f targetVec = deltaRot * baseVec;
			camera->SetPosition(targetPos + targetVec * length);

			camera->SetRotation(deltaRot * camera->GetRotation());
		}
		else
		{
			sh::math::Vector3f targetPos(0.0f);
			if (m_cameraTargetEntity)
			{
				sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>( m_cameraTargetEntity->GetComponent(sh::Component::Type::TRANSFORM) );
				targetPos = transformComponent->GetPosition();
				sh::math::Planef plane(targetPos, camera->GetFrontVector() * ( -1.0f ));

				sh::math::Vector3f rayOrigin, rayDirOld, rayDirCurrent;
				camera->BuildRay(old.x, old.y, rayOrigin, rayDirOld);
				camera->BuildRay(current.x, current.y, rayOrigin, rayDirCurrent);
				sh::math::Vector3f intersectionOld(0.0f), intersectionCurrent(0.0f);
				plane.GetIntersectionWithLine(rayOrigin, rayDirOld, intersectionOld);
				plane.GetIntersectionWithLine(rayOrigin, rayDirCurrent, intersectionCurrent);
				sh::math::Vector3f delta = intersectionCurrent - intersectionOld;
				camera->SetPosition(camera->GetPosition() - delta);
			}
			else
			{
				sh::math::Vector2i delta = current - old;
				sh::math::Vector3f cameraUpMove = camera->GetUpVector() * delta.y * 0.1f;
				sh::math::Vector3f cameraRightMove = camera->GetRightVector() * ( -delta.x ) * 0.1f;
				sh::math::Vector3f cameraDeltaMove = cameraUpMove + cameraRightMove;
				//camera->SetPosition(camera->GetPosition() + cameraDeltaMove);
			}
		}
	}

	inputManager->SetMousePositionOld(current);
	///////////////////////////////////////////////////////////////

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
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter =
		"XML files (*.xml)\0*.xml\0"
		"All files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open scene";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
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

	if (code == sh::MouseCode::ButtonWheel)
	{
		switch (type)
		{
			case sh::MouseEventType::ButtonPressed:
			{
				const auto& picker = sh::Device::GetInstance()->GetSceneManager()->GetPicker();
				m_cameraTargetEntity = picker->TryToPick(x, y, 640, 480);
			}
			break;
			case sh::MouseEventType::ButtonReleased:
				m_cameraTargetEntity = nullptr;
				break;
			default:
				break;
		}

		return;
	}

	if (m_gizmo->IsEnabled())
	{
		m_gizmo->OnMouseEvent(x, y, type, code);
		return;
	}

	switch (type)
	{
		case sh::MouseEventType::ButtonPressed:
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
			}
		}

		break;
		case sh::MouseEventType::Moved:
			break;
		default:
			break;
	}
}

void MainWindow::OnMouseWeelEvent(int d)
{
	float delta = d > 0 ? 0.5f : -0.5f;

	auto camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();

	const auto& cameraPos = camera->GetPosition();
	const auto& cameraFront = camera->GetFrontVector();

	camera->SetPosition(cameraPos + cameraFront * delta);

	/*

	math::Vector2i old = inputManager->GetMousePositionOld();
	math::Vector2i current = inputManager->GetMousePositionCurrent();
	if (inputManager->IsMouseButtonPressed(MouseCode::ButtonRight))
	{

	math::Vector2i delta = current - old;
	float xAngle = (float)delta.x * 0.01f;
	float yAngle = (float)delta.y * 0.01f;

	math::Quaternionf xRot;
	xRot.SetFromAxisAngle(SceneManager::GetUpVector(), xAngle);

	math::Quaternionf yRot;
	yRot.SetFromAxisAngle(m_rightVector, yAngle);

	SetRotation(xRot * yRot * m_rotation);
	}
	*/
}

void MainWindow::OnKeyboardEvent(sh::KeyboardEventType type, sh::KeyCode code)
{
	if (sh::gui::GuiManager::GetInstance()->ProcessKeyboardInput(type, code))
		return;

	if (type == sh::KeyboardEventType::KeyPressed)
		sh::Device::GetInstance()->GetInputManager()->SetKeyPressed(code);
	else
		sh::Device::GetInstance()->GetInputManager()->SetKeyReleased(code);
}

void MainWindow::OnWindowResized(int width, int height)
{

}