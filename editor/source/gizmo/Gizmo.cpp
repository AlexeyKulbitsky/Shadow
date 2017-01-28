#include "Gizmo.h"

///////////////////////////////////////////////////////////////////

Gizmo::Gizmo()
{
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
		sh::math::Vector3f color(0.5f, 0.5f, 0.5f);
		m_axises[0].lineColorUniform->Set(color);
	}

	////////////////////////////////////////////////

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
		sh::math::Vector3f color(0.5f, 0.5f, 0.5f);
		m_axises[1].lineColorUniform->Set(color);
	}

	/////////////////////////////////////////////////

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
		sh::math::Vector3f color(0.5f, 0.5f, 0.5f);
		m_axises[2].lineColorUniform->Set(color);
	}
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
			}
		}
	}


	for (size_t i = 0; i < 3; ++i)
	{
		driver->Render(m_axises[i].lineModel.get());
	}
}

///////////////////////////////////////////////////////////////////

void Gizmo::SetEntity(sh::Entity* entity)
{
	m_entity = entity;
}

///////////////////////////////////////////////////////////////////