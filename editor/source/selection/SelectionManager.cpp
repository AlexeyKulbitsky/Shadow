#include "SelectionManager.h"

SelectionManager::SelectionManager()
{
	m_defaultGizmo.reset(new Gizmo());
	m_moveGizmo.reset(new MoveGizmo());
	m_rotateGizmo.reset(new RotateGizmo());
	m_scaleGizmo.reset(new ScaleGizmo());

	m_gizmo = m_defaultGizmo;

	m_gizmoButtonGroup.reset(new sh::gui::ButtonGroup());
	m_gizmoButtonGroup->buttonToggled.Connect(std::bind(&SelectionManager::OnGizmoButtonToggled, this, std::placeholders::_1));
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
	m_gizmoButtonGroup->AddButton(m_moveGizmoButton);
}

void SelectionManager::SetRotateButton(const sh::gui::ButtonPtr& button) 
{ 
	m_rotateGizmoButton = button; 
	m_gizmoButtonGroup->AddButton(m_rotateGizmoButton);
}

void SelectionManager::SetScaleButton(const sh::gui::ButtonPtr& button) 
{ 
	m_scaleGizmoButton = button; 
	m_gizmoButtonGroup->AddButton(m_scaleGizmoButton);
}

void SelectionManager::SetArrowButton(const sh::gui::ButtonPtr& button) 
{ 
	m_arrowButton = button; 
	m_gizmoButtonGroup->AddButton(m_arrowButton);
}

void SelectionManager::OnGizmoButtonToggled(const sh::gui::ButtonPtr& sender)
{
	sh::Entity* entity = nullptr;
	if (m_gizmo)
	{
		entity = m_gizmo->GetEntity();
		m_gizmo->SetEntity(nullptr);
	}

	if (sender == m_moveGizmoButton)
		m_gizmo = m_moveGizmo;
	else if (sender == m_scaleGizmoButton)
		m_gizmo = m_scaleGizmo;
	else if (sender == m_rotateGizmoButton)
		m_gizmo = m_rotateGizmo;
	else if (sender == m_arrowButton)
		m_gizmo = m_defaultGizmo;

	if (m_gizmo)
	{
		m_gizmo->SetEntity(entity);
	}
}

void SelectionManager::OnEntityFromListSelected(sh::Entity* entity)
{
	m_gizmo->SetEntity(entity);
	m_inspectorWidget->SetEntity(entity);
}