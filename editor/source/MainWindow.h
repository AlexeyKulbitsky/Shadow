#ifndef SHADOW_EDITOR_MAIN_WINDOW
#define SHADOW_EDITOR_MAIN_WINDOW

#include "gizmo/Gizmo.h"
#include "gizmo/MoveGizmo.h"
#include "gizmo/RotateGizmo.h"
#include "gizmo/ScaleGizmo.h"
#include "gui/inspectorwidget/InspectorWidget.h"
#include "gui/hierarchywidget/HierarchyWidget.h"
#include "gui/assetswidget/AssetsWidget.h"


#include <Shadow.h>

class MainWindow : public sh::Application
{
public:
	MainWindow();

	void Update();

	void OpenScene();
	void SaveScene();
	void Close();

	void OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	void OnMouseWeelEvent(int x, int y, int delta);
	void OnKeyboardEvent(sh::KeyboardEventType type, sh::KeyCode code);
	void OnWindowResized(int width, int height);

	// Application interface
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(sh::u64 delta) override;

	void OnMoveButtonToggled(bool toggled);
	void OnRotateButtonToggled(bool toggled);
	void OnScaleButtonToggled(bool toggled);
	void OnArrowButtonToggled(bool toggled);

	void OnEntityFromListSelected(sh::Entity* entity);

private:
	sh::SPtr<InspectorWidget> m_inspectorWidget;
	sh::SPtr<HierarchyWidget> m_hierarchyWidget;
	sh::SPtr<AssetsWidget> m_assetsWidget;
	

	// Gizmos
	sh::SPtr<Gizmo> m_gizmo;

	sh::SPtr<Gizmo> m_defaultGizmo;
	sh::SPtr<MoveGizmo> m_moveGizmo;
	sh::SPtr<RotateGizmo> m_rotateGizmo;
	sh::SPtr<ScaleGizmo> m_scaleGizmo;

	sh::gui::ButtonPtr m_moveGizmoButton;
	sh::gui::ButtonPtr m_rotateGizmoButton;
	sh::gui::ButtonPtr m_scaleGizmoButton;
	sh::gui::ButtonPtr m_arrowButton;

	sh::Entity* m_cameraTargetEntity = nullptr;

	sh::gui::WidgetPtr m_mainWidget;
};

#endif
