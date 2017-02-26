#include "GraphicsWidget.h"
#include "decorators\TransformComponent\TransformComponentDecorator.h"
#include <QResizeEvent>
#include <QAction>



GraphicsWidget::GraphicsWidget(QWidget* parent) : QWidget(parent)
{
	setMouseTracking(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////

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

void GraphicsWidget::mouseMoveEvent(QMouseEvent * ev)
{	
	sh::Event e;
	e.type = sh::EventType::MOUSE_INPUT_EVENT;
	e.mouseEvent.type = sh::MouseEventType::MOVED;	
	e.mouseEvent.x = ev->x();
	e.mouseEvent.y = ev->y();
	sh::Device::GetInstance()->OnEvent(e);

	if (m_gizmo && m_leftButtonPressed)
	{
		m_gizmo->OnMouseMoved(ev->x(), ev->y());
		return;
	}

	if (m_gizmo && m_gizmo->IsEnabled())
	{
		m_gizmo->TryToSelect(ev->x(), ev->y(), width(), height());
	}
	else
	{
		
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::mousePressEvent(QMouseEvent * ev)
{
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
		
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::mouseReleaseEvent(QMouseEvent * ev)
{
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
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::wheelEvent(QWheelEvent * e)
{
	float delta = (float)e->delta() * 0.1f;
	sh::scene::Camera* camera = m_sceneManager->GetCamera();
	camera->SetPosition(camera->GetPosition() + camera->GetFrontVector() * delta);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::keyPressEvent(QKeyEvent * ev)
{
	sh::Event e;
	e.type = sh::EventType::KEYBOARD_INPUT_EVENT;
	e.keyboardEvent.keyCode = (sh::KeyCode)ev->key(); 
	e.keyboardEvent.type = sh::KeyboardEventType::KEY_PRESEED;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::keyReleaseEvent(QKeyEvent * ev)
{
	sh::Event e;
	e.type = sh::EventType::KEYBOARD_INPUT_EVENT;
	e.keyboardEvent.keyCode = (sh::KeyCode)ev->key();
	e.keyboardEvent.type = sh::KeyboardEventType::KEY_RELEASED;
}

/////////////////////////////////////////////////////////////////////////////////////////////

GraphicsWidget::~GraphicsWidget()
{
	//delete m_gizmo;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Init()
{
	m_driver = sh::Device::GetInstance()->GetDriver();
	m_sceneManager = sh::Device::GetInstance()->GetSceneManager();

	// Create all gizmos
	m_defaultGizmo = new Gizmo();
	m_moveGizmo = new MoveGizmo();
	m_rotateGizmo = new RotateGizmo();
	//m_scaleGizmo = new ScaleGizmo();

	// Set default gizmo as current
	m_gizmo = m_defaultGizmo;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Update()
{
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
			float xAngle = (float)delta.x * 0.01f;
			float yAngle = -(float)delta.y * 0.01f;

			sh::math::Quaternionf xRot;
			xRot.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), xAngle);
			sh::math::Quaternionf yRot;
			yRot.SetFromAxisAngle(camera->GetRightVector(), yAngle);
			sh::math::Quaternionf deltaRot = xRot * yRot;
			sh::math::Vector3f targetVec = camera->GetPosition() - targetPos;
			targetVec = deltaRot * targetVec;
			//camera->SetRotation(deltaRot * camera->GetRotation());
			camera->SetPosition(targetPos + targetVec);
			sh::math::Vector3f dir = targetPos - camera->GetPosition();
			dir.Normalize();
			sh::math::Quaternionf finalCamRot;
			finalCamRot.RotationFromTo(sh::scene::SceneManager::GetFrontVector(), dir);
			camera->SetRotation(finalCamRot);
		}
		else
		{
			sh::math::Vector2i delta = current - old;
			sh::math::Vector3f cameraUpMove = camera->GetUpVector() * delta.y * 0.1f;
			sh::math::Vector3f cameraRightMove = camera->GetRightVector() * (-delta.x) * 0.1f;
			sh::math::Vector3f cameraDeltaMove = cameraUpMove + cameraRightMove;
			camera->SetPosition(camera->GetPosition() + cameraDeltaMove);
		}		
	}
	inputManager->SetMousePositionOld(current);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Render()
{
	m_driver->BeginRendering();
	m_sceneManager->Update();

	if (m_gizmo)
	{
		m_gizmo->Render();
	}
	
	m_driver->EndRendering();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::TransformActionChanged(QAction* action)
{
	QString text = action->text();
	if (text == "Select")
	{
		m_defaultGizmo->SetEntity(m_gizmo->GetEntity());
		m_defaultGizmo->SetEnabled(m_gizmo->IsEnabled());
		m_gizmo->SetEntity(nullptr);
		m_gizmo->SetEnabled(false);
		m_gizmo = m_defaultGizmo;
	}
	else if (text == "Move")
	{
		m_moveGizmo->SetEntity(m_gizmo->GetEntity());
		m_moveGizmo->SetEnabled(m_gizmo->IsEnabled());
		m_gizmo->SetEntity(nullptr);
		m_gizmo->SetEnabled(false);
		m_gizmo = m_moveGizmo;
	}
	else if (text == "Rotate")
	{
		m_rotateGizmo->SetEntity(m_gizmo->GetEntity());
		m_rotateGizmo->SetEnabled(m_gizmo->IsEnabled());
		m_gizmo->SetEntity(nullptr);
		m_gizmo->SetEnabled(false);
		m_gizmo = m_rotateGizmo;
	}
	else if (text == "Scale")
	{
		printf("Scale\n");
		m_gizmo = nullptr;
	}
	else
	{
		m_gizmo = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

