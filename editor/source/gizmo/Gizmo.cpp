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
#include <Device.h>

///////////////////////////////////////////////////////////////////

Gizmo::Gizmo()
{
	m_lineModel = sh::scene::GeometryGenerator::GetLineModel();
	
}

///////////////////////////////////////////////////////////////////

Gizmo::~Gizmo()
{
	if (m_lineModel)
		delete m_lineModel;
}

///////////////////////////////////////////////////////////////////

void Gizmo::Render()
{
	if (!m_lineModel)
		return;

	size_t meshesCount = m_lineModel->GetMeshesCount();
	for (size_t i = 0; i < meshesCount; ++i)
	{
		sh::scene::Mesh* mesh = m_lineModel->GetMesh(i);

		sh::video::Material* material = mesh->GetMaterial();
		sh::video::RenderPass* renderPass = material->GetRenderPass(0);

		sh::video::RenderState* renderState = material->GetRenderTechnique()->GetRenderPass(0)->GetRenderState();
		renderState->Apply();
		renderPass->GetShaderProgram()->BindProgram();

		sh::math::Matrix4f mat;
		mat.SetIdentity();
		mat.SetTranslation(sh::math::Vector3f(0.0f, 0.0f, -3.0f));
		mesh->SetWorldMatrix(mat);
		mesh->Render();
		
		sh::video::RenderCommand* renderCommand = mesh->GetRenderCommand();
		sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

		driver->Render(renderCommand);
		renderPass->GetShaderProgram()->UnbindProgram();
		
	}
}

///////////////////////////////////////////////////////////////////