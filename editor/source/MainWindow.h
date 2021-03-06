#ifndef SHADOW_EDITOR_MAIN_WINDOW
#define SHADOW_EDITOR_MAIN_WINDOW

#include "gizmo/Gizmo.h"
#include "gizmo/MoveGizmo.h"
#include "gizmo/RotateGizmo.h"
#include "gizmo/ScaleGizmo.h"
#include "gui/inspectorwidget/InspectorWidget.h"
#include "gui/hierarchywidget/HierarchyWidget.h"
#include "gui/assetswidget/AssetsWidget.h"

#include "SceneWindow.h"


#include <Shadow.h>

class MainWindow : public sh::Application
{
public:
	MainWindow();

	void Update();

	void NewScene();
	void OpenScene();
    void OpenScene(const sh::String& path);
	void SaveScene();

	void NewProject();
	void OpenProject();
    void OpenProject(const sh::String& path);
	void SaveProject();

	void Close();

	void OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	void OnMouseWeelEvent(int x, int y, int delta);
	void OnKeyboardEvent(sh::KeyboardEventType type, sh::KeyCode code);
	void OnWindowResized(int width, int height);
    void OnSurfaceChanged(void*, int width, int height);

	// Application interface
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(sh::u64 delta) override;

private:
	sh::gui::MenuBarPtr CreateMenuBar();
	sh::gui::ToolBarPtr CreateToolbar();

private:
	sh::SPtr<InspectorWidget> m_inspectorWidget;
	sh::SPtr<HierarchyWidget> m_hierarchyWidget;
	sh::SPtr<AssetsWidget> m_assetsWidget;
    sh::SPtr<SceneWindow> m_sceneWindow;
	
	sh::Entity* m_cameraTargetEntity = nullptr;

	sh::gui::WidgetPtr m_mainWidget;

	// Active game module that is being edited
	sh::DynamicLibrary* m_gameModuleLibrary = nullptr;
	sh::Application* m_gameModule = nullptr;
};

#endif
