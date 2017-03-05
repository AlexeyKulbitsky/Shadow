#include "MoveGizmo.h"


MoveGizmo::MoveGizmo()
{
	CreateArrow(Axis::Type::X_AXIS);
	CreateArrow(Axis::Type::Y_AXIS);
	CreateArrow(Axis::Type::Z_AXIS);

	CreatePlane(Axis::Type::XY_PLANE);
	CreatePlane(Axis::Type::XZ_PLANE);
	CreatePlane(Axis::Type::YZ_PLANE);
}

//////////////////////////////////////////////////////////////////////////

MoveGizmo::~MoveGizmo()
{
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::Render()
{
	//if (!m_axises[0].lineModel || !m_axises[1].lineModel || !m_axises[2].lineModel ||
	//	!m_axises[0].coneModel || !m_axises[1].coneModel || !m_axises[2].coneModel ||
	//	!m_entity)
	//	return;
	if (!m_entity)
	{
		return;
	}

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

			for (size_t i = 0; i < 6; ++i)
			{
				for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size(); ++modelIdx)
				{
					m_axises[i].models[modelIdx].model->SetWorldMatrix(matrix);
					m_axises[i].models[modelIdx].model->UpdateTransformationUniforms();
				}
			}
		}
	}


	
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size(); ++modelIdx)
		{
			driver->Render(m_axises[i].models[modelIdx].model.get());
			
		}
	}

	for (size_t i = 3; i < 6; ++i)
	{
		for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size() - 1; ++modelIdx)
		{
			driver->Render(m_axises[i].models[modelIdx].model.get());			
		}
		if (m_axises[i].active)
		{
			driver->Render(m_axises[i].models[m_axises[i].models.size() - 1].model.get());
		}
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
	/*
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
	*/

	for (size_t i = 0; i < static_cast<size_t>(Axis::Type::COUNT); ++i)
	{
		if (m_axises[i].active)
		{
			Move(static_cast<Axis::Type>(i));
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::CreateArrow(Axis::Type type)
{
	float radius = 0.1f;
	float height = 4.0f;
	sh::u32 numberOfSides = 100U;
	
	sh::math::Vector3f translation(0.0f);
	sh::math::Vector4f color;
	sh::math::Quaternionf rotation;	
	sh::math::Matrix4f transform;
	transform.SetIdentity();

	switch (type)
	{
		case Axis::Type::X_AXIS:
		{
			translation = sh::math::Vector3f(3.0f, 0.0f, 0.0f);
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
			color = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
			break;
		case Axis::Type::Y_AXIS:
		{
			translation = sh::math::Vector3f(0.0f, 3.0f, 0.0f);
			rotation.SetIndentity();
			color = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
		}
			break;
		case Axis::Type::Z_AXIS:
		{
			translation = sh::math::Vector3f(0.0f, 0.0f, 3.0f);
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			color = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
		}
			break;
		default:
			SH_ASSERT(0, "Invalid type for creation Movegizmo axis!!!");
	}

	
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();

	Axis::ModelInfo modelInfo;
	modelInfo.model = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);	
	sh::video::UniformBufferPtr uniformBuffer = modelInfo.model->GetMesh(0)->GetMaterial()->GetRenderPipeline()->GetUniformBuffer();
	modelInfo.uniform = uniformBuffer->GetUniform(sh::String("color"));
	if (modelInfo.uniform)
	{
		modelInfo.color = color;
		modelInfo.uniform->Set(modelInfo.color);
	}
	m_axises[(Axis::Type)type].models.push_back(modelInfo);

	transform.SetIdentity();
	translation *= 1.5f;
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();
	modelInfo.model = sh::scene::GeometryGenerator::GetConeModel(0.25f, 1.0f, transform);
	
	uniformBuffer = modelInfo.model->GetMesh(0)->GetMaterial()->GetRenderPipeline()->GetUniformBuffer();
	modelInfo.uniform = uniformBuffer->GetUniform(sh::String("color"));
	if (modelInfo.uniform)
	{
		modelInfo.color = color;
		modelInfo.uniform->Set(modelInfo.color);
	}
	m_axises[(Axis::Type)type].models.push_back(modelInfo);
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::CreatePlane(Axis::Type type)
{
	float radius = 0.1f;
	float height = 3.0f;
	sh::u32 numberOfSides = 100U;

	sh::math::Matrix4f transform1 = sh::math::Matrix4f::Identity();
	sh::math::Matrix4f transform2 = sh::math::Matrix4f::Identity();
	sh::math::Matrix4f transform3 = sh::math::Matrix4f::Identity();
	sh::math::Vector4f color1(0.0f);
	sh::math::Vector4f color2(0.0f);
	sh::math::Vector4f color3(0.0f);
	switch (type)
	{
		case Axis::Type::XY_PLANE:
		{
			sh::math::Quaternionf rotation;

			transform1.SetTranslation(sh::math::Vector3f(height, height / 2.0f, 0.0f));
			color1 = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

			transform2.SetTranslation(sh::math::Vector3f(height / 2.0f, height, 0.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
			transform2 = transform2 * rotation.GetAsMatrix4();
			color2 = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

			transform3.SetTranslation(sh::math::Vector3f(height / 2.0f, height / 2.0f, 0.0f));
			color3 = sh::math::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
		}
			break;
		case Axis::Type::XZ_PLANE:
		{
			sh::math::Quaternionf rotation;

			transform1.SetTranslation(sh::math::Vector3f(height, 0.0f, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			transform1 = transform1 * rotation.GetAsMatrix4();
			color1 = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

			transform2.SetTranslation(sh::math::Vector3f(height / 2.0f, 0.0f, height));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
			transform2 = transform2 * rotation.GetAsMatrix4();
			color2 = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

			transform3.SetTranslation(sh::math::Vector3f(height / 2.0f, 0.0f, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			transform3 = transform3 * rotation.GetAsMatrix4();
			color3 = sh::math::Vector4f(1.0f, 0.0f, 1.0f, 1.0f);
		}
			break;
		case Axis::Type::YZ_PLANE:
		{
			sh::math::Quaternionf rotation;
			transform1.SetTranslation(sh::math::Vector3f(0.0f, height, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			transform1 = transform1 * rotation.GetAsMatrix4();
			color1 = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

			transform2.SetTranslation(sh::math::Vector3f(0.0f, height / 2.0f, height));
			color2 = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

			transform3.SetTranslation(sh::math::Vector3f(0.0f, height / 2.0f, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), sh::math::k_pi_2);
			transform3 = transform3 * rotation.GetAsMatrix4();
			color3 = sh::math::Vector4f(0.0f, 1.0f, 1.0f, 1.0f);
		}
			break;
		default:
			SH_ASSERT(0, "Invalid type for creation Movegizmo plane!!!");
	}


	Axis::ModelInfo modelInfo;
	modelInfo.model = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform1);	
	sh::video::UniformBufferPtr uniformBuffer = modelInfo.model->GetMesh(0)->GetMaterial()->GetRenderPipeline()->GetUniformBuffer();
	modelInfo.uniform = uniformBuffer->GetUniform(sh::String("color"));
	if (modelInfo.uniform)
	{
		modelInfo.color = color1;
		modelInfo.uniform->Set(modelInfo.color);
	}
	m_axises[(Axis::Type)type].models.push_back(modelInfo);



	modelInfo.model = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform2);	
	uniformBuffer = modelInfo.model->GetMesh(0)->GetMaterial()->GetRenderPipeline()->GetUniformBuffer();
	modelInfo.uniform = uniformBuffer->GetUniform(sh::String("color"));
	if (modelInfo.uniform)
	{
		modelInfo.color = color2;
		modelInfo.uniform->Set(modelInfo.color);
	}
	m_axises[(Axis::Type)type].models.push_back(modelInfo);

	///////////////////////////////////////////////////////////////////////

	modelInfo.model = sh::scene::GeometryGenerator::GetPlaneModel(height, height, transform3);
	uniformBuffer = modelInfo.model->GetMesh(0)->GetMaterial()->GetRenderPipeline()->GetUniformBuffer();
	modelInfo.uniform = uniformBuffer->GetUniform(sh::String("color"));
	if (modelInfo.uniform)
	{
		modelInfo.color = color3;
		modelInfo.uniform->Set(modelInfo.color);
	}
	m_axises[(Axis::Type)type].models.push_back(modelInfo);
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

	sh::math::Planef plane;
	sh::math::Vector3f direction;
	switch (axis)
	{
		case Axis::Type::X_AXIS:
		{
			sh::math::Vector3f axisDir = sh::scene::SceneManager::GetRightVector();
			sh::math::Vector3f orthoVector(camera->GetUpVector());
			direction = rotation * axisDir;
			if (orthoVector.Equals(direction, sh::math::k_eps_5) || (orthoVector + direction).GetLength() < sh::math::k_eps_5)
			{
				orthoVector = camera->GetRightVector();
			}
			plane = sh::math::Planef(pos, pos + orthoVector, pos + direction);
		}		
		break;

		case Axis::Type::Y_AXIS:
		{
			sh::math::Vector3f axisDir = sh::scene::SceneManager::GetUpVector();
			sh::math::Vector3f orthoVector(camera->GetUpVector());
			direction = rotation * axisDir;
			if (orthoVector.Equals(direction, sh::math::k_eps_5) || (orthoVector + direction).GetLength() < sh::math::k_eps_5)
			{
				orthoVector = camera->GetRightVector();
			}
			plane = sh::math::Planef(pos, pos + orthoVector, pos + direction);
		}		
		break;

		case Axis::Type::Z_AXIS:
		{
			sh::math::Vector3f axisDir = -sh::scene::SceneManager::GetFrontVector();
			sh::math::Vector3f orthoVector(camera->GetUpVector());
			direction = rotation * axisDir;
			if (orthoVector.Equals(direction, sh::math::k_eps_5) || (orthoVector + direction).GetLength() < sh::math::k_eps_5)
			{
				orthoVector = camera->GetRightVector();
			}
			plane = sh::math::Planef(pos, pos + orthoVector, pos + direction);
		}		
		break;

		case Axis::Type::XY_PLANE:
		{
			sh::math::Vector3f axisDirX = rotation * sh::scene::SceneManager::GetRightVector();
			sh::math::Vector3f axisDirY = rotation * sh::scene::SceneManager::GetUpVector();
			plane = sh::math::Planef(pos, pos + axisDirX, pos + axisDirY);
		}
		break;

		case Axis::Type::XZ_PLANE:
		{
			sh::math::Vector3f axisDirX = rotation * sh::scene::SceneManager::GetRightVector();
			sh::math::Vector3f axisDirZ = rotation * (-sh::scene::SceneManager::GetFrontVector());
			plane = sh::math::Planef(pos, pos + axisDirX, pos + axisDirZ);
		}
		break;

		case Axis::Type::YZ_PLANE:
		{
			sh::math::Vector3f axisDirZ = rotation * (-sh::scene::SceneManager::GetFrontVector());
			sh::math::Vector3f axisDirY = rotation * sh::scene::SceneManager::GetUpVector();
			plane = sh::math::Planef(pos, pos + axisDirZ, pos + axisDirY);
		}
		break;

		default:
			break;
	}
	
	

	sh::math::Vector3f intersectionOld(0.0f), intersectionCurrent(0.0f);
	plane.GetIntersectionWithLine(rayOrigin, rayDirOld, intersectionOld);
	plane.GetIntersectionWithLine(rayOrigin, rayDirCurrent, intersectionCurrent);
	sh::math::Vector3f delta = intersectionCurrent - intersectionOld;

	if (axis == Axis::Type::X_AXIS || axis == Axis::Type::Y_AXIS || axis == Axis::Type::Z_AXIS)
	{
		sh::f32 deltaPart = delta.Dot(direction);
		direction *= deltaPart;
	}
	else if (axis == Axis::Type::XY_PLANE || axis == Axis::Type::XZ_PLANE || axis == Axis::Type::YZ_PLANE)
	{
		direction = delta;
	}


	transformComponent->SetPosition(pos + direction);

	inputManager->SetMousePositionOld(current);
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
{
	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
	driver->ClearBuffers();


	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size(); ++modelIdx)
		{
			driver->Render(m_axises[i].models[modelIdx].model.get());
			
		}
	}

	








	unsigned char data[4];
	driver->GetPixelData(x, y, width, height, data);
	sh::math::Vector4f color(1.0f, 1.0f, 1.0f, 1.0f);
	if (data[0] == 255 && data[1] == 0 && data[2] == 0)
	{
		for (size_t i = 0; i < m_axises[Axis::Type::X_AXIS].models.size(); ++i)
		{
			m_axises[Axis::Type::X_AXIS].models[i].uniform->Set(color);
		}
		m_axises[Axis::Type::X_AXIS].active = true;

		for (size_t i = 0; i < m_axises[Axis::Type::Y_AXIS].models.size(); ++i)
		{
			const auto& col = m_axises[Axis::Type::Y_AXIS].models[i].color;
			m_axises[Axis::Type::Y_AXIS].models[i].uniform->Set(col);
		}
		m_axises[Axis::Type::Y_AXIS].active = false;

		for (size_t i = 0; i < m_axises[Axis::Type::Z_AXIS].models.size(); ++i)
		{
			const auto& col = m_axises[Axis::Type::Z_AXIS].models[i].color;
			m_axises[Axis::Type::Z_AXIS].models[i].uniform->Set(col);
		}
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
	else if (data[0] == 0 && data[1] == 255 && data[2] == 0)
	{
		for (size_t i = 0; i < m_axises[Axis::Type::Y_AXIS].models.size(); ++i)
		{
			m_axises[Axis::Type::Y_AXIS].models[i].uniform->Set(color);
		}
		m_axises[Axis::Type::Y_AXIS].active = true;

		for (size_t i = 0; i < m_axises[Axis::Type::X_AXIS].models.size(); ++i)
		{
			const auto& col = m_axises[Axis::Type::X_AXIS].models[i].color;
			m_axises[Axis::Type::X_AXIS].models[i].uniform->Set(col);
		}
		m_axises[Axis::Type::X_AXIS].active = false;

		for (size_t i = 0; i < m_axises[Axis::Type::Z_AXIS].models.size(); ++i)
		{
			const auto& col = m_axises[Axis::Type::Z_AXIS].models[i].color;
			m_axises[Axis::Type::Z_AXIS].models[i].uniform->Set(col);
		}
		m_axises[Axis::Type::Z_AXIS].active = false;
	}
	else if (data[0] == 0 && data[1] == 0 && data[2] == 255)
	{
		for (size_t i = 0; i < m_axises[Axis::Type::Z_AXIS].models.size(); ++i)
		{
			m_axises[Axis::Type::Z_AXIS].models[i].uniform->Set(color);
		}
		m_axises[Axis::Type::Z_AXIS].active = true;

		for (size_t i = 0; i < m_axises[Axis::Type::X_AXIS].models.size(); ++i)
		{
			const auto& col = m_axises[Axis::Type::X_AXIS].models[i].color;
			m_axises[Axis::Type::X_AXIS].models[i].uniform->Set(col);
		}
		m_axises[Axis::Type::X_AXIS].active = false;

		for (size_t i = 0; i < m_axises[Axis::Type::Y_AXIS].models.size(); ++i)
		{
			const auto& col = m_axises[Axis::Type::Y_AXIS].models[i].color;
			m_axises[Axis::Type::Y_AXIS].models[i].uniform->Set(col);
		}
		m_axises[Axis::Type::Y_AXIS].active = false;
	}
	else if (data[0] == 255 && data[1] == 255 && data[2] == 255)
	{
		return;
	}
	else
	{
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < m_axises[i].models.size(); ++j)
			{
				const auto& col = m_axises[i].models[j].color;
				m_axises[i].models[j].uniform->Set(col);
			}
			m_axises[i].active = false;
		}
		
	}
	

	///////////////////////////////////////////////////////////////

	for (size_t i = 3; i < 6; ++i)
	{
		const auto& color = m_axises[i].models[m_axises[i].models.size() - 1].color;
		m_axises[i].models[m_axises[i].models.size() - 1].uniform->Set(color);
		driver->Render(m_axises[i].models[m_axises[i].models.size() - 1].model.get());
		m_axises[i].models[m_axises[i].models.size() - 1].uniform->Set(sh::math::Vector4f(1.0f, 1.0f, 1.0f, 0.7f));
	}

	driver->GetPixelData(x, y, width, height, data);

	if (data[0] == 255 && data[1] == 255 && data[2] == 0)
	{
		m_axises[3].active = true;

		m_axises[4].active = false;
		m_axises[5].active = false;
	}
	else if (data[0] == 255 && data[1] == 0 && data[2] == 255)
	{
		m_axises[4].active = true;

		m_axises[3].active = false;
		m_axises[5].active = false;
	}
	else if (data[0] == 0 && data[1] == 255 && data[2] == 255)
	{
		m_axises[5].active = true;

		m_axises[3].active = false;
		m_axises[4].active = false;
	}
	else if (data[0] == 255 && data[1] == 255 && data[2] == 255)
	{
		return;
	}
	else
	{
		for (size_t i = 3; i < 6; ++i)
		{			
			m_axises[i].active = false;
		}
	}

	driver->ClearBuffers();
}

//////////////////////////////////////////////////////////////////////////

bool MoveGizmo::IsActive() const
{
	if (m_axises[0].active || m_axises[1].active || m_axises[2].active ||
		m_axises[3].active || m_axises[4].active || m_axises[5].active)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////