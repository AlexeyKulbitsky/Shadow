#include "GraphicsWidget.h"

#include <QResizeEvent>

#include <Shadow.h>
#include <scene\SceneManager.h>
#include <scene\Camera.h>


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
			break;
		case Qt::MiddleButton:
			e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_WHEEL;
		default:
			break;
	}
		
	e.mouseEvent.x = ev->x();
	e.mouseEvent.y = ev->y();
	
	sh::Device::GetInstance()->OnEvent(e);
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
		break;
	case Qt::MiddleButton:
		e.mouseEvent.mouseCode = sh::MouseCode::BUTTON_WHEEL;
	default:
		break;
	}

	e.mouseEvent.x = ev->x();
	e.mouseEvent.y = ev->y();

	sh::Device::GetInstance()->OnEvent(e);
}

/////////////////////////////////////////////////////////////////////////////////////////////

GraphicsWidget::~GraphicsWidget()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Init()
{
	m_driver = sh::Device::GetInstance()->GetDriver();
	m_sceneManager = sh::Device::GetInstance()->GetSceneManager();
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Render()
{
	m_driver->BeginRendering();
	m_sceneManager->Update();
	m_sceneManager->Render();

	m_driver->EndRendering();
}

/////////////////////////////////////////////////////////////////////////////////////////////