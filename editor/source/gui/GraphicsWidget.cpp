#include "GraphicsWidget.h"

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
	
	
	if (ev->button() == Qt::LeftButton)
	{
		sh::Entity* entity = m_picker->TryToPick(ev->x(), ev->y(), width(), height());	
		
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

GraphicsWidget::~GraphicsWidget()
{
	//delete m_gizmo;
	delete m_picker;
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
	m_picker = new Picker();

	size_t entitiesCount = m_sceneManager->GetEntitiesCount();
	for (size_t i = 0; i < entitiesCount; ++i)
	{
		m_picker->RegisterEntity(m_sceneManager->GetEntity(i));
	}
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

