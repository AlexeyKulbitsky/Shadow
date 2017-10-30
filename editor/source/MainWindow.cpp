#include "MainWindow.h"

#include "selection/SelectionManager.h"

#include <Windows.h>
#include <Commdlg.h>
#include <tchar.h>

#include <jobs/Test.h>

using namespace std::placeholders;

MainWindow::MainWindow()
{
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::Update()
{
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
		sh::String fullPath(ofn.lpstrFile);
		size_t pos = fullPath.find_last_of('\\');
		sceneMgr->LoadScene(fullPath.substr(pos + 1).c_str());

		sh::u32 entitiesCount = sceneMgr->GetEntitiesCount();
		for (sh::u32 i = 0U; i < entitiesCount; ++i)
		{
			auto entity = sceneMgr->GetEntity(i);
			m_hierarchyWidget->AddEntity(entity);
		}
	}
}

void MainWindow::SaveScene()
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
	ofn.lpstrTitle = "Save scene";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "xml";
	if (GetSaveFileName(&ofn))
	{
		sh::scene::SceneManager* sceneMgr = sh::Device::GetInstance()->GetSceneManager();
		//sceneMgr->SaveScene(ofn.lpstrFile);
	}
}

void MainWindow::Close()
{

}

void MainWindow::OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code)
{
	sh::gui::GUIEvent ev;
	switch (type)
	{
	case sh::MouseEventType::ButtonPressed:
		ev.type = sh::gui::EventType::PointerDown;
		break;
	case sh::MouseEventType::ButtonReleased:
		ev.type = sh::gui::EventType::PointerUp;
		break;
	case sh::MouseEventType::Moved:
		ev.type = sh::gui::EventType::PointerMove;
		break;
	case sh::MouseEventType::ButtonDoubleClicked:
		ev.type = sh::gui::EventType::PointerDoubleClick;
		break;
	}
	ev.x = x;
	ev.y = y;
	ev.mouseButtonCode = code;

	if (sh::gui::GuiManager::GetInstance()->ProcessEvent(ev))
		return;

	if (code == sh::MouseCode::ButtonWheel)
	{
		switch (type)
		{
			case sh::MouseEventType::ButtonPressed:
			{
				const auto& picker = sh::Device::GetInstance()->GetSceneManager()->GetPicker();
				m_cameraTargetEntity = picker->TryToPick(x, y);
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

	SelectionManager::GetInstance()->ProcessMouseEvent(x, y, type, code);
}

void MainWindow::OnMouseWeelEvent(int x, int y, int d)
{
	sh::gui::GUIEvent ev;
	ev.type = sh::gui::EventType::Wheel;
	ev.delta = d;
	ev.x = x;
	ev.y = y;
	if (sh::gui::GuiManager::GetInstance()->ProcessEvent(ev))
		return;

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
	sh::gui::GUIEvent ev;
	if (type == sh::KeyboardEventType::KeyPressed)
		ev.type = sh::gui::EventType::KeyDown;
	else
		ev.type = sh::gui::EventType::KeyUp;
	ev.keyCode = (int)code;


	if (sh::gui::GuiManager::GetInstance()->ProcessEvent(ev))
		return;

	if (type == sh::KeyboardEventType::KeyPressed)
		sh::Device::GetInstance()->GetInputManager()->SetKeyPressed(code);
	else
		sh::Device::GetInstance()->GetInputManager()->SetKeyReleased(code);
}

void MainWindow::OnWindowResized(int width, int height)
{
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(nullptr);
	m_mainWidget->SetRect(sh::math::Recti(0, 0, width, height));
}

void MainWindow::Init()
{
	sh::gui::GuiManager::CreateInstance();
	sh::gui::GuiManager::GetInstance()->Init();

	SelectionManager::CreateInstance();

	sh::Device::GetInstance()->mouseEvent.Connect(std::bind(&MainWindow::OnMouseEvent, this, _1, _2, _3, _4));
	sh::Device::GetInstance()->mouseWheelEvent.Connect(std::bind(&MainWindow::OnMouseWeelEvent, this, _1, _2, _3));
	sh::Device::GetInstance()->keyboardEvent.Connect(std::bind(&MainWindow::OnKeyboardEvent, this, _1, _2));
	sh::Device::GetInstance()->windowResizeEvent.Connect(std::bind(&MainWindow::OnWindowResized, this, _1, _2));

	auto fileSystem = sh::Device::GetInstance()->GetFileSystem();

	auto guiMgr = sh::gui::GuiManager::GetInstance();
	m_mainWidget.reset(new sh::gui::Widget());
	guiMgr->AddChild(m_mainWidget);
	guiMgr->LoadGui("editor_gui.xml");

	////////////////////////////////////////////////////////////////////////////

	sh::gui::StylePtr style(new sh::gui::Style());
	style->Load("editor_style.xml");
	guiMgr->SetStyle(style);

	////////////////////////////////////////////////////////////////////////////

	auto font = sh::FontManager::GetInstance()->GenerateFont("VeraMono.ttf");
	guiMgr->SetFont(font);

	////////////////////////////////////////////////////////////////////////////

	sh::gui::VerticalLayoutPtr mainVerticalLayout(new sh::gui::VerticalLayout());
	
	mainVerticalLayout->AddWidget(CreateMenuBar());
	mainVerticalLayout->AddWidget(CreateToolbar());

	// Inspector
	m_inspectorWidget.reset(new InspectorWidget());
	m_assetsWidget.reset(new AssetsWidget());
	m_hierarchyWidget.reset(new HierarchyWidget());
	
	SelectionManager::GetInstance()->SetHierarchyWidget(m_hierarchyWidget);
	SelectionManager::GetInstance()->SetInspectorWidget(m_inspectorWidget);

	sh::gui::VerticalLayoutPtr assetsHierarchyLayout(new sh::gui::VerticalLayout());
	assetsHierarchyLayout->AddWidget(m_hierarchyWidget);
	assetsHierarchyLayout->AddWidget(m_assetsWidget);

	sh::gui::VerticalLayoutPtr inspectorLayout(new sh::gui::VerticalLayout());
	inspectorLayout->AddWidget(m_inspectorWidget);

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());

	mainVerticalLayout->AddLayout(mainLayout);


	sh::SPtr<sh::gui::Widget> colorPicker(new ColorPicker());
	guiMgr->AddChild(colorPicker);

	mainLayout->AddLayout(assetsHierarchyLayout);
	sh::gui::WidgetPtr emptyWidget(new sh::gui::Widget());
	emptyWidget->SetVisible(false);
	emptyWidget->SetEnabled(false);
	mainLayout->AddWidget(emptyWidget);
	emptyWidget.reset(new sh::gui::Widget());
	emptyWidget->SetVisible(false);
	emptyWidget->SetEnabled(false);
	mainLayout->AddWidget(emptyWidget);
	mainLayout->AddLayout(inspectorLayout);
	m_mainWidget->SetLayout(mainVerticalLayout);
	auto viewport = sh::Device::GetInstance()->GetDriver()->GetViewport();
	m_mainWidget->SetRect(sh::math::Recti(0, 0, viewport.lowerRightCorner.x, viewport.lowerRightCorner.y));
}

void MainWindow::Destroy()
{
	SelectionManager::DestroyInstance();
}

void MainWindow::Update(sh::u64 delta)
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
				auto transformComponent = m_cameraTargetEntity->GetComponent<sh::TransformComponent>();
				targetPos = transformComponent->GetPosition();
			}

			sh::math::Vector2i delta = current - old;

			float yAxisAngle = -(float)delta.x * 0.01f;
			sh::math::Quaternionf yAxisRot;
			//yAxisRot.SetFromAxisAngle(camera->GetUpVector(), yAxisAngle);
			yAxisRot.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), yAxisAngle);

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
				auto transformComponent = m_cameraTargetEntity->GetComponent<sh::TransformComponent>();
				targetPos = transformComponent->GetPosition();
				sh::math::Planef plane(targetPos, camera->GetFrontVector() * (-1.0f));

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
				sh::math::Vector3f cameraUpMove = camera->GetUpVector() * static_cast<float>(delta.y) * 0.1f;
				sh::math::Vector3f cameraRightMove = camera->GetRightVector() * static_cast<float>(-delta.x) * 0.1f;
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

	SelectionManager::GetInstance()->Render();

	sh::gui::GuiManager::GetInstance()->Update(0U);
	sh::gui::GuiManager::GetInstance()->Render();

	driver->EndRendering();
}

