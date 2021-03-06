#ifndef SHADOW_EDITOR_SELECTION_MANAGER_INCLUDE
#define SHADOW_EDITOR_SELECTION_MANAGER_INCLUDE

#include <Shadow.h>

#include "../gizmo/Gizmo.h"
#include "../gizmo/MoveGizmo.h"
#include "../gizmo/RotateGizmo.h"
#include "../gizmo/ScaleGizmo.h"

#include "../gui/hierarchywidget/HierarchyWidget.h"
#include "../gui/inspectorwidget/InspectorWidget.h"

class SelectionManager : public sh::Singleton<SelectionManager>
{
public:
	SelectionManager();

	bool ProcessMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	void Render();

	sh::Entity* GetSelectedEntity() { return m_selectedEntity; }
	void SetSelectedEntity(sh::Entity* entity);

	void SetHierarchyWidget(const sh::SPtr<HierarchyWidget>& widget);
	void SetInspectorWidget(const sh::SPtr<InspectorWidget>& widget);

	void SetMoveButton(const sh::gui::ButtonPtr& button);
	void SetRotateButton(const sh::gui::ButtonPtr& button);
	void SetScaleButton(const sh::gui::ButtonPtr& button);
	void SetArrowButton(const sh::gui::ButtonPtr& button);

	// Signal comes from one of gizmo button for switching between gizmo modes
	void OnGizmoButtonToggled(const sh::gui::ButtonPtr& sender);

	// Signal comes from hierarchy widget when users click on it's tree view
	void OnEntityFromListSelected(sh::Entity* entity);

	// Signals come from gizmos whe user changes one of the transform components using them
	void OnPositionChanged(const sh::math::Vector3&);
	void OnRotationChanged(const sh::math::Quaternion&);
	void OnScaleChanged(const sh::math::Vector3&);

private:
	sh::Entity* m_selectedEntity = nullptr;

	// Gizmos pack
	sh::SPtr<Gizmo> m_gizmo;

	sh::SPtr<Gizmo> m_defaultGizmo;
	sh::SPtr<MoveGizmo> m_moveGizmo;
	sh::SPtr<RotateGizmo> m_rotateGizmo;
	sh::SPtr<ScaleGizmo> m_scaleGizmo;

	// WIdgets
	sh::SPtr<HierarchyWidget> m_hierarchyWidget;
	sh::SPtr<InspectorWidget> m_inspectorWidget;

	sh::gui::ButtonPtr m_moveGizmoButton;
	sh::gui::ButtonPtr m_rotateGizmoButton;
	sh::gui::ButtonPtr m_scaleGizmoButton;
	sh::gui::ButtonPtr m_arrowButton;

	sh::gui::ButtonGroupPtr m_gizmoButtonGroup;

	sh::video::MaterialPtr m_aabbMaterial;
};

#endif
