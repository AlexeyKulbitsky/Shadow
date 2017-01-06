#include "Gizmo.h"
#include <scene\GeometryGenerator.h>
#include <scene\Mesh.h>
#include <scene\Model.h>

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
	m_lineModelX = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f), sh::math::Vector3f(5.0f, 0.0f, 0.0f));	
	sh::video::UniformBuffer* uniformBuffer = m_lineModelX->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	sh::video::Uniform* colorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (colorUniform)
	{
		sh::math::Vector3f color(1.0f, 0.0f, 0.0f);
		colorUniform->Set(color);
	}

	m_coneModelX = sh::scene::GeometryGenerator::GetConeModel(
		sh::math::Vector3f(5.0f, 0.0f, 0.0f), 
		sh::math::Vector3f(1.0f, 0.0f, 0.0f),
		sh::math::Vector3f(0.0f, 1.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));	
	uniformBuffer = m_coneModelX->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	colorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (colorUniform)
	{
		sh::math::Vector3f color(1.0f, 0.0f, 0.0f);
		colorUniform->Set(color);
	}

	////////////////////////////////////////////////

	m_lineModelY = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f), sh::math::Vector3f(0.0f, 5.0f, 0.0f));	
	uniformBuffer = m_lineModelY->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	colorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (colorUniform)
	{
		sh::math::Vector3f color(0.0f, 1.0f, 0.0f);
		colorUniform->Set(color);
	}

	m_coneModelY = sh::scene::GeometryGenerator::GetConeModel(
		sh::math::Vector3f(0.0f, 5.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 1.0f, 0.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 0.0f, 1.0f));	
	uniformBuffer = m_coneModelY->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	colorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (colorUniform)
	{
		sh::math::Vector3f color(0.0f, 1.0f, 0.0f);
		colorUniform->Set(color);
	}

	/////////////////////////////////////////////////

	m_lineModelZ = sh::scene::GeometryGenerator::GetLineModel(sh::math::Vector3f(0.0f), sh::math::Vector3f(0.0f, 0.0f, 5.0f));	
	uniformBuffer = m_lineModelZ->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	colorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (colorUniform)
	{
		sh::math::Vector3f color(0.0f, 0.0f, 1.0f);
		colorUniform->Set(color);
	}

	m_coneModelZ = sh::scene::GeometryGenerator::GetConeModel(
		sh::math::Vector3f(0.0f, 0.0f, 5.0f), 
		sh::math::Vector3f(0.0f, 0.0f, 1.0f),
		sh::math::Vector3f(1.0f, 0.0f, 0.0f), 
		sh::math::Vector3f(0.0f, 1.0f, 0.0f));	
	uniformBuffer = m_coneModelZ->GetMesh(0)->GetRenderCommand()->GetUniformBuffer();
	colorUniform = uniformBuffer->GetUniform(sh::String("color"));
	if (colorUniform)
	{
		sh::math::Vector3f color(0.0f, 0.0f, 1.0f);
		colorUniform->Set(color);
	}
}

///////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{
	if (m_lineModelX)
		delete m_lineModelX;

	if (m_lineModelY)
		delete m_lineModelY;

	if (m_lineModelZ)
		delete m_lineModelZ;

	if (m_coneModelX)
		delete m_coneModelX;

	if (m_coneModelY)
		delete m_coneModelY;

	if (m_coneModelZ)
		delete m_coneModelZ;
	
}

///////////////////////////////////////////////////////////////////

void Gizmo::Render()
{
	if (!m_lineModelX || !m_lineModelY || !m_lineModelZ || 
		!m_coneModelX || !m_coneModelY || !m_coneModelZ)
		return;

	sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

	if (m_entity)
	{
		sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>( m_entity->GetComponent(sh::Component::Type::TRANSFORM) );
		if (transformComponent)
		{
			sh::math::Matrix4f matrix = transformComponent->GetWorldMatrix();
			m_lineModelX->SetWorldMatrix(matrix);
			m_lineModelX->UpdateTransformationUniforms();
			m_coneModelX->SetWorldMatrix(matrix);
			m_coneModelX->UpdateTransformationUniforms();

			m_lineModelY->SetWorldMatrix(matrix);
			m_lineModelY->UpdateTransformationUniforms();
			m_coneModelY->SetWorldMatrix(matrix);
			m_coneModelY->UpdateTransformationUniforms();

			m_lineModelZ->SetWorldMatrix(matrix);
			m_lineModelZ->UpdateTransformationUniforms();	
			m_coneModelZ->SetWorldMatrix(matrix);
			m_coneModelZ->UpdateTransformationUniforms();
		}			
	}

	driver->Render(m_lineModelX);
	driver->Render(m_coneModelX);

	driver->Render(m_lineModelY);
	driver->Render(m_coneModelY);

	driver->Render(m_lineModelZ);
	driver->Render(m_coneModelZ);
}

///////////////////////////////////////////////////////////////////