#include "MoveGizmo.h"


MoveGizmo::MoveGizmo()
{
	//m_axises[0].lineModel = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(1.0f, 0.0f, 0.0f), sh::math::Vector3f(5.0f, 0.0f, 0.0f));
	m_axises[0].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		0.1f,
		sh::math::Vector3f(4.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 1.0f, 0.0f),
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));
	sh::video::UniformBuffer* uniformBuffer = m_axises[0].lineModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[0].lineColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[0].lineColorUniform)
	{
		sh::math::Vector3f color(1.0f, 0.0f, 0.0f);
		m_axises[0].lineColorUniform->Set(color);
	}

	m_axises[0].coneModel = sh::scene::GeometryGenerator::GetConeModel(
		sh::math::Vector3f(5.0f, 0.0f, 0.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 1.0f, 0.0f),
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));
	uniformBuffer = m_axises[0].coneModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[0].coneColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[0].coneColorUniform)
	{
		sh::math::Vector3f color(1.0f, 0.0f, 0.0f);
		m_axises[0].coneColorUniform->Set(color);
	}

	////////////////////////////////////////////////

	//m_axises[1].lineModel = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f, 1.0f, 0.0f), sh::math::Vector3f(0.0f, 5.0f, 0.0f));	
	m_axises[1].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(
		sh::math::Vector3f(0.0f, 1.0f, 0.0f),
		0.1f,
		sh::math::Vector3f(0.0f, 4.0f, 0.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));	
	uniformBuffer = m_axises[1].lineModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[1].lineColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[1].lineColorUniform)
	{
		sh::math::Vector3f color(0.0f, 1.0f, 0.0f);
		m_axises[1].lineColorUniform->Set(color);
	}

	m_axises[1].coneModel = sh::scene::GeometryGenerator::GetConeModel(
		sh::math::Vector3f(0.0f, 5.0f, 0.0f),
		sh::math::Vector3f(0.0f, 1.0f, 0.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));
	uniformBuffer = m_axises[1].coneModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[1].coneColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[1].coneColorUniform)
	{
		sh::math::Vector3f color(0.0f, 1.0f, 0.0f);
		m_axises[1].coneColorUniform->Set(color);
	}

	/////////////////////////////////////////////////

	//m_axises[2].lineModel = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f, 0.0f, 1.0f), sh::math::Vector3f(0.0f, 0.0f, 5.0f));
	m_axises[2].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(
		sh::math::Vector3f(0.0f, 0.0f, 1.0f),
		0.1f,
		sh::math::Vector3f(0.0f, 0.0f, 4.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	uniformBuffer = m_axises[2].lineModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[2].lineColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[2].lineColorUniform)
	{
		sh::math::Vector3f color(0.0f, 0.0f, 1.0f);
		m_axises[2].lineColorUniform->Set(color);
	}

	m_axises[2].coneModel = sh::scene::GeometryGenerator::GetConeModel(
		sh::math::Vector3f(0.0f, 0.0f, 5.0f),
		sh::math::Vector3f(0.0f, 0.0f, 1.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 1.0f, 0.0f));
	uniformBuffer = m_axises[2].coneModel->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	m_axises[2].coneColorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (m_axises[2].coneColorUniform)
	{
		sh::math::Vector3f color(0.0f, 0.0f, 1.0f);
		m_axises[2].coneColorUniform->Set(color);
	}
}

//////////////////////////////////////////////////////////////////////////

