#include "SelectionManager.h"

SelectionManager::SelectionManager()
{
	m_aabbMaterial.reset(new sh::video::Material());
	m_aabbMaterial->SetRenderTechnique("vertex_color.rt");

	m_defaultGizmo.reset(new Gizmo());
	m_moveGizmo.reset(new MoveGizmo());
	m_moveGizmo->positionChanged.Connect(std::bind(&SelectionManager::OnPositionChanged, this, std::placeholders::_1));
	m_rotateGizmo.reset(new RotateGizmo());
	m_rotateGizmo->rotationChanged.Connect(std::bind(&SelectionManager::OnRotationChanged, this, std::placeholders::_1));
	m_scaleGizmo.reset(new ScaleGizmo());
	m_scaleGizmo->scaleChanged.Connect(std::bind(&SelectionManager::OnScaleChanged, this, std::placeholders::_1));

	m_gizmo = m_defaultGizmo;

	m_gizmoButtonGroup.reset(new sh::gui::ButtonGroup());
	m_gizmoButtonGroup->buttonToggled.Connect(std::bind(&SelectionManager::OnGizmoButtonToggled, this, std::placeholders::_1));
}

bool SelectionManager::ProcessMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code)
{
	if (m_gizmo->IsEnabled())
	{
		if (m_gizmo->OnMouseEvent(x, y, type, code))
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
				m_selectedEntity = result;
				m_inspectorWidget->SetEntity(m_selectedEntity);
				m_hierarchyWidget->SetSelectedEntity(m_selectedEntity);
				if (!m_selectedEntity)
					return false;

				auto transformComponent = result->GetComponent<sh::TransformComponent>();
				if (transformComponent)
				{
					m_gizmo->SetPosition(transformComponent->GetPosition());
					m_gizmo->SetRotation(transformComponent->GetRotation());
					m_gizmo->SetScale(transformComponent->GetScale());
					m_gizmo->SetEnabled(true);
				}

				
			}
			else
			{
				if (!m_gizmo->OnMouseEvent(x, y, type, code))
				{
					m_gizmo->SetEnabled(false);
					m_inspectorWidget->SetEntity(nullptr);
					m_hierarchyWidget->SetSelectedEntity(nullptr);
					m_selectedEntity = nullptr;
					return true;
				}
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
	{
		m_gizmo->Render();
	}

	// Render selection for entity if it is possible
	if (m_selectedEntity)
	{
		auto renderComponent = m_selectedEntity->GetComponent<sh::RenderComponent>();
		if (renderComponent)
		{
			sh::Device::GetInstance()->GetDriver()->GetPainter()->SetMaterial(m_aabbMaterial);
			auto driver = sh::Device::GetInstance()->GetDriver();
			driver->GetPainter()->DrawBox(renderComponent->GetModel()->GetBoundingBox());
			driver->GetPainter()->Flush();
		}
		else 
		{
			auto terrainComponent = m_selectedEntity->GetComponent<sh::TerrainComponent>();
			if (terrainComponent)
			{
				sh::Device::GetInstance()->GetDriver()->GetPainter()->SetMaterial(m_aabbMaterial);
				auto driver = sh::Device::GetInstance()->GetDriver();
				driver->GetPainter()->DrawBox(terrainComponent->GetModel()->GetBoundingBox());
				driver->GetPainter()->Flush();
			}
		}
	}
}

void SelectionManager::SetSelectedEntity(sh::Entity* entity)
{
	m_selectedEntity = entity;
}

void SelectionManager::SetHierarchyWidget(const sh::SPtr<HierarchyWidget>& widget) 
{
	m_hierarchyWidget = widget; 
	m_hierarchyWidget->OnEntitySelected.Connect(std::bind(&SelectionManager::OnEntityFromListSelected, this, std::placeholders::_1));
}

void SelectionManager::SetInspectorWidget(const sh::SPtr<InspectorWidget>& widget) 
{ 
	m_inspectorWidget = widget; 
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
	bool enabled = m_gizmo->IsEnabled();
	if (sender == m_moveGizmoButton)
		m_gizmo = m_moveGizmo;
	else if (sender == m_scaleGizmoButton)
		m_gizmo = m_scaleGizmo;
	else if (sender == m_rotateGizmoButton)
		m_gizmo = m_rotateGizmo;
	else if (sender == m_arrowButton)
		m_gizmo = m_defaultGizmo;
	m_gizmo->SetEnabled(enabled);
}

void SelectionManager::OnEntityFromListSelected(sh::Entity* entity)
{
	m_selectedEntity = entity;
	m_inspectorWidget->SetEntity(m_selectedEntity);
	
	// If entity is selected then reset gizmo
	if (m_selectedEntity)
	{
		auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
		if (transformComponent)
		{
			m_gizmo->SetPosition(transformComponent->GetPosition());
			m_gizmo->SetRotation(transformComponent->GetRotation());
			m_gizmo->SetScale(transformComponent->GetScale());
			m_gizmo->SetEnabled(true);
		}
	}
	// Otherwise clear anythig connected with entity selection
	else
	{
		m_gizmo->SetEnabled(false);
	}
}

void SelectionManager::OnPositionChanged(const sh::math::Vector3f& position)
{
	auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
	transformComponent->SetPosition(position);
	m_inspectorWidget->GetTransformWidget()->Update();
}

void SelectionManager::OnRotationChanged(const sh::math::Quaternionf& rotation)
{
	auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
	transformComponent->SetRotation(rotation);
	m_inspectorWidget->GetTransformWidget()->Update();
}

void SelectionManager::OnScaleChanged(const sh::math::Vector3f& scale)
{
	auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
	transformComponent->SetScale(scale);
	m_inspectorWidget->GetTransformWidget()->Update();
}