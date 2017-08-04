#include "Gizmo.h"

///////////////////////////////////////////////////////////////////

Gizmo::Gizmo()
{
	m_material.reset(new sh::video::Material());
	m_material->SetRenderTechnique("editor_base_color.xml");
	const auto& info = m_material->GetRenderPipeline()->GetAutoParamsInfo();

	sh::Device::GetInstance()->GetDriver()->GetPainter()->SetMaterial(m_material);

	float radius = 0.1f;
	float height = 3.0f;
	sh::u32 numberOfSides = 100U;

	sh::math::Vector3f translation(0.0f);
	sh::math::Quaternionf rotation;
	sh::math::Matrix4f transform;
	transform.SetIdentity();

	rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
	translation = sh::math::Vector3f(height / 2.0f, 0.0f, 0.0f);
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();

	m_axises[0].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);
	m_axises[0].lineModel->SetMaterial(m_material);
	m_axises[0].params = sh::video::GpuParams::Create(info);
	m_axises[0].params->GetParam("matWVP", m_axises[0].wvpMtrix);
	m_axises[0].params->GetParam("color", m_axises[0].color);
	m_axises[0].color.Set(sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));

	////////////////////////////////////////////////

	translation = sh::math::Vector3f(0.0f, height / 2.0f, 0.0f);
	transform.SetIdentity();
	transform.SetTranslation(translation);
	m_axises[1].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);
	m_axises[1].lineModel->SetMaterial(m_material);
	m_axises[1].params = sh::video::GpuParams::Create(info);
	m_axises[1].params->GetParam("matWVP", m_axises[1].wvpMtrix);
	m_axises[1].params->GetParam("color", m_axises[1].color);
	m_axises[1].color.Set(sh::math::Vector4f(0.0f, 1.0f, 0.0f, 1.0f));

	/////////////////////////////////////////////////

	rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
	translation = sh::math::Vector3f(0.0f, 0.0f, height / 2.0f);
	transform.SetIdentity();
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();
	m_axises[2].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);
	m_axises[2].lineModel->SetMaterial(m_material);
	m_axises[2].params = sh::video::GpuParams::Create(info);
	m_axises[2].params->GetParam("matWVP", m_axises[2].wvpMtrix);
	m_axises[2].params->GetParam("color", m_axises[2].color);
	m_axises[2].color.Set(sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
}

///////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{

	
}

///////////////////////////////////////////////////////////////////

void Gizmo::Render()
{
	if (!m_axises[0].lineModel || !m_axises[1].lineModel || !m_axises[2].lineModel ||
		!m_entity)
		return;

	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
	sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
	sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
	sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();

	if (m_entity)
	{
		sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>(m_entity->GetComponent(sh::Component::Type::Transform));
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
				m_axises[i].wvpMtrix.Set(wvpMatrix);
			}
		}
	}


	driver->SetRenderPipeline(m_material->GetRenderPipeline());

	for (size_t i = 0; i < 3; ++i)
	{
		driver->SetGpuParams(m_axises[i].params);

		sh::u32 meshesCount = m_axises[i].lineModel->GetMeshesCount();
		for (size_t j = 0; j < meshesCount; ++j)
		{
			const auto& mesh = m_axises[i].lineModel->GetMesh(j);
			const auto& renderable = mesh->GetRanderable();

			
			driver->SetVertexBuffer(renderable->GetVertexBuffer());
			driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration());
			driver->SetIndexBuffer(renderable->GetIndexBuffer());				
			driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
		}
	}
}

///////////////////////////////////////////////////////////////////

void Gizmo::OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code)
{
	if (code != sh::MouseCode::ButtonLeft)
		return;

	switch (type)
	{
		case sh::MouseEventType::ButtonPressed:
			OnMousePressed(x, y);
			break;
		case sh::MouseEventType::ButtonReleased:
			OnMouseReleased(x, y);
			break;
		case sh::MouseEventType::Moved:
			OnMouseMoved(x, y);
			break;
		default:
			break;
	}
}

///////////////////////////////////////////////////////////////////

void Gizmo::OnMousePressed(sh::u32 x, sh::u32 y) 
{
	if (!TryToSelect(x, y))
	{
		return;
	}
	m_mousePressed = true;
}

///////////////////////////////////////////////////////////////////

void Gizmo::OnMouseReleased(sh::u32 x, sh::u32 y) 
{
	if (!TryToSelect(x, y) && !m_mousePressed)
	{
		const auto& picker = sh::Device::GetInstance()->GetSceneManager()->GetPicker();
		auto result = picker->TryToPick(x, y, 640, 480);
		if (result != m_entity)
		{
			SetEntity(nullptr);
		}
		return;
	}
	m_mousePressed = false;
}

///////////////////////////////////////////////////////////////////

void Gizmo::SetEntity(sh::Entity* entity)
{
	m_entity = entity;
	m_enabled = false;
	if (m_entity)
	{
		m_enabled = true;
	}
}

///////////////////////////////////////////////////////////////////