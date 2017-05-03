#include "GraphicsWidget.h"

#if 0

void GraphicsWidget::resizeEvent(QResizeEvent *e)
{
	QSize newSize = e->size();
	sh::u32 width = newSize.width();
	sh::u32 height = newSize.height();

	sh::scene::Camera* camera = m_sceneManager->GetCamera();
	camera->SetProjection(camera->GetFov(), (sh::f32)width / (sh::f32)height, camera->GetNearPlane(), camera->GetFarPlane());
	m_driver->SetViewport(0, 0, width, height);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::mousePressEvent(QMouseEvent * ev)
{
	/*
	sh::Event e;
	e.type = sh::EventType::MOUSE_INPUT_EVENT;
	e.mouseEvent.type = sh::MouseEventType::BUTTON_PRESSED;
	
	switch (ev->button())
	{
		case Qt::RightButton:
			e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_RIGHT;
			break;
		case Qt::LeftButton:
			e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_LEFT;
			m_leftButtonPressed = true;
			break;
		case Qt::MiddleButton:
			e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_WHEEL;
		default:
			break;
	}

	e.mouseEvent.x = ev->x();
	e.mouseEvent.y = ev->y();	
	sh::Device::GetInstance()->OnEvent(e);	
	

	sh::gui::GuiManager::GetInstance()->OnMouseEvent(ev->x(), ev->y());
	return;
	if (m_gizmo)
	{
		m_gizmo->OnMousePressed(ev->x(), ev->y());
		if (m_gizmo->IsActive())
		{
			return;
		}
	}
	
	const sh::scene::PickerPtr& picker = sh::Device::GetInstance()->GetSceneManager()->GetPicker();
	if (ev->button() == Qt::LeftButton)
	{
		sh::Entity* entity = picker->TryToPick(ev->x(), ev->y(), width(), height());	
		
		if (!entity || entity != m_gizmo->GetEntity())
		{
			emit EntitySelected(nullptr);
			emit EntitySelected(entity);
		}
		
		m_gizmo->SetEntity(entity);
		
		if (entity)
		{
			m_gizmo->SetEnabled(true);
		}
		else
		{
			m_gizmo->SetEnabled(false);
		}
		
	}
	else if (ev->button() == Qt::MiddleButton)
	{
		m_cameraTargetEntity = picker->TryToPick(ev->x(), ev->y(), width(), height());	
	}
		*/
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::mouseReleaseEvent(QMouseEvent * ev)
{
	/*
	sh::Event e;
	e.type = sh::EventType::MOUSE_INPUT_EVENT;
	e.mouseEvent.type = sh::MouseEventType::BUTTON_RELEASED;

	switch (ev->button())
	{
	case Qt::RightButton:
		e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_RIGHT;
		break;
	case Qt::LeftButton:
		e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_LEFT;
		m_leftButtonPressed = false;
		break;
	case Qt::MiddleButton:
		e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_WHEEL;
	default:
		break;
	}

	e.mouseEvent.x = ev->x();
	e.mouseEvent.y = ev->y();
	sh::Device::GetInstance()->OnEvent(e);

	if (m_gizmo->IsEnabled())
		m_gizmo->OnMouseReleased(ev->x(), ev->y());
		*/
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Update()
{
	/*
	sh::Device* device = sh::Device::GetInstance();
	sh::InputManager* inputManager = device->GetInputManager();
	sh::math::Vector2i old = inputManager->GetMousePositionOld();
	sh::math::Vector2i current = inputManager->GetMousePositionCurrent();
	sh::scene::Camera* camera = device->GetSceneManager()->GetCamera();

	if (inputManager->IsMouseButtonPressed(sh::MouseCode::BUTTON_WHEEL))
	{
		if (inputManager->IsKeyPressed(sh::KeyCode::KEY_MENU))
		{
			const sh::math::Vector4u& viewport = device->GetDriver()->GetViewPort();
			sh::math::Vector3f targetPos(0.0f);
			if (m_cameraTargetEntity)
			{
				sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>(m_cameraTargetEntity->GetComponent(sh::Component::Type::TRANSFORM));
				targetPos = transformComponent->GetPosition();
			}
			
			sh::math::Vector2i delta = current - old;
			float xAngle = -(float)delta.x * 0.01f;
			float yAngle = (float)delta.y * 0.01f;

			sh::math::Quaternionf xRot;
			xRot.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), -xAngle);
			sh::math::Quaternionf yRot;
			yRot.SetFromAxisAngle(camera->GetRightVector(), yAngle);
			sh::math::Quaternionf deltaRot = xRot * yRot;
			sh::math::Vector3f baseVec = camera->GetPosition() - targetPos;
			//float length = baseVec.GetLength();
			//baseVec.Normalize();
			sh::math::Vector3f targetVec = deltaRot * baseVec;
			//camera->SetPosition(targetPos + targetVec);
			
			sh::math::Quaternionf finalCamRot = camera->GetRotation() * deltaRot;
			camera->SetRotation(finalCamRot);
		}
		else
		{
			
		}		
	}
	inputManager->SetMousePositionOld(current);
	*/
}

#endif