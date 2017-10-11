#include "ScaleGizmo.h"

#include "../gui/inspectorwidget/transformcomponent/TransformComponentWidget.h"

ScaleGizmo::ScaleGizmo()
{
	m_material.reset(new sh::video::Material());
	m_material->SetRenderTechnique("editor_base_color.rt");

	sh::video::CommandBufferDescription commandBufferDesc;
	commandBufferDesc.type = sh::COMMAND_BUFFER_TYPE_SECONDARY;
	m_commandBuffer = sh::video::CommandBuffer::Create(commandBufferDesc);

	CreateArrow(Axis::Type::X_AXIS);
	CreateArrow(Axis::Type::Y_AXIS);
	CreateArrow(Axis::Type::Z_AXIS);
}

//////////////////////////////////////////////////////////////////////////

ScaleGizmo::~ScaleGizmo()
{

}

//////////////////////////////////////////////////////////////////////////

void ScaleGizmo::Render()
{
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
		auto transformComponent = m_entity->GetComponent<sh::TransformComponent>();
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
			wvpMatrix.Transpose();

			for (size_t i = 0; i < Axis::COUNT; ++i)
			{
				m_axises[i].wvpMatrix.Set(wvpMatrix);
			}
		}
	}

	m_commandBuffer->Begin();

	driver->SetRenderPipeline(m_material->GetRenderPipeline(), m_commandBuffer);

	for (size_t i = 0; i < Axis::COUNT; ++i)
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
				driver->SetGpuParams(axis.params, m_commandBuffer);

				driver->SetVertexBuffer(renderable->GetVertexBuffer(), m_commandBuffer);
				driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration(), m_commandBuffer);
				driver->SetTopology(renderable->GetTopology(), m_commandBuffer);
				if (renderable->GetIndexBuffer())
				{
					driver->SetIndexBuffer(renderable->GetIndexBuffer(), m_commandBuffer);
					driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount(), 1U, m_commandBuffer);
				}
				else
				{
					driver->Draw(0, renderable->GetVertexBuffer()->GetVerticesCount(), 1U, m_commandBuffer);
				}

			}
		}
	}

	m_commandBuffer->End();

	driver->SubmitCommandBuffer(m_commandBuffer);

	DrawBoundingBox();
}

//////////////////////////////////////////////////////////////////////////

void ScaleGizmo::Process()
{

}

//////////////////////////////////////////////////////////////////////////

void ScaleGizmo::OnMouseMoved(sh::u32 x, sh::u32 y)
{
	for (size_t i = 0; i < static_cast<size_t>(Axis::Type::COUNT); ++i)
	{
		if (m_axises[i].active && m_mousePressed)
		{
			Scale(static_cast<Axis::Type>(i));
			return;
		}
	}

	TryToSelect(x, y);
}

//////////////////////////////////////////////////////////////////////////

