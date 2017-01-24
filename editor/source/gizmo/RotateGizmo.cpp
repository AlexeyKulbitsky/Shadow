#include "RotateGizmo.h"

#include <scene\GeometryGenerator.h>
#include <scene\Mesh.h>
#include <scene\Model.h>
#include <scene\SceneManager.h>
#include <scene\Camera.h>

#include <video\Material.h>
#include <video\RenderCommand.h>
#include <video\RenderPipeline.h>
#include <video\Driver.h>
#include <video\RenderState.h>
#include <video\ShaderProgram.h>
#include <video\RenderTechnique.h>
#include <video\UniformBuffer.h>
#include <video\Uniform.h>
#include <Device.h>

#include <entity\Entity.h>
#include <entity\components\TransformComponent.h>

RotateGizmo::RotateGizmo()
{
	//m_axises[0].circleModel = sh::scene::GeometryGenerator::GetCircleModel(sh::math::Vector3f(0.0f), 5.0f, sh::math::Vector3f(0.0f, 0.0f, 1.0f), sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	m_axises[0].circleModel = sh::scene::GeometryGenerator::GetHalfTorusModel(
		sh::math::Vector3f(0.0f), 
		5.0f, 
		0.1f,
		8, 
		32, 
		sh::math::Vector3f(0.0f, 0.0f, 1.0f), 
		sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	sh::video::UniformBuffer* uniformBuffer = m_axises[0].circleModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[0].circleColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[0].circleColorUniform)
	{
		sh::math::Vector3f color(1.0f, 0.0f, 0.0f);
		m_axises[0].circleColorUniform->Set(color);
	}

	m_axises[0].localUp = sh::math::Vector3f(0.0f, 1.0f, 0.0f);

	////////////////////////////////////////////////

	//m_axises[1].circleModel = sh::scene::GeometryGenerator::GetCircleModel(sh::math::Vector3f(0.0f), 5.0f, sh::math::Vector3f(1.0f, 0.0f, 0.0f), sh::math::Vector3f(0.0f, 0.0f, 1.0f));
	m_axises[1].circleModel = sh::scene::GeometryGenerator::GetHalfTorusModel(
		sh::math::Vector3f(0.0f), 
		5.0f, 
		0.1f,
		8, 
		32, 
		sh::math::Vector3f(1.0f, 0.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));
	uniformBuffer = m_axises[1].circleModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[1].circleColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[1].circleColorUniform)
	{
		sh::math::Vector3f color(0.0f, 1.0f, 0.0f);
		m_axises[1].circleColorUniform->Set(color);
	}

	/////////////////////////////////////////////////

	//m_axises[2].circleModel = sh::scene::GeometryGenerator::GetCircleModel(sh::math::Vector3f(0.0f), 5.0f, sh::math::Vector3f(1.0f, 0.0f, 0.0f), sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	m_axises[2].circleModel = sh::scene::GeometryGenerator::GetHalfTorusModel(
		sh::math::Vector3f(0.0f), 
		5.0f, 
		0.1f,
		8, 
		32, 
		sh::math::Vector3f(1.0f, 0.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	uniformBuffer = m_axises[2].circleModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[2].circleColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[2].circleColorUniform)
	{
		sh::math::Vector3f color(0.0f, 0.0f, 1.0f);
		m_axises[2].circleColorUniform->Set(color);
	}

	////////////////////////////////////////////////////////////////////////

	m_axises[3].circleModel = sh::scene::GeometryGenerator::GetTorusModel(
		sh::math::Vector3f(0.0f), 
		5.0f, 
		0.05f,
		8, 
		32, 
		sh::math::Vector3f(1.0f, 0.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	uniformBuffer = m_axises[3].circleModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[3].circleColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[3].circleColorUniform)
	{
		sh::math::Vector3f color(0.5f, 0.5f, 0.5f);
		m_axises[3].circleColorUniform->Set(color);
	}
}

//////////////////////////////////////////////////////////////////////////

