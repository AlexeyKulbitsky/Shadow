#include "MainWindow.h"

#include "selection/SelectionManager.h"
#include "gui/propertyeditors/PropertyEditorsFactory.h"

#include <Windows.h>
#include <Commdlg.h>
#include <tchar.h>

#include <jobs/Test.h>

#include <pugixml.hpp>

using namespace std::placeholders;

MainWindow::MainWindow()
{
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::Update()
{
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::NewScene()
{
	sh::scene::SceneManager* sceneMgr = sh::Device::GetInstance()->GetSceneManager();
	sceneMgr->ClearScene();
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
		sceneMgr->SaveScene(ofn.lpstrFile);
	}
}

void MainWindow::NewProject()
{
	HWND hWnd = (HWND)sh::Device::GetInstance()->GetWinId();

	char szFileName[MAX_PATH] = "";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = "Project files (*.proj)\0*.proj\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "New project";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "proj";
	if (GetSaveFileName(&ofn))
	{
		auto filesystem = sh::io::FileSystem::GetInstance();
		auto root = filesystem->GetRoot();
		sh::String projectFileName = ofn.lpstrFile;
		sh::String projectFolder = "";
		// Get project folder
		sh::String::size_type pos = projectFileName.find_last_of("\\/");
		if (pos != sh::String::npos)
			projectFolder = projectFileName.substr(0U, pos);

		// Create assets folder
		sh::String assetsFolder = projectFolder + "/assets";
		filesystem->CreateFolder(assetsFolder);

		// Create source folder
		sh::String sourceFolder = projectFolder + "/source";
		filesystem->CreateFolder(sourceFolder);
		auto mainTemplateFileInfo = filesystem->FindFile("main.cpp");
		if (!mainTemplateFileInfo.expired())
		{
			filesystem->Copy(mainTemplateFileInfo.lock()->absolutePath, sourceFolder + "/main.cpp");
		}

		// Create solution for Visual Studio
		auto cmakeTemplateFileInfo = filesystem->FindFile("CMakeLists_template.txt");
		if (!cmakeTemplateFileInfo.expired())
		{
			filesystem->Copy(cmakeTemplateFileInfo.lock()->absolutePath, sourceFolder + "/CMakeLists.txt");
			sh::String IDEKey = "\"Visual Studio 14\"";
			sh::String IDEProjectFolder = projectFolder + "/project";

			sh::String systemCommandString = "cmake -E make_directory " + IDEProjectFolder + " && cmake -E chdir " + IDEProjectFolder + " cmake -G " + IDEKey + " ../source";
			system(systemCommandString.c_str());
		}

		// Save project
		pugi::xml_document doc;
		pugi::xml_node sceneNode = doc.append_child("project");
		sceneNode.append_attribute("name").set_value("Test project");

		

		doc.save_file(ofn.lpstrFile);
	}
}

void MainWindow::OpenProject()
{
	HWND hWnd = (HWND)sh::Device::GetInstance()->GetWinId();

	char szFileName[MAX_PATH] = "";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = "Project files (*.proj)\0*.proj\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open project";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "proj";
	if (GetOpenFileName(&ofn))
	{
		auto filesystem = sh::io::FileSystem::GetInstance();
		auto root = filesystem->GetRoot();
		sh::String projectFileName = ofn.lpstrFile;
		sh::String projectFolder = "";
		// Get project folder
		sh::String::size_type pos = projectFileName.find_last_of("\\/");
		if (pos != sh::String::npos)
			projectFolder = projectFileName.substr(0U, pos);

		// Set assets folder for file system
		filesystem->AddFolder(projectFolder + "/assets");
		
		// Refresh assets list
		m_assetsWidget->RefreshAssetsList();

		// Load Game as dynamic library
		sh::String gameLibraryPath = projectFolder + "/bin/Debug/GameLibrary.dll";
		
		m_gameModuleLibrary = new sh::DynamicLibrary(gameLibraryPath);
		SH_ASSERT(m_gameModuleLibrary->Load(), "Can not load Game module!");


		auto CreateGameModulePtr = reinterpret_cast<sh::Application*(*)()>(m_gameModuleLibrary->GetSymbol("CreateGameModule"));
		SH_ASSERT(CreateGameModulePtr, "Can not load CreateGameModule() function address");
		if (!CreateGameModulePtr)
			return;
		m_gameModule = CreateGameModulePtr();

		auto SetDevicePtr = reinterpret_cast<void(*)(sh::Device*)>(m_gameModuleLibrary->GetSymbol("SetDevice"));
		SH_ASSERT(CreateGameModulePtr, "Can not load SetDevice(sh::Device*) function address");
		if (!CreateGameModulePtr)
			return;
		SetDevicePtr(sh::Device::GetInstance());
	}
}

void MainWindow::SaveProject()
{
}

void MainWindow::Close()
{
	SetNeedsToBeClosed(true);
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
	m_mainWidget->SetRect(sh::math::Rect(0, 0, width, height));
}

void MainWindow::Init()
{
	// Add internal editor resources
	auto filesystem = sh::io::FileSystem::GetInstance();
	filesystem->AddInternalFolder(sh::String("../../../editor/data"));


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

	PropertyEditorsFactory::CreateInstance();

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


	//sh::SPtr<sh::gui::Widget> colorPicker(new ColorPicker());
	//guiMgr->AddChild(colorPicker);

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
	m_mainWidget->SetRect(sh::math::Rect(0, 0, viewport.lowerRightCorner.x, viewport.lowerRightCorner.y));
}

void MainWindow::Destroy()
{
	if (m_gameModuleLibrary)
	{
		if (m_gameModule)
		{
			auto DestroyGameModulePtr = reinterpret_cast<void(*)()>(m_gameModuleLibrary->GetSymbol("DestroyGameModule"));
			SH_ASSERT(DestroyGameModulePtr, "Can not load DestroyGameModule() function address");
			if (!DestroyGameModulePtr)
				return;
			DestroyGameModulePtr();
		}
		m_gameModuleLibrary->Unload();
		delete m_gameModuleLibrary;
	}
	

	SelectionManager::DestroyInstance();
	PropertyEditorsFactory::DestroyInstance();
}

void MainWindow::Update(sh::u64 delta)
{
	sh::Device* device = sh::Device::GetInstance();
	sh::InputManager* inputManager = device->GetInputManager();
	sh::math::Vector2Int old = inputManager->GetMousePositionOld();
	sh::math::Vector2Int current = inputManager->GetMousePositionCurrent();
	sh::scene::Camera* camera = device->GetSceneManager()->GetCamera();


	if (inputManager->IsMouseButtonPressed(sh::MouseCode::ButtonWheel))
	{
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_MENU))
		{
			const sh::math::Vector4& viewport = device->GetDriver()->GetViewPort();
			sh::math::Vector3 targetPos(0.0f);
			if (m_cameraTargetEntity)
			{
				auto transformComponent = m_cameraTargetEntity->GetComponent<sh::TransformComponent>();
				targetPos = transformComponent->GetPosition();
			}

			sh::math::Vector2Int delta = current - old;

			float yAxisAngle = -(float)delta.x * 0.01f;
			sh::math::Quaternion yAxisRot;
			//yAxisRot.SetFromAxisAngle(camera->GetUpVector(), yAxisAngle);
			yAxisRot.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), yAxisAngle);

			float xAxisangle = -(float)delta.y * 0.01f;
			sh::math::Quaternion xAxisRot;
			xAxisRot.SetFromAxisAngle(camera->GetRightVector(), xAxisangle);

			sh::math::Quaternion deltaRot = yAxisRot * xAxisRot;

			sh::math::Vector3 baseVec = camera->GetPosition() - targetPos;
			float length = baseVec.GetLength();
			baseVec.Normalize();
			sh::math::Vector3 targetVec = deltaRot * baseVec;
			camera->SetPosition(targetPos + targetVec * length);

			camera->SetRotation(deltaRot * camera->GetRotation());
		}
		else
		{
			sh::math::Vector3 targetPos(0.0f);
			if (m_cameraTargetEntity)
			{
				auto transformComponent = m_cameraTargetEntity->GetComponent<sh::TransformComponent>();
				targetPos = transformComponent->GetPosition();
				sh::math::Plane plane(targetPos, camera->GetFrontVector() * (-1.0f));

				sh::math::Vector3 rayOrigin, rayDirOld, rayDirCurrent;
				camera->BuildRay(old.x, old.y, rayOrigin, rayDirOld);
				camera->BuildRay(current.x, current.y, rayOrigin, rayDirCurrent);
				sh::math::Vector3 intersectionOld(0.0f), intersectionCurrent(0.0f);
				plane.GetIntersectionWithLine(rayOrigin, rayDirOld, intersectionOld);
				plane.GetIntersectionWithLine(rayOrigin, rayDirCurrent, intersectionCurrent);
				sh::math::Vector3 delta = intersectionCurrent - intersectionOld;
				camera->SetPosition(camera->GetPosition() - delta);
			}
			else
			{
				sh::math::Vector2Int delta = current - old;
				sh::math::Vector3 cameraUpMove = camera->GetUpVector() * static_cast<float>(delta.y) * 0.1f;
				sh::math::Vector3 cameraRightMove = camera->GetRightVector() * static_cast<float>(-delta.x) * 0.1f;
				sh::math::Vector3 cameraDeltaMove = cameraUpMove + cameraRightMove;
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

	// Updating game module if exists
	if (m_gameModule)
		m_gameModule->Update(delta);
}

sh::gui::MenuBarPtr MainWindow::CreateMenuBar()
{
	// Menu bar
	sh::gui::MenuBarPtr menuBar(new sh::gui::MenuBar());
	sh::gui::ButtonPtr menuButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	sh::gui::ButtonPtr editButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	menuButton->SetToggleable(true);
	const auto& fileMenu = menuBar->AddMenu("File", menuButton);
	const auto& editMenu = menuBar->AddMenu("Edit", editButton);

	sh::gui::ButtonPtr newSceneButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	newSceneButton->SetText("New scene");
	newSceneButton->OnRelease.Connect(std::bind(&MainWindow::NewScene, this));
	fileMenu->AddItem(newSceneButton);

	sh::gui::ButtonPtr openSceneButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	openSceneButton->SetText("Open scene");
	openSceneButton->OnRelease.Connect(std::bind(&MainWindow::OpenScene, this));
	fileMenu->AddItem(openSceneButton);

	sh::gui::ButtonPtr saveSceneButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	saveSceneButton->SetText("Save scene...");
	saveSceneButton->OnRelease.Connect(std::bind(&MainWindow::SaveScene, this));
	fileMenu->AddItem(saveSceneButton);

	sh::gui::ButtonPtr newProjectButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	newProjectButton->SetText("New project");
	newProjectButton->OnRelease.Connect(std::bind(&MainWindow::NewProject, this));
	fileMenu->AddItem(newProjectButton);

	sh::gui::ButtonPtr openProjectButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	openProjectButton->SetText("Open project");
	openProjectButton->OnRelease.Connect(std::bind(&MainWindow::OpenProject, this));
	fileMenu->AddItem(openProjectButton);

	sh::gui::ButtonPtr saveProjectButton(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	saveProjectButton->SetText("Save project...");
	saveProjectButton->OnRelease.Connect(std::bind(&MainWindow::SaveProject, this));
	fileMenu->AddItem(saveProjectButton);

	sh::gui::ButtonPtr exitButton1(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	exitButton1->SetText("Exit");
	exitButton1->OnRelease.Connect(std::bind(&MainWindow::Close, this));
	fileMenu->AddItem(exitButton1);

	////////////////////////////////////////////////////////

	sh::gui::ButtonPtr edit1Button(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	edit1Button->SetText("Edit 1");
	editMenu->AddItem(edit1Button);

	sh::gui::ButtonPtr edit2Button(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	edit2Button->SetText("Edit 2");
	editMenu->AddItem(edit2Button);

	sh::gui::MenuPtr subMenu(new sh::gui::Menu());
	sh::gui::ButtonPtr sub1Button(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	sub1Button->SetText("Submenu 1");
	subMenu->AddItem(sub1Button);

	sh::gui::ButtonPtr sub2Button(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
	sub2Button->SetText("Submenu 2");
	subMenu->AddItem(sub2Button);

	sh::gui::ButtonPtr sub3Button(new sh::gui::Button(sh::math::Rect(0, 0, 50, 15)));
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