bool ScaleGizmo::TryToSelect(sh::u32 x, sh::u32 y)
{
	auto transformComponent = m_entity->GetComponent<sh::TransformComponent>();
	if (transformComponent)
	{
		sh::math::Matrix4f matrix;
		sh::math::Matrix4f invMatrix;
		matrix.SetIdentity();
		sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();

		sh::math::Vector3f position = transformComponent->GetPosition();
		sh::math::Quaternionf rotation = transformComponent->GetRotation();
		sh::f32 scaleFactor = (camera->GetPosition() - position).GetLength() / 35.0f;
		sh::math::Vector3f scale(scaleFactor);

		matrix.SetScale(scale);
		matrix.SetTranslation(position);
		matrix = matrix * rotation.GetAsMatrix4();
		invMatrix = matrix.GetInversed();

		sh::math::Vector3f localX = rotation.GetAsMatrix3() * sh::math::Vector3f(1.0f, 0.0f, 0.0f);
		sh::math::Vector3f localY = rotation.GetAsMatrix3() * sh::math::Vector3f(0.0f, 1.0f, 0.0f);
		sh::math::Vector3f localZ = rotation.GetAsMatrix3() * sh::math::Vector3f(0.0f, 0.0f, 1.0f);

		sh::math::Vector3f rayOrigin(0.0f);
		sh::math::Vector3f rayDirection(0.0f);
		camera->BuildRay(x, y, rayOrigin, rayDirection);

		// Intersection with X/Y - axis
		sh::math::Planef plane(position, position + localX, position + localY);
		sh::math::Vector3f iPoint(0.0f);
		bool res = plane.GetIntersectionWithLine(rayOrigin, rayDirection, iPoint);
		iPoint = invMatrix * iPoint;

		float length = 5.0f;
		float halfLength = length * 0.6f;

		bool inters = false;
		Axis::Type oldModifier = m_activeModifier;
		if (iPoint.x > 0.0f && iPoint.x < length && fabs(iPoint.y) < 0.15f) { m_activeModifier = Axis::Type::X_AXIS; inters = true; }
		else if (iPoint.y > 0.0f && iPoint.y < length && fabs(iPoint.x) < 0.15f) { m_activeModifier = Axis::Type::Y_AXIS; inters = true; }
		if (inters)
		{
			SetModifierActive(oldModifier, false);
			SetModifierActive(m_activeModifier, true);
			return true;
		}

		// Intersection with Y/Z - axis
		plane.SetPlane(position, position + localY, position + localZ);
		res = plane.GetIntersectionWithLine(rayOrigin, rayDirection, iPoint);
		iPoint = invMatrix * iPoint;
		if (iPoint.y > 0.0f && iPoint.y < length && fabs(iPoint.z) < 0.15f) { m_activeModifier = Axis::Type::Y_AXIS; inters = true; }
		else if (iPoint.z > 0.0f && iPoint.z < length && fabs(iPoint.y) < 0.15f) { m_activeModifier = Axis::Type::Z_AXIS; inters = true; }
		if (inters)
		{
			SetModifierActive(oldModifier, false);
			SetModifierActive(m_activeModifier, true);
			return true;
		}

		// Intersection with X/Z - axis
		plane.SetPlane(position, position + localX, position + localZ);
		res = plane.GetIntersectionWithLine(rayOrigin, rayDirection, iPoint);
		iPoint = invMatrix * iPoint;
		if (iPoint.x > 0.0f && iPoint.x < length && fabs(iPoint.z) < 0.15f) { m_activeModifier = Axis::Type::X_AXIS; inters = true; }
		else if (iPoint.z > 0.0f && iPoint.z < length && fabs(iPoint.x) < 0.15f) { m_activeModifier = Axis::Type::Z_AXIS; inters = true; }
		if (inters)
		{
			SetModifierActive(oldModifier, false);
			SetModifierActive(m_activeModifier, true);
			return true;
		}

		SetModifierActive(m_activeModifier, false);
		m_activeModifier = Axis::Type::NONE;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool ScaleGizmo::IsActive() const
{
	if (m_axises[0].active || m_axises[1].active || m_axises[2].active)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void ScaleGizmo::CreateArrow(Axis::Type type)
{
	const auto& info = m_material->GetRenderPipeline()->GetParamsInfo();

	auto& axis = m_axises[(Axis::Type)type];
	axis.params = sh::video::GpuParams::Create(info);
	axis.params->GetParam("matrixWVP", axis.wvpMatrix);
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
		SH_ASSERT(0, "Invalid type for creation Scalegizmo axis!!!");
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

void ScaleGizmo::Scale(Axis::Type axis)
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


	auto transformComponent = m_entity->GetComponent<sh::TransformComponent>();
	sh::math::Vector3f pos = transformComponent->GetPosition();
	sh::math::Vector3f scale = transformComponent->GetScale();
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
		direction = axisDir;
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
		direction = axisDir;
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
		direction = axisDir;
		if (orthoVector.Equals(direction, sh::math::k_eps_5) || (orthoVector + direction).GetLength() < sh::math::k_eps_5)
		{
			orthoVector = camera->GetRightVector();
		}
		plane = sh::math::Planef(pos, pos + orthoVector, pos + direction);
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

	transformComponent->SetScale(scale + direction);

	if (m_transformWidget)
		m_transformWidget->Update();
}

//////////////////////////////////////////////////////////////////////////

void ScaleGizmo::SetModifierActive(Axis::Type idx, bool active)
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

//////////////////////////////////////////////////////////////////////////