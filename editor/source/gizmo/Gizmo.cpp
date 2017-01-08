#include "Gizmo.h"
#include <scene\GeometryGenerator.h>
#include <scene\Mesh.h>
#include <scene\Model.h>
#include <scene\SceneManager.h>

#include <video\Material.h>
#include <video\RenderCommand.h>
#include <video\RenderPass.h>
#include <video\Driver.h>
#include <video\RenderState.h>
#include <video\ShaderProgram.h>
#include <video\RenderTechnique.h>
#include <video\UniformBuffer.h>
#include <video\Uniform.h>
#include <Device.h>

#include <entity\Entity.h>
#include <entity\components\TransformComponent.h>

///////////////////////////////////////////////////////////////////

Gizmo::Gizmo()
{
	
	m_axises[0].lineModel = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f), sh::math::Vector3f(5.0f, 0.0f, 0.0f));	
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

	m_axises[1].lineModel = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f), sh::math::Vector3f(0.0f, 5.0f, 0.0f));	
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

	m_axises[2].lineModel = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f), sh::math::Vector3f(0.0f, 0.0f, 5.0f));	
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

///////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{
	for (size_t i = 0; i < 3; ++i)
	{
		delete m_axises[i].lineModel;
		delete m_axises[i].coneModel;
	}
	
}

///////////////////////////////////////////////////////////////////

void Gizmo::Render()
{
	if (!m_axises[0].lineModel || !m_axises[1].lineModel || !m_axises[2].lineModel || 
		!m_axises[0].coneModel || !m_axises[1].coneModel || !m_axises[2].coneModel ||
		!m_entity)
		return;

	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

	if (m_entity)
	{
		sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>( m_entity->GetComponent(sh::Component::Type::TRANSFORM) );
		if (transformComponent)
		{
			sh::math::Matrix4f matrix = transformComponent->GetWorldMatrix();

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
		driver->Render(m_axises[i].lineModel);
		driver->Render(m_axises[i].coneModel);
	}
}

///////////////////////////////////////////////////////////////////

void Gizmo::Move(Axis::Type axis)
{
	if (!m_entity || !m_enabled)
		return;

	sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>( m_entity->GetComponent(sh::Component::Type::TRANSFORM) );
	sh::math::Quaternionf rotation = transformComponent->GetRotation();
	sh::math::Vector3f axisRotations(0.0f);
	rotation.GetAsEulerXYZ(axisRotations);
	axisRotations.z *= -1.0f;
	axisRotations.x *= -1.0f;
	rotation.SetFromEulerXYZ(axisRotations);

	sh::math::Vector3f axisDir(0.0f);

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
	
	sh::math::Vector3f pos = transformComponent->GetPosition();
	transformComponent->SetPosition(pos + direction);
}

///////////////////////////////////////////////////////////////////