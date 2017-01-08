#include "GraphicsWidget.h"

#include <QResizeEvent>

#include <Shadow.h>
#include <scene\SceneManager.h>
#include <scene\Camera.h>
#include <scene\Model.h>
#include <entity\Entity.h>


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
	if (m_leftButtonPressed)
	{
		if (m_gizmo->m_axises[0].active)
		{
			m_gizmo->Move(Gizmo::Axis::Type::X_AXIS);
		}
		else if (m_gizmo->m_axises[1].active)
		{
			m_gizmo->Move(Gizmo::Axis::Type::Y_AXIS);
		}
		else if (m_gizmo->m_axises[2].active)
		{
			m_gizmo->Move(Gizmo::Axis::Type::Z_AXIS);
		}
	}

	sh::Event e;
	e.type = sh::EventType::MOUSE_INPUT_EVENT;
	e.mouseEvent.type = sh::MouseEventType::MOVED;
	
	e.mouseEvent.x = ev->x();
	e.mouseEvent.y = ev->y();

	sh::Device::GetInstance()->OnEvent(e);

	if (m_gizmo->IsEnabled())
	{
		m_picker->TryToPick(m_gizmo, ev->x(), ev->y(), width(), height());
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


	if (m_gizmo->IsEnabled())
	{
		if (m_gizmo->m_axises[0].active ||
			m_gizmo->m_axises[1].active ||
			m_gizmo->m_axises[2].active)
		{
			return;
		}
	}





	sh::math::Matrix4f inverseProjMatrix = m_sceneManager->GetCamera()->GetProjectionMatrix().GetInversed();
	sh::math::Matrix4f inverseViewMatrix = m_sceneManager->GetCamera()->GetViewMatrix().GetInversed();
	sh::math::Vector3f cameraPos = m_sceneManager->GetCamera()->GetPosition();
	
	// 3d Normalised Device Coordinates	
	float x = (2.0f * ev->x()) / width() - 1.0f;
	float y = 1.0f - (2.0f * ev->y()) / height();
	float z = 1.0f;
	sh::math::Vector3f rayNds(x, y, z);

	// 4d Homogeneous Clip Coordinates
	sh::math::Vector4f rayClip(rayNds.x, rayNds.y, -1.0f, 1.0f);

	// 4d Eye (Camera) Coordinates
	sh::math::Vector4f rayEye = inverseProjMatrix * rayClip;
	rayEye = sh::math::Vector4f(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// 4d World Coordinates
	sh::math::Vector4f temp = inverseViewMatrix * rayEye;
	sh::math::Vector3f rayWorld(temp.x, temp.y, temp.z);
	// don't forget to normalise the vector at some point
	rayWorld.Normalize();
	//printf("x:%f y:%f z:%f\n", rayWorld.x, rayWorld.y, rayWorld.z);

	sh::Entity* entity = m_picker->TryToPick(ev->x(), ev->y(), width(), height());
	m_gizmo->SetEntity(entity);
	emit EntitySelected(entity);
	if (entity)
	{
		m_gizmo->SetEnabled(true);
	}
	else
	{
		m_gizmo->SetEnabled(false);
	}
		
	sh::Device::GetInstance()->GetDriver()->PrintPixelInfo(ev->x(), ev->y(), width(), height());	
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
}

/////////////////////////////////////////////////////////////////////////////////////////////

GraphicsWidget::~GraphicsWidget()
{
	delete m_gizmo;
	delete m_picker;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void GraphicsWidget::Init()
{
	m_driver = sh::Device::GetInstance()->GetDriver();
	m_sceneManager = sh::Device::GetInstance()->GetSceneManager();

	m_gizmo = new Gizmo();
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

	m_gizmo->Render();

	m_driver->EndRendering();
}

/////////////////////////////////////////////////////////////////////////////////////////////