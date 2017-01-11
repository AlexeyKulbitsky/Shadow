#include "Gizmo.h"
#include <scene\GeometryGenerator.h>
#include <scene\Mesh.h>
#include <scene\Model.h>
#include <scene\SceneManager.h>
#include <scene\Camera.h>

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

}

///////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{

	
}

///////////////////////////////////////////////////////////////////

void Gizmo::Render()
{
	
}

///////////////////////////////////////////////////////////////////

void Gizmo::SetEntity(sh::Entity* entity)
{
	m_entity = entity;
}

///////////////////////////////////////////////////////////////////