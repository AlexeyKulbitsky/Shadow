#include "SceneManager.h"
#include "../resources/ResourceManager.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../video/RenderPass.h"
#include "../video/ShaderProgram.h"
#include "../video/UniformBuffer.h"
#include "../video/Uniform.h"
#include "../video/Sampler.h"
#include "ModelLoader/ModelLoader.h"
#include "../video/RenderTechnique.h"
#include "../video/Material.h"
#include "../video/CommandPool.h"
#include "Model.h"
#include "ModelBase.h"
#include "Mesh.h"
#include "MeshBase.h"
#include "Camera.h"
#include <pugixml.hpp>

namespace sh
{
	namespace scene
	{
		math::Vector3f SceneManager::s_rightVector(1.0f, 0.0f, 0.0f);
		math::Vector3f SceneManager::s_upVector(0.0f, 1.0f, 0.0f);
		math::Vector3f SceneManager::s_frontVector(0.0f, 0.0f, -1.0f);

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::SceneManager()
		{
			m_commandPool = new video::CommandPool();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::~SceneManager()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::LoadScene(const char* filename)
		{
			ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();



			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filename);
			pugi::xml_node firstChild = doc.first_child();

			pugi::xml_node objectNode = firstChild.child("entity");
			while (objectNode)
			{
				// Read model
				sh::scene::Model* model = nullptr;
				sh::scene::ModelBase* modelBase = nullptr;
				pugi::xml_node modelNode = objectNode.child("model");
				if (modelNode)
				{
					pugi::xml_attribute attr = modelNode.attribute("filename");
					String modelFileName = attr.as_string();
					modelBase = resourceManager->GetModelBase(modelFileName);
					model = new Model(modelBase);
				}

				// Read name
				pugi::xml_attribute nameAttribute = objectNode.attribute("name");
				if (nameAttribute)
				{
					printf("Object %s", nameAttribute.as_string());
				}

				// Read transform
				pugi::xml_node transformNode = objectNode.child("transform");
				if (transformNode)
				{
					pugi::xml_node positionNode = transformNode.child("position");
					if (positionNode)
					{
						math::Vector3f pos(0.0f);
						pugi::xml_attribute attr = positionNode.attribute("x");
						pos.x = attr.as_float();

						attr = positionNode.attribute("y");
						pos.y = attr.as_float();

						attr = positionNode.attribute("z");
						pos.z = attr.as_float();

						model->SetPosition(pos);
					}
				}	

				// Read material
				pugi::xml_node materialNode = objectNode.child("material");
				if (materialNode)
				{
					sh::video::Material* material = new sh::video::Material();

					pugi::xml_node techniqueNode = materialNode.child("technique");
					pugi::xml_attribute fileName = techniqueNode.attribute("filename");
					String techniqueFileName = fileName.as_string();

					sh::video::RenderTechnique* rt = resourceManager->GetRenderTechnique(techniqueFileName);

					material->SetRenderTechnique(rt);

					// Read uniforms
					sh::video::UniformBuffer* uniformBuffer = material->GetRenderPass(0)->GetUniformBuffer();

					pugi::xml_node uniformsNode = materialNode.child("uniforms");
					if (uniformsNode)
					{
						pugi::xml_node uniNode = uniformsNode.first_child();
						
						while (!uniNode.empty())
						{
							pugi::xml_attribute nameAttr = uniNode.attribute("name");
							pugi::xml_attribute typeAttr = uniNode.attribute("type");
							pugi::xml_attribute valAttr = uniNode.attribute("val");

							std::string name = nameAttr.as_string();
							std::string typeName = typeAttr.as_string();

							
							sh::video::Uniform* uniform = uniformBuffer->GetUniform(name);
							if (typeName == "float")
							{
								float value = valAttr.as_float();							
								uniform->Set(value);

							}
							else if (typeName == "int")
							{
								int value = valAttr.as_int();
								uniform->Set(value);
							}
							uniNode = uniNode.next_sibling();
						}
					}

					// Read samplers
					pugi::xml_node samplersNode = materialNode.child("samplers");
					if (samplersNode)
					{
						pugi::xml_node samplNode = samplersNode.first_child();

						while (!samplNode.empty())
						{
							pugi::xml_attribute typeAttr = samplNode.attribute("type");
							String typeName = typeAttr.as_string();

							pugi::xml_attribute nameAttr = samplNode.attribute("name");
							std::string name = nameAttr.as_string();

							sh::video::Sampler* sampler = uniformBuffer->GetSampler(name);
							sh::video::Texture* texture = nullptr;
							if (typeName == "2D")
							{
								pugi::xml_attribute fileNameAttr = samplNode.attribute("filename");
								std::string fileName = fileNameAttr.as_string();
								texture = resourceManager->GetTexture(fileName);
							}
							else if (typeName == "cube")
							{
								pugi::xml_attribute right = samplNode.attribute("right");
								pugi::xml_attribute left = samplNode.attribute("left");
								pugi::xml_attribute top = samplNode.attribute("top");
								pugi::xml_attribute bottom = samplNode.attribute("bottom");
								pugi::xml_attribute back = samplNode.attribute("back");
								pugi::xml_attribute front = samplNode.attribute("front");

								std::vector<String> faces;
								faces.push_back(right.as_string());
								faces.push_back(left.as_string());
								faces.push_back(top.as_string());
								faces.push_back(bottom.as_string());
								faces.push_back(back.as_string());
								faces.push_back(front.as_string());

								texture = resourceManager->GetCubeTexture(faces);
							}
													

							sampler->Set(texture);

							samplNode = samplNode.next_sibling();
						}
					}

					

					// Init model with material
					for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
					{
						sh::scene::Mesh* mesh = model->GetMesh(i);
						mesh->SetMaterial(material);
					}		

					// Reinit samplers
					{
						for (size_t i = 0; i < modelBase->GetMeshesCount(); ++i)
						{
							MeshBase* meshBase = modelBase->GetMesh(i);
							Mesh* mesh = model->GetMesh(i);
							video::UniformBuffer* meshUniformBuffer = mesh->GetRenderCommand()->GetUniformBuffer();

							for (size_t j = 0; j < meshBase->GetSamplersCount(); ++j)
							{
								video::Sampler* baseSampler = meshBase->GetSampler(j);
								video::Sampler* sampler = meshUniformBuffer->GetSampler(baseSampler->GetUsage());
								if (sampler)
								{
									sh::video::Texture* texture = resourceManager->GetTexture(baseSampler->GetTextureName());
									sampler->Set(texture);
								}
							}
						}
					}
				}
				// Add model to render list
				m_models.push_back(model);

				for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
				{
					m_commandPool->AddMesh(model->GetMesh(i));
				}

				// Read next object
				objectNode = objectNode.next_sibling();
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SetCamera(Camera* camera)
		{
			m_camera = camera;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::Render()
		{
			for (size_t i = 0, sz = m_models.size(); i < sz; ++i)
			{
				m_models[i]->Render();
			}

			m_commandPool->Submit();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::Update()
		{
			m_camera->Update();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::OnEvent(const Event& e)
		{
			switch (e.type)
			{
			case EventType::KEYBOARD_INPUT_EVENT:
				break;
			case EventType::MOUSE_INPUT_EVENT:
				break;
			default:
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	}
}