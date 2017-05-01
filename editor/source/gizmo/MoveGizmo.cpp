#include "MoveGizmo.h"


MoveGizmo::MoveGizmo()
{
	m_material.reset(new sh::video::Material());
	m_material->SetRenderTechnique("editor_base_color.xml");

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
	sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
	sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();


	if (m_entity)
	{
		sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>(m_entity->GetComponent(sh::Component::Type::TRANSFORM));
		if (transformComponent)
		{
			sh::math::Matrix4f matrix;
			matrix.SetIdentity();

			sh::math::Vector3f position = transformComponent->GetPosition();
			sh::math::Quaternionf rotation = transformComponent->GetRotation();
			sh::f32 scaleFactor = (camera->GetPosition() - position).GetLength() / 35.0f;
			sh::math::Vector3f scale(scaleFactor);

			matrix.SetScale(scale);
			matrix.SetTranslation(position);
			matrix = matrix * rotation.GetAsMatrix4();

			sh::math::Matrix4f wvpMatrix = projectionMatrix * viewMatrix * matrix;

			for (size_t i = 0; i < 6; ++i)
			{
				m_axises[i].wvpMatrix.Set(wvpMatrix);
			}
		}
	}


	driver->SetRenderPipeline(m_material->GetRenderPipeline());

	for (size_t i = 0; i < 6; ++i)
	{
		auto& axis = m_axises[i];

		for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size(); ++modelIdx)
		{
			sh::u32 meshesCount = m_axises[i].models[modelIdx].model->GetMeshesCount();
			for (size_t j = 0; j < meshesCount; ++j)
			{
				auto& model = axis.models[modelIdx];
				const auto& mesh = model.model->GetMesh(j);
				const auto& renderable = mesh->GetRanderable();

				axis.color.Set(model.currentColor);
				driver->SetGpuParams(axis.params);
			
				driver->SetVertexBuffer(renderable->GetVertexBuffer());
				driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration());
				driver->SetTopology(renderable->GetTopology());
				if (renderable->GetIndexBuffer())
				{
					driver->SetIndexBuffer(renderable->GetIndexBuffer());				
					driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
				}
				else
				{
					driver->Draw(0, renderable->GetVertexBuffer()->GetVerticesCount());
				}
				
			}
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
	TryToSelect(x, y, 640, 480);
	m_mousePressed = true;
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::OnMouseReleased(sh::u32 x, sh::u32 y) 
{
	TryToSelect(x, y, 640, 480);
	m_mousePressed = false;
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::OnMouseMoved(sh::u32 x, sh::u32 y) 
{
	for (size_t i = 0; i < static_cast<size_t>(Axis::Type::COUNT); ++i)
	{
		if (m_axises[i].active && m_mousePressed)
		{
			Move(static_cast<Axis::Type>(i));
			return;
		}
	}

	TryToSelect(x, y, 640, 480);
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::CreateArrow(Axis::Type type)
{
	const auto& info = m_material->GetRenderPipeline()->GetAutoParamsInfo();

	auto& axis = m_axises[(Axis::Type)type];
	axis.params = sh::video::GpuParams::Create(info);
	axis.params->GetParam("matWVP", axis.wvpMatrix);
	axis.params->GetParam("color", axis.color);

	float radius = 0.1f;
	float height = 4.0f;
	sh::u32 numberOfSides = 100U;
	
	sh::math::Vector3f translation(0.0f);
	sh::math::Vector4f defaultColor;
	sh::math::Vector4f selectedColor(1.0f);
	sh::math::Quaternionf rotation;	
	sh::math::Matrix4f transform;
	transform.SetIdentity();

	switch (type)
	{
		case Axis::Type::X_AXIS:
		{
			translation = sh::math::Vector3f(3.0f, 0.0f, 0.0f);
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
			defaultColor = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
			break;
		case Axis::Type::Y_AXIS:
		{
			translation = sh::math::Vector3f(0.0f, 3.0f, 0.0f);
			rotation.SetIndentity();
			defaultColor = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
		}
			break;
		case Axis::Type::Z_AXIS:
		{
			translation = sh::math::Vector3f(0.0f, 0.0f, 3.0f);
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			defaultColor = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
		}
			break;
		default:
			SH_ASSERT(0, "Invalid type for creation Movegizmo axis!!!");
	}

	
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();

	Axis::ModelInfo modelInfo;
	modelInfo.model = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);	
	modelInfo.model->SetMaterial(m_material);
	modelInfo.defaultColor = defaultColor;
	modelInfo.selectedColor = selectedColor;
	modelInfo.currentColor = defaultColor;
	axis.models.push_back(modelInfo);

	transform.SetIdentity();
	translation *= 1.5f;
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();
	modelInfo.model = sh::scene::GeometryGenerator::GetConeModel(0.25f, 1.0f, transform);
	modelInfo.model->SetMaterial(m_material);
	modelInfo.defaultColor = defaultColor;
	modelInfo.selectedColor = selectedColor;
	modelInfo.currentColor = defaultColor;
	axis.models.push_back(modelInfo);
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::CreatePlane(Axis::Type type)
{
	const auto& info = m_material->GetRenderPipeline()->GetAutoParamsInfo();
	auto& axis = m_axises[type];
	axis.params = sh::video::GpuParams::Create(info);
	axis.params->GetParam("matWVP", axis.wvpMatrix);
	axis.params->GetParam("color", axis.color);

	float radius = 0.1f;
	float height = 3.0f;
	sh::u32 numberOfSides = 100U;

	sh::math::Matrix4f transform1 = sh::math::Matrix4f::Identity();
	sh::math::Matrix4f transform2 = sh::math::Matrix4f::Identity();
	sh::math::Matrix4f transform3 = sh::math::Matrix4f::Identity();
	sh::math::Vector4f color1_default(0.0f);
	sh::math::Vector4f color1_selected(0.0f);
	sh::math::Vector4f color2_default(0.0f);
	sh::math::Vector4f color2_selected(0.0f);
	sh::math::Vector4f color3_default(0.0f);
	sh::math::Vector4f color3_selected(0.0f);
	switch (type)
	{
		case Axis::Type::XY_PLANE:
		{
			sh::math::Quaternionf rotation;

			transform1.SetTranslation(sh::math::Vector3f(height, height / 2.0f, 0.0f));
			color1_default = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
			color1_selected = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

			transform2.SetTranslation(sh::math::Vector3f(height / 2.0f, height, 0.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
			transform2 = transform2 * rotation.GetAsMatrix4();
			color2_default = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
			color2_selected = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

			transform3.SetTranslation(sh::math::Vector3f(height / 2.0f, height / 2.0f, 0.0f));
			color3_default = sh::math::Vector4f(1.0f, 1.0f, 0.0f, 0.2f);
			color3_selected = sh::math::Vector4f(1.0f, 1.0f, 1.0f, 0.5f);
		}
			break;
		case Axis::Type::XZ_PLANE:
		{
			sh::math::Quaternionf rotation;

			transform1.SetTranslation(sh::math::Vector3f(height, 0.0f, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			transform1 = transform1 * rotation.GetAsMatrix4();
			color1_default = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
			color1_selected = sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

			transform2.SetTranslation(sh::math::Vector3f(height / 2.0f, 0.0f, height));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
			transform2 = transform2 * rotation.GetAsMatrix4();
			color2_default = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
			color2_selected = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

			transform3.SetTranslation(sh::math::Vector3f(height / 2.0f, 0.0f, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			transform3 = transform3 * rotation.GetAsMatrix4();
			color3_default = sh::math::Vector4f(1.0f, 0.0f, 1.0f, 0.2f);
			color3_selected = sh::math::Vector4f(1.0f, 1.0f, 1.0f, 0.5f);
		}
			break;
		case Axis::Type::YZ_PLANE:
		{
			sh::math::Quaternionf rotation;
			transform1.SetTranslation(sh::math::Vector3f(0.0f, height, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
			transform1 = transform1 * rotation.GetAsMatrix4();
			color1_default = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
			color1_selected = sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

			transform2.SetTranslation(sh::math::Vector3f(0.0f, height / 2.0f, height));
			color2_default = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
			color2_selected = sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

			transform3.SetTranslation(sh::math::Vector3f(0.0f, height / 2.0f, height / 2.0f));
			rotation.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), sh::math::k_pi_2);
			transform3 = transform3 * rotation.GetAsMatrix4();
			color3_default = sh::math::Vector4f(0.0f, 1.0f, 1.0f, 0.2f);
			color3_selected = sh::math::Vector4f(1.0f, 1.0f, 1.0f, 0.5f);
		}
			break;
		default:
			SH_ASSERT(0, "Invalid type for creation Movegizmo plane!!!");
	}


	Axis::ModelInfo modelInfo;
	modelInfo.model = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform1);	
	modelInfo.model->SetMaterial(m_material);
	modelInfo.defaultColor = color1_default;
	modelInfo.selectedColor = color1_selected;
	modelInfo.currentColor = color1_default;
	axis.models.push_back(modelInfo);



	modelInfo.model = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform2);	
	modelInfo.model->SetMaterial(m_material);
	modelInfo.defaultColor = color2_default;
	modelInfo.selectedColor = color2_selected;
	modelInfo.currentColor = color2_default;
	axis.models.push_back(modelInfo);

	///////////////////////////////////////////////////////////////////////

	modelInfo.model = sh::scene::GeometryGenerator::GetPlaneModel(height, height, transform3);
	modelInfo.model->SetMaterial(m_material);
	modelInfo.defaultColor = color3_default;
	modelInfo.selectedColor = color3_selected;
	modelInfo.currentColor = color3_default;
	axis.models.push_back(modelInfo);
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
}

//////////////////////////////////////////////////////////////////////////

void MoveGizmo::SetModifierActive(Axis::Type idx, bool active)
{
	if (idx >= Axis::COUNT)
		return;

	
	auto& axis = m_axises[idx];
	axis.active = active;

	for (auto& model : axis.models)
	{
		if (active)
			model.currentColor = model.selectedColor;
		else
			model.currentColor = model.defaultColor;
	}
}

void MoveGizmo::TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
{
	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
	driver->ClearBuffers();


	driver->SetRenderPipeline(m_material->GetRenderPipeline());

	for (size_t i = 0; i < 3; ++i)
	{
		driver->SetGpuParams(m_axises[i].params);

		for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size(); ++modelIdx)
		{
			sh::u32 meshesCount = m_axises[i].models[modelIdx].model->GetMeshesCount();
			for (size_t j = 0; j < meshesCount; ++j)
			{
				const auto& mesh = m_axises[i].models[modelIdx].model->GetMesh(j);
				const auto& renderable = mesh->GetRanderable();

			
				driver->SetVertexBuffer(renderable->GetVertexBuffer());
				driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration());
				driver->SetTopology(renderable->GetTopology());
				if (renderable->GetIndexBuffer())
				{
					driver->SetIndexBuffer(renderable->GetIndexBuffer());				
					driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
				}
				else
				{
					driver->Draw(0, renderable->GetVertexBuffer()->GetVerticesCount());
				}
				
			}
		}
	}

	

	unsigned char data[4];
	driver->GetPixelData(x, y, width, height, data);

	if (data[0] == 255 && data[1] == 0 && data[2] == 0)
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::X_AXIS;
		SetModifierActive(m_activeModifier, true);
		return;
	}
	else if (data[0] == 0 && data[1] == 255 && data[2] == 0)
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::Y_AXIS;
		SetModifierActive(m_activeModifier, true);
		return;
	}
	else if (data[0] == 0 && data[1] == 0 && data[2] == 255)
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::Z_AXIS;
		SetModifierActive(m_activeModifier, true);
		return;
	}
	else if (data[0] == 255 && data[1] == 255 && data[2] == 255)
	{
		return;
	}
	
	driver->ClearBuffers();

	///////////////////////////////////////////////////////////////

	for (size_t i = 3; i < 6; ++i)
	{
		sh::math::Vector4f color = m_axises[i].color.Get();
		color.w = 1.0f;
		m_axises[i].color.Set(color);
		driver->SetGpuParams(m_axises[i].params);

		for (size_t modelIdx = 0U; modelIdx < m_axises[i].models.size(); ++modelIdx)
		{
			sh::u32 meshesCount = m_axises[i].models[modelIdx].model->GetMeshesCount();
			for (size_t j = 0; j < meshesCount; ++j)
			{
				const auto& mesh = m_axises[i].models[modelIdx].model->GetMesh(j);
				const auto& renderable = mesh->GetRanderable();

			
				driver->SetVertexBuffer(renderable->GetVertexBuffer());
				driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration());
				driver->SetTopology(renderable->GetTopology());
				if (renderable->GetIndexBuffer())
				{
					driver->SetIndexBuffer(renderable->GetIndexBuffer());				
					driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
				}
				else
				{
					driver->Draw(0, renderable->GetVertexBuffer()->GetVerticesCount());
				}
				
			}
		}
	}

	driver->GetPixelData(x, y, width, height, data);

	if (data[0] == 255 && data[1] == 255 && data[2] == 0)
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::XY_PLANE;
		SetModifierActive(m_activeModifier, true);
		return;
	}
	else if (data[0] == 255 && data[1] == 0 && data[2] == 255)
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::XZ_PLANE;
		SetModifierActive(m_activeModifier, true);
		return;
	}
	else if (data[0] == 0 && data[1] == 255 && data[2] == 255)
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::YZ_PLANE;
		SetModifierActive(m_activeModifier, true);
		return;
	}
	else if (data[0] == 255 && data[1] == 255 && data[2] == 255)
	{
		return;
	}
	else
	{
		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::NONE;
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