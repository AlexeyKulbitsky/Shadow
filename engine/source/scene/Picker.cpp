#include "Picker.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../video/UniformBuffer.h"
#include "../video/Material.h"
#include "../video/RenderPipeline.h"
#include "../video/ShaderProgram.h"
#include "../scene/Mesh.h"
#include "../scene/Model.h"
#include "../entity/Entity.h"
#include "../entity/Component.h"
#include "../entity/components/RenderComponent.h"

namespace sh
{
	namespace scene
	{
		Picker::Picker()
		{	
			m_renderTechnique = sh::Device::GetInstance()->GetResourceManager()->GetRenderTechnique("base.xml");		
		}

		/////////////////////////////////////////////////////////

		Picker::~Picker()
		{
		}

		/////////////////////////////////////////////////////////

		void Picker::RegisterEntity(sh::Entity* entity)
		{
			if (!entity->GetComponent(sh::Component::Type::RENDER))
				return;

			m_entities.push_back(entity);

		}

		/////////////////////////////////////////////////////////

		sh::Entity* Picker::TryToPick(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

			driver->BeginRendering();

			for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
			{
				int r = (i & 0x000000FF) >>  0;
				int g = (i & 0x0000FF00) >>  8;
				int b = (i & 0x00FF0000) >> 16;
				sh::math::Vector4f color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

				sh::RenderComponent* renderComponent = static_cast<sh::RenderComponent*>(m_entities[i]->GetComponent(sh::Component::Type::RENDER));		
				sh::scene::Model* model = renderComponent->GetModel();

				size_t meshesCount = model->GetMeshesCount();
				for (size_t j = 0; j < meshesCount; ++j)
				{
					sh::scene::MeshPtr mesh = model->GetMesh(j);
					mesh->GetMaterial()->GetRenderPipeline(1)->GetShaderProgram()->BindProgram();
					//sh::video::Uniform* uniform = mesh->GetRenderCommand(1)->GetUniformBuffer()->GetUniform("color");
					const sh::video::UniformBufferPtr& uniformBuffer = mesh->GetMaterial()->GetRenderPipeline(1)->GetUniformBuffer();
					sh::video::Uniform* uniform = uniformBuffer->GetUniform("color");
					if (uniform)
					{
						uniform->Set(color);
					}
					uniformBuffer->Upload();
					driver->Render(mesh->GetRenderCommand(1));
					mesh->GetMaterial()->GetRenderPipeline(1)->GetShaderProgram()->UnbindProgram();
				}			
			}
		
			unsigned char data[4];
			driver->GetPixelData(x, y, width, height, data);

			int pickedID = data[0] + data[1] * 256 + data[2] * 256*256;
			if (pickedID >= 0 && (size_t)pickedID < m_entities.size())
			{
				return m_entities[pickedID];
			}

			return nullptr;
		}

		/////////////////////////////////////////////////////////

		void Picker::Clear()
		{
			m_entities.clear();
		}

		/////////////////////////////////////////////////////////
	}
}

