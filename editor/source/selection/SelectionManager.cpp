#include "SelectionManager.h"

SelectionManager::SelectionManager()
{
	m_defaultGizmo.reset(new Gizmo());
	m_moveGizmo.reset(new MoveGizmo());
	m_rotateGizmo.reset(new RotateGizmo());
	m_scaleGizmo.reset(new ScaleGizmo());

	m_gizmo = m_defaultGizmo;
}

bool SelectionManager::ProcessMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code)
{
	if (m_gizmo->IsEnabled())
	{
		m_gizmo->OnMouseEvent(x, y, type, code);
		return true;
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
				auto result = picker->TryToPick(x, y);
				m_gizmo->SetEntity(result);
				m_inspectorWidget->SetEntity(result);
				m_hierarchyWidget->SetSelectedEntity(result);
			}
		}
	}

	break;
	case sh::MouseEventType::Moved:
		break;
	default:
		break;
	}

	return false;
}

void SelectionManager::Render()
{
	if (m_gizmo)
		m_gizmo->Render();
}

void SelectionManager::SetSelectedEntity(sh::Entity* entity)
{
	m_selectedEntity = entity;
}

void SelectionManager::SetHierarchyWidget(const sh::SPtr<HierarchyWidget>& widget) 
{
	m_hierarchyWidget = widget; 
	m_hierarchyWidget->OnEntitySelected.Connect(std::bind(&SelectionManager::OnEntityFromListSelected, this, std::placeholders::_1));

	m_defaultGizmo->OnSelectedEntityChanged.Connect(std::bind(&HierarchyWidget::SetSelectedEntity, m_hierarchyWidget, std::placeholders::_1));
	m_moveGizmo->OnSelectedEntityChanged.Connect(std::bind(&HierarchyWidget::SetSelectedEntity, m_hierarchyWidget, std::placeholders::_1));
	m_scaleGizmo->OnSelectedEntityChanged.Connect(std::bind(&HierarchyWidget::SetSelectedEntity, m_hierarchyWidget, std::placeholders::_1));
	m_rotateGizmo->OnSelectedEntityChanged.Connect(std::bind(&HierarchyWidget::SetSelectedEntity, m_hierarchyWidget, std::placeholders::_1));
}

void SelectionManager::SetInspectorWidget(const sh::SPtr<InspectorWidget>& widget) 
{ 
	m_inspectorWidget = widget; 
	m_moveGizmo->SetTransformWidget(m_inspectorWidget->GetTransformWidget());
	m_rotateGizmo->SetTransformWidget(m_inspectorWidget->GetTransformWidget());
	m_scaleGizmo->SetTransformWidget(m_inspectorWidget->GetTransformWidget());

	m_defaultGizmo->OnSelectedEntityChanged.Connect(std::bind(&InspectorWidget::SetEntity, m_inspectorWidget, std::placeholders::_1));
	m_moveGizmo->OnSelectedEntityChanged.Connect(std::bind(&InspectorWidget::SetEntity, m_inspectorWidget, std::placeholders::_1));
	m_scaleGizmo->OnSelectedEntityChanged.Connect(std::bind(&InspectorWidget::SetEntity, m_inspectorWidget, std::placeholders::_1));
	m_rotateGizmo->OnSelectedEntityChanged.Connect(std::bind(&InspectorWidget::SetEntity, m_inspectorWidget, std::placeholders::_1));
}

void SelectionManager::SetMoveButton(const sh::gui::ButtonPtr& button) 
{ 
	m_moveGizmoButton = button; 
	m_moveGizmoButton->OnToggle.Connect(std::bind(&SelectionManager::OnMoveButtonToggled, this, std::placeholders::_1));
}

void SelectionManager::SetRotateButton(const sh::gui::ButtonPtr& button) 
{ 
	m_rotateGizmoButton = button; 
	m_rotateGizmoButton->OnToggle.Connect(std::bind(&SelectionManager::OnRotateButtonToggled, this, std::placeholders::_1));
}

void SelectionManager::SetScaleButton(const sh::gui::ButtonPtr& button) 
{ 
	m_scaleGizmoButton = button; 
	m_scaleGizmoButton->OnToggle.Connect(std::bind(&SelectionManager::OnScaleButtonToggled, this, std::placeholders::_1));
}

void SelectionManager::SetArrowButton(const sh::gui::ButtonPtr& button) 
{ 
	m_arrowButton = button; 
	m_arrowButton->OnToggle.Connect(std::bind(&SelectionManager::OnArrowButtonToggled, this, std::placeholders::_1));
}

void SelectionManager::OnMoveButtonToggled(bool toggled)
{
	if (toggled)
	{
		m_rotateGizmoButton->SetToggled(false);
		m_scaleGizmoButton->SetToggled(false);
		m_arrowButton->SetToggled(false);
		m_gizmo = m_moveGizmo;
	}
}

void SelectionManager::OnRotateButtonToggled(bool toggled)
{
	if (toggled)
	{
		m_moveGizmoButton->SetToggled(false);
		m_scaleGizmoButton->SetToggled(false);
		m_arrowButton->SetToggled(false);
		auto entity = m_gizmo->GetEntity();
		m_gizmo = m_rotateGizmo;
		m_gizmo->SetEntity(entity);
	}
}

void SelectionManager::OnScaleButtonToggled(bool toggled)
{
	if (toggled)
	{
		m_moveGizmoButton->SetToggled(false);
		m_rotateGizmoButton->SetToggled(false);
		m_arrowButton->SetToggled(false);
		auto entity = m_gizmo->GetEntity();
		m_gizmo = m_scaleGizmo;
		m_gizmo->SetEntity(entity);
	}
}

void SelectionManager::OnArrowButtonToggled(bool toggled)
{
	if (toggled)
	{
		m_moveGizmoButton->SetToggled(false);
		m_scaleGizmoButton->SetToggled(false);
		m_rotateGizmoButton->SetToggled(false);
		auto entity = m_gizmo->GetEntity();
		m_gizmo = m_defaultGizmo;
		m_gizmo->SetEntity(entity);
	}
}

void SelectionManager::OnEntityFromListSelected(sh::Entity* entity)
{
	m_gizmo->SetEntity(entity);
	m_inspectorWidget->SetEntity(entity);
}