MoveGizmo::~MoveGizmo()
{
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::Render()
{
	if (!m_axises[0].lineModel || !m_axises[1].lineModel || !m_axises[2].lineModel ||
		!m_axises[0].coneModel || !m_axises[1].coneModel || !m_axises[2].coneModel ||
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
				m_axises[i].lineModel->SetWorldMatrix(matrix);
				m_axises[i].lineModel->UpdateTransformationUniforms();

				m_axises[i].coneModel->SetWorldMatrix(matrix);
				m_axises[i].coneModel->UpdateTransformationUniforms();
			}
		}
	}


	for (size_t i = 0; i < 3; ++i)
	{
		driver->Render(m_axises[i].lineModel.get());
		driver->Render(m_axises[i].coneModel.get());
	}
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::Process()
{

}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::OnMousePressed(sh::u32 x, sh::u32 y) 
{
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::OnMouseReleased(sh::u32 x, sh::u32 y) 
{
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::OnMouseMoved(sh::u32 x, sh::u32 y) 
{
	if (m_axises[0].active)
	{
		Move(Axis::Type::X_AXIS);		
	}
	else if (m_axises[1].active)
	{
		Move(Axis::Type::Y_AXIS);
	}
	else if (m_axises[2].active)
	{
		Move(Axis::Type::Z_AXIS);
	}
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::Move(Axis::Type axis)
{
	if (!m_entity)
		return;

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
	sh::math::Vector3f orthoVector(camera->GetUpVector());

	switch (axis)
	{
	case Axis::Type::X_AXIS:
		axisDir = sh::scene::SceneManager::GetRightVector();
		break;
	case Axis::Type::Y_AXIS:
		axisDir = sh::scene::SceneManager::GetUpVector();
		break;
	case Axis::Type::Z_AXIS:
		axisDir = -sh::scene::SceneManager::GetFrontVector();
		break;
	}
	
	sh::math::Vector3f direction = rotation * axisDir;
	if (orthoVector.Equals(direction, sh::math::k_eps_5) || (orthoVector + direction).GetLength() < sh::math::k_eps_5)
	{
		orthoVector = camera->GetRightVector();
	}
	sh::math::Planef plane(pos, pos + orthoVector, pos + direction);

	sh::math::Vector3f intersectionOld(0.0f), intersectionCurrent(0.0f);
	plane.GetIntersectionWithLine(rayOrigin, rayDirOld, intersectionOld);
	plane.GetIntersectionWithLine(rayOrigin, rayDirCurrent, intersectionCurrent);
	sh::math::Vector3f delta = intersectionCurrent - intersectionOld;
	sh::f32 deltaPart = delta.Dot(direction);
	direction *= deltaPart;

	transformComponent->SetPosition(pos + direction);

	inputManager->SetMousePositionOld(current);
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
{
	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

	unsigned char data[4];
	driver->GetPixelData(x, y, width, height, data);
	sh::math::Vector3f color(1.0f, 1.0f, 1.0f);
	if (data[0] == 255 && data[1] == 0 && data[2] == 0)
	{
		m_axises[Axis::Type::X_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = true;

		color = sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		m_axises[Axis::Type::Y_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 0.0f, 1.0f);
		m_axises[Axis::Type::Z_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
	else if (data[0] == 0 && data[1] == 255 && data[2] == 0)
	{
		m_axises[Axis::Type::Y_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = true;

		color = sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		m_axises[Axis::Type::X_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 0.0f, 1.0f);
		m_axises[Axis::Type::Z_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
	else if (data[0] == 0 && data[1] == 0 && data[2] == 255)
	{
		m_axises[Axis::Type::Z_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = true;

		color = sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		m_axises[Axis::Type::X_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		m_axises[Axis::Type::Y_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = false;
	}
	else if (data[0] == 255 && data[1] == 255 && data[2] == 255)
	{
		return;
	}
	else
	{
		color = sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		m_axises[Axis::Type::X_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::X_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		m_axises[Axis::Type::Y_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Y_AXIS].active = false;

		color = sh::math::Vector3f(0.0f, 0.0f, 1.0f);
		m_axises[Axis::Type::Z_AXIS].lineColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].coneColorUniform->Set(color);
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
}

//////////////////////////////////////////////////////////////////////////

bool MoveGizmo::IsActive() const
{
	if (m_axises[0].active || m_axises[1].active || m_axises[2].active)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////