#include "GraphicsWidget.h"

#include <QResizeEvent>

#include <Shadow.h>
#include <scene\SceneManager.h>
#include <scene\Camera.h>
#include <scene\Model.h>


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

	size_t modelsCount = m_sceneManager->GetModelsCount();
	for (size_t i = 0; i < modelsCount; ++i)
	{
		sh::math::Vector3f pos = m_sceneManager->GetModel(i)->GetPosition();
		float radius = 1.0f;
		float t1 = 0.0f, t2 = 0.0f;
		int result = sh::math::RayIntersectSphere(cameraPos, rayWorld, pos, radius, t1, t2);
		if (result != 0)
		{
			printf("Intersection with %s\n", m_sceneManager->GetModel(i)->GetName().c_str());
		}
		else
		{
			printf("No intersection with %s\n", m_sceneManager->GetModel(i)->GetName().c_str());
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