sh::gui::MenuBarPtr MainWindow::CreateMenuBar()
{
	// Menu bar
	sh::gui::MenuBarPtr menuBar(new sh::gui::MenuBar());
	sh::gui::ButtonPtr menuButton(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	sh::gui::ButtonPtr editButton(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	menuButton->SetToggleable(true);
	const auto& fileMenu = menuBar->AddMenu("File", menuButton);
	const auto& editMenu = menuBar->AddMenu("Edit", editButton);

	sh::gui::ButtonPtr openSceneButton(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	openSceneButton->SetText("Open scene...");
	openSceneButton->OnRelease.Connect(std::bind(&MainWindow::OpenScene, this));
	fileMenu->AddItem(openSceneButton);

	sh::gui::ButtonPtr saveSceneButton(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	saveSceneButton->SetText("Save scene...");
	saveSceneButton->OnRelease.Connect(std::bind(&MainWindow::SaveScene, this));
	fileMenu->AddItem(saveSceneButton);

	sh::gui::ButtonPtr exitButton1(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	exitButton1->SetText("Exit");
	exitButton1->OnRelease.Connect(std::bind(&MainWindow::Close, this));
	fileMenu->AddItem(exitButton1);

	////////////////////////////////////////////////////////

	sh::gui::ButtonPtr edit1Button(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	edit1Button->SetText("Edit 1");
	editMenu->AddItem(edit1Button);

	sh::gui::ButtonPtr edit2Button(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	edit2Button->SetText("Edit 2");
	editMenu->AddItem(edit2Button);

	sh::gui::MenuPtr subMenu(new sh::gui::Menu());
	sh::gui::ButtonPtr sub1Button(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	sub1Button->SetText("Submenu 1");
	subMenu->AddItem(sub1Button);

	sh::gui::ButtonPtr sub2Button(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	sub2Button->SetText("Submenu 2");
	subMenu->AddItem(sub2Button);

	sh::gui::ButtonPtr sub3Button(new sh::gui::Button(sh::math::Recti(0, 0, 50, 15)));
	sub3Button->SetText("Submenu 3");
	subMenu->AddItem(sub3Button);

	editMenu->AddSubmenu("Edit 2", subMenu);

	return menuBar;
}

sh::gui::ToolBarPtr MainWindow::CreateToolbar()
{
	// Tool bar
	auto guiMgr = sh::gui::GuiManager::GetInstance();

	sh::gui::ButtonPtr moveGizmoButton = guiMgr->GetStyle()->GetButton("MoveGizmoButton");
	moveGizmoButton->SetToggleable(true);
	SelectionManager::GetInstance()->SetMoveButton(moveGizmoButton);

	sh::gui::ButtonPtr rotateGizmoButton = guiMgr->GetStyle()->GetButton("RotateGizmoButton");
	rotateGizmoButton->SetToggleable(true);
	SelectionManager::GetInstance()->SetRotateButton(rotateGizmoButton);

	sh::gui::ButtonPtr scaleGizmoButton = guiMgr->GetStyle()->GetButton("ScaleGizmoButton");
	scaleGizmoButton->SetToggleable(true);
	SelectionManager::GetInstance()->SetScaleButton(scaleGizmoButton);

	sh::gui::ButtonPtr arrowButton = guiMgr->GetStyle()->GetButton("ArrowButton");
	arrowButton->SetToggleable(true);
	SelectionManager::GetInstance()->SetArrowButton(arrowButton);

	sh::gui::ToolBarPtr toolBar(new sh::gui::ToolBar());
	toolBar->AddItem(arrowButton);
	toolBar->AddItem(moveGizmoButton);
	toolBar->AddItem(rotateGizmoButton);
	toolBar->AddItem(scaleGizmoButton);

	return toolBar;
}