RotateGizmo::~RotateGizmo()
{
	for (size_t i = 0; i < 4; ++i)
	{
		delete m_axises[i].circleModel;
	}
}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::Render()
{
	if (!m_axises[0].circleModel || !m_axises[1].circleModel || !m_axises[2].circleModel ||
		!m_entity)
		return;

	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
	sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
	if (m_entity)
	{
		sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>(m_entity->GetComponent(sh::Component::Type::TRANSFORM));
		if (transformComponent)
		{
			//sh::math::Matrix4f matrix = transformComponent->GetWorldMatrix();
			sh::math::Matrix4f matrix;
			matrix.SetIdentity();

			sh::math::Vector3f position = transformComponent->GetPosition();
			sh::math::Quaternionf rotation = transformComponent->GetRotation();
			sh::f32 scaleFactor = (camera->GetPosition() - position).GetLength() / 35.0f;
			sh::math::Vector3f scale(scaleFactor);

			matrix.SetScale(scale);
			matrix.SetTranslation(position);

			matrix = matrix * rotation.GetAsMatrix4();

			for (size_t i = 0; i < 3; ++i)
			{
				m_axises[i].circleModel->SetWorldMatrix(matrix);
				m_axises[i].circleModel->UpdateTransformationUniforms();
			}

			matrix.SetIdentity();
			matrix.SetScale(scale);
			matrix.SetTranslation(position);
			matrix = matrix * camera->GetRotation().GetAsMatrix4();
			m_axises[3].circleModel->SetWorldMatrix(matrix);
			m_axises[3].circleModel->UpdateTransformationUniforms();
		}
	}

	driver->Render(m_axises[3].circleModel);

	for (size_t i = 0; i < 3; ++i)
	{
		driver->Render(m_axises[i].circleModel);
	}
}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::Process()
{

}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::OnMousePressed(sh::u32 x, sh::u32 y)
{

}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::OnMouseReleased(sh::u32 x, sh::u32 y)
{

}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::OnMouseMoved(sh::u32 x, sh::u32 y)
{
	if (m_axises[0].active)
	{
		Rotate(Axis::Type::X_AXIS);
	}
	else if (m_axises[1].active)
	{
		Rotate(Axis::Type::Y_AXIS);
	}
	else if (m_axises[2].active)
	{
		Rotate(Axis::Type::Z_AXIS);
	}
}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
{
	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

	unsigned char data[4];
	driver->GetPixelData(x, y, width, height, data);
	sh::math::Vector3f color(1.0f, 1.0f, 1.0f);
	if (data[0] == 255 && data[1] == 0 && data[2] == 0)
	{
		m_axises[Axis::Type::X_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = true;

		color = sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		m_axises[Axis::Type::Y_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 0.0f, 1.0f);
		m_axises[Axis::Type::Z_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
	else if (data[0] == 0 && data[1] == 255 && data[2] == 0)
	{
		m_axises[Axis::Type::Y_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = true;

		color = sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		m_axises[Axis::Type::X_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 0.0f, 1.0f);
		m_axises[Axis::Type::Z_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
	else if (data[0] == 0 && data[1] == 0 && data[2] == 255)
	{
		m_axises[Axis::Type::Z_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = true;

		color = sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		m_axises[Axis::Type::X_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		m_axises[Axis::Type::Y_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = false;
	}
	else if (data[0] == 255 && data[1] == 255 && data[2] == 255)
	{
		return;
	}
	else
	{
		color = sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		m_axises[Axis::Type::X_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		m_axises[Axis::Type::Y_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 0.0f, 1.0f);
		m_axises[Axis::Type::Z_AXIS].circleColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
}

//////////////////////////////////////////////////////////////////////////

bool RotateGizmo::IsActive() const
{
	if (m_axises[0].active || m_axises[1].active || m_axises[2].active)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void RotateGizmo::Rotate(Axis::Type axis)
{
	sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
	sh::InputManager* inputManager = sh::Device::GetInstance()->GetInputManager();
	sh::math::Vector2i old = inputManager->GetMousePositionOld();
	sh::math::Vector2i current = inputManager->GetMousePositionCurrent();
	sh::math::Vector3f rayOrigin, rayDirOld, rayDirCurrent;
	camera->BuildRay(old.x, old.y, rayOrigin, rayDirOld);
	camera->BuildRay(current.x, current.y, rayOrigin, rayDirCurrent);

	sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>(m_entity->GetComponent(sh::Component::Type::TRANSFORM));
	sh::math::Vector3f pos = transformComponent->GetPosition();
	sh::math::Matrix3f rotation = transformComponent->GetRotation().GetAsMatrix3();
	sh::math::Vector3f axisRotations(0.0f);

	sh::math::Vector3f axisDir(0.0f);
	sh::math::Vector3f normalDir(0.0f);

	switch (axis)
	{
	case Axis::Type::X_AXIS:
		normalDir = sh::scene::SceneManager::GetRightVector();		
		break;
	case Axis::Type::Y_AXIS:
		normalDir = sh::scene::SceneManager::GetUpVector();
		break;
	case Axis::Type::Z_AXIS:
		normalDir = sh::scene::SceneManager::GetFrontVector();
		break;
	}

	sh::math::Vector3f normal = rotation * normalDir;
	sh::math::Planef plane(pos, normal);

	sh::math::Vector3f intersectionOld(0.0f), intersectionCurrent(0.0f);
	plane.GetIntersectionWithLine(rayOrigin, rayDirOld, intersectionOld);
	plane.GetIntersectionWithLine(rayOrigin, rayDirCurrent, intersectionCurrent);
	
	sh::math::Vector3f vec1 = (intersectionOld - pos).Normalize();
	sh::math::Vector3f vec2 = (intersectionCurrent - pos).Normalize();
	sh::f32 angle = sh::math::Acos(vec1.Dot(vec2));

	sh::math::Quaternionf rot;
	rot.RotationFromTo(vec1, vec2);

	sh::math::Quaternionf finalRotation =  transformComponent->GetRotation() * rot;

	transformComponent->SetRotation(finalRotation);

	inputManager->SetMousePositionOld(current);
}

//////////////////////////////////////////////////////////////////////////