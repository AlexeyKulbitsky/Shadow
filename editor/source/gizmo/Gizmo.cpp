#include "Gizmo.h"

///////////////////////////////////////////////////////////////////

sh::math::Vector3 Gizmo::s_position;
sh::math::Quaternion Gizmo::s_rotation;
sh::math::Vector3 Gizmo::s_scale;

Gizmo::Gizmo()
{
	m_material.reset(new sh::video::Material());
	m_material->SetRenderTechnique("ConstantColor3D.rt");
	const auto& info = m_material->GetRenderPipeline()->GetParamsInfo();

	float radius = 0.1f;
	float height = 3.0f;
	sh::u32 numberOfSides = 100U;

	sh::math::Vector3 translation(0.0f);
	sh::math::Quaternion rotation;
	sh::math::Matrix4 transform;
	transform.SetIdentity();

	rotation.SetFromAxisAngle(sh::scene::SceneManager::GetFrontVector(), sh::math::k_pi_2);
	translation = sh::math::Vector3(height / 2.0f, 0.0f, 0.0f);
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();

	m_axises[0].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);
	m_axises[0].lineModel->SetMaterial(m_material);
	m_axises[0].params = sh::video::GpuParams::Create(info);
	m_axises[0].params->GetParam("matrixWVP", m_axises[0].wvpMtrix);
	m_axises[0].params->GetParam("color", m_axises[0].color);
	m_axises[0].color.Set(sh::math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	////////////////////////////////////////////////

	translation = sh::math::Vector3(0.0f, height / 2.0f, 0.0f);
	transform.SetIdentity();
	transform.SetTranslation(translation);
	m_axises[1].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);
	m_axises[1].lineModel->SetMaterial(m_material);
	m_axises[1].params = sh::video::GpuParams::Create(info);
	m_axises[1].params->GetParam("matrixWVP", m_axises[1].wvpMtrix);
	m_axises[1].params->GetParam("color", m_axises[1].color);
	m_axises[1].color.Set(sh::math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	/////////////////////////////////////////////////

	rotation.SetFromAxisAngle(sh::scene::SceneManager::GetRightVector(), sh::math::k_pi_2);
	translation = sh::math::Vector3(0.0f, 0.0f, height / 2.0f);
	transform.SetIdentity();
	transform.SetTranslation(translation);
	transform = transform * rotation.GetAsMatrix4();
	m_axises[2].lineModel = sh::scene::GeometryGenerator::GetCylinderModel(height, radius, numberOfSides, transform);
	m_axises[2].lineModel->SetMaterial(m_material);
	m_axises[2].params = sh::video::GpuParams::Create(info);
	m_axises[2].params->GetParam("matrixWVP", m_axises[2].wvpMtrix);
	m_axises[2].params->GetParam("color", m_axises[2].color);
	m_axises[2].color.Set(sh::math::Vector4(0.0f, 0.0f, 1.0f, 1.0f));
}

///////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{

	
}

///////////////////////////////////////////////////////////////////

void Gizmo::Render()
{
	if (!m_enabled || !m_axises[0].lineModel || !m_axises[1].lineModel || !m_axises[2].lineModel)
		return;

	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
	const sh::scene::CameraPtr& camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
	sh::math::Matrix4 viewMatrix = camera->GetViewMatrix();
	sh::math::Matrix4 projectionMatrix = camera->GetProjectionMatrix();

	sh::math::Matrix4 matrix;
	matrix.SetIdentity();

	sh::math::Vector3 position = s_position;
	sh::math::Quaternion rotation = s_rotation;
	sh::f32 scaleFactor = (camera->GetPosition() - position).GetLength() / 35.0f;
	sh::math::Vector3 scale(scaleFactor);

	matrix.SetScale(scale);
	matrix.SetTranslation(position);
	matrix = matrix * rotation.GetAsMatrix4();

	sh::math::Matrix4 wvpMatrix = projectionMatrix * viewMatrix * matrix;
	wvpMatrix.Transpose();

	for (size_t i = 0; i < Axis::COUNT; ++i)
	{
		m_axises[i].wvpMtrix.Set(wvpMatrix);
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

bool Gizmo::OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code)
{
	if (code != sh::MouseCode::ButtonLeft)
		return false;

	switch (type)
	{
		case sh::MouseEventType::ButtonPressed:
			return OnMousePressed(x, y);
			break;
		case sh::MouseEventType::ButtonReleased:
			return OnMouseReleased(x, y);
			break;
		case sh::MouseEventType::Moved:
			return OnMouseMoved(x, y);
			break;
		default:
			break;
	}
	return false;
}

///////////////////////////////////////////////////////////////////

bool Gizmo::OnMousePressed(sh::u32 x, sh::u32 y)
{
	if (!TryToSelect(x, y))
	{
		return false;
	}
	m_mousePressed = true;
	return true;
}

///////////////////////////////////////////////////////////////////

bool Gizmo::OnMouseReleased(sh::u32 x, sh::u32 y)
{
	if (!TryToSelect(x, y) && !m_mousePressed)
	{
		return false;
	}
	m_mousePressed = false;
	return true;
}

///////////////////////////////////////////////////////////////////
