#include "SelectionManager.h"
#include "../gui/propertyeditors/Vector3PropertyEditor.h"
#include "../gui/propertyeditors/QuaternionPropertyEditor.h"

SelectionManager::SelectionManager()
{
	m_aabbMaterial.reset(new sh::video::Material());
	m_aabbMaterial->SetRenderTechnique("VertexColor3D.rt");

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
		// If it has render component then render the Bounding box of the model
		auto renderComponent = m_selectedEntity->GetComponent<sh::RenderComponent>();
		if (renderComponent)
		{
            if (const auto& model = renderComponent->GetModel())
            {
                sh::Device::GetInstance()->GetDriver()->GetPainter()->SetMaterial(m_aabbMaterial);
                auto driver = sh::Device::GetInstance()->GetDriver();
                driver->GetPainter()->DrawBox(model->GetBoundingBox());
                driver->GetPainter()->Flush();
            }
			return;
		}
		// If it has terrain component then render the Bounding box of the terrain model
		auto terrainComponent = m_selectedEntity->GetComponent<sh::TerrainComponent>();
		if (terrainComponent)
		{
			sh::Device::GetInstance()->GetDriver()->GetPainter()->SetMaterial(m_aabbMaterial);
			auto driver = sh::Device::GetInstance()->GetDriver();
			driver->GetPainter()->DrawBox(terrainComponent->GetModel()->GetBoundingBox());
			driver->GetPainter()->Flush();
			return;
		}
		// If it has light component then render light bounds according to its type
		auto lightComponent = m_selectedEntity->GetComponent<sh::LightComponent>();
		if (lightComponent)
		{
			sh::Device::GetInstance()->GetDriver()->GetPainter()->SetMaterial(m_aabbMaterial);
			auto driver = sh::Device::GetInstance()->GetDriver();

			switch (lightComponent->GetType())
			{
			case sh::scene::Light::Type::DIRECTIONAL:
			{
				auto transformConponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
				auto delta = lightComponent->GetDirection() * 3.0f;
				auto position = lightComponent->GetPosition();
				auto xOffset = transformConponent->GetRotation() * sh::scene::SceneManager::GetRightVector();
				auto zOffset = transformConponent->GetRotation() * sh::scene::SceneManager::GetFrontVector();
				const float scaleFactor = 0.5f;
				driver->GetPainter()->DrawLine(position, position + delta);

				driver->GetPainter()->DrawLine(position + xOffset * scaleFactor, position + xOffset * scaleFactor + delta);
				driver->GetPainter()->DrawLine(position - xOffset * scaleFactor, position - xOffset * scaleFactor + delta);
				driver->GetPainter()->DrawLine(position + zOffset * scaleFactor, position + zOffset * scaleFactor + delta);
				driver->GetPainter()->DrawLine(position - zOffset * scaleFactor, position - zOffset * scaleFactor + delta);

				driver->GetPainter()->Flush();
			}
				break;
			case sh::scene::Light::Type::POINT:
				break;
			case sh::scene::Light::Type::SPOT:
				break;
			default:
				break;
			}
			
			return;
		}
	}
}

void SelectionManager::SetSelectedEntity(sh::Entity* entity)
{
	m_selectedEntity = entity;
	
	m_inspectorWidget->SetEntity(m_selectedEntity);
	m_hierarchyWidget->SetSelectedEntity(m_selectedEntity);
	if (m_selectedEntity)
	{
		m_gizmo->SetEnabled(true);
	}
	else
	{
		m_gizmo->SetEnabled(false);
	}
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
			return;
		}

		m_gizmo->SetEnabled(false);
	}
	// Otherwise clear anything connected with entity selection
	else
	{
		m_gizmo->SetEnabled(false);
	}
}

void SelectionManager::OnPositionChanged(const sh::math::Vector3& position)
{
	auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
	if (transformComponent)
	{
        auto transformWidget = m_inspectorWidget->FindChild(transformComponent->GetTypeName());
        if (!transformWidget)
            return;
        auto positionPropertyWidget = transformWidget->FindChild("Position");
        if (!positionPropertyWidget)
            return;
        static_cast<Vector3PropertyEditor*>(positionPropertyWidget.get())->SetValue(position);
	}
}

void SelectionManager::OnRotationChanged(const sh::math::Quaternion& rotation)
{
	auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
	if (transformComponent)
	{
        auto transformWidget = m_inspectorWidget->FindChild(transformComponent->GetTypeName());
        if (!transformWidget)
            return;
        auto rotationePropertyWidget = transformWidget->FindChild("Rotation");
        if (!rotationePropertyWidget)
            return;
        static_cast<QuaternionPropertyEditor*>(rotationePropertyWidget.get())->SetValue(rotation);
	}
}

void SelectionManager::OnScaleChanged(const sh::math::Vector3& scale)
{
	auto transformComponent = m_selectedEntity->GetComponent<sh::TransformComponent>();
	if (transformComponent)
	{
        auto transformWidget = m_inspectorWidget->FindChild(transformComponent->GetTypeName());
        if (!transformWidget)
            return;
        auto scalePropertyWidget = transformWidget->FindChild("Scale");
        if (!scalePropertyWidget)
            return;
        static_cast<Vector3PropertyEditor*>(scalePropertyWidget.get())->SetValue(scale);
	}
}
