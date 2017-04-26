#include "GuiManager.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../video/Material.h"
#include "../video/VertexDeclaration.h"
#include "../video/RenderPipeline.h"

#include "Button.h"

#include "../video/Driver.h"
#include "../Device.h"



using namespace std::placeholders;

namespace sh
{

namespace gui
{

	GuiManager::GuiManager()
	{
		Device::GetInstance()->mouseEvent.Connect(std::bind(&GuiManager::OnMouseEvent, this, _1, _2, _3, _4));
		Device::GetInstance()->keyboardEvent.Connect(std::bind(&GuiManager::OnKeyboardEvent, this, _1, _2));
	}

	GuiManager::~GuiManager()
	{
	}

	void GuiManager::Init()
	{
		///*
		video::VertexBufferDecription desc;
		desc.usage = USAGE_DYNAMIC;
		m_mainBatch.vertexBuffer = video::VertexBuffer::Create(desc);

		video::IndexBufferDescription indexDesc;
		indexDesc.indexType = IT_32_BIT;
		indexDesc.usage = USAGE_DYNAMIC;
		m_mainBatch.indexBuffer = video::IndexBuffer::Create(indexDesc);

		m_mainBatch.material.reset(new video::Material());
		m_mainBatch.material->SetRenderTechnique("ui_base.xml");


		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
		sh::video::Attribute colorAttribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 3U);
		vertexDeclaration->AddAttribute(positionAttribute);	
		vertexDeclaration->AddAttribute(uvAttribute);
		vertexDeclaration->AddAttribute(colorAttribute);

		m_mainBatch.vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());
		m_mainBatch.vertexBuffer->SetVertexDeclaration(vertexDeclaration);

		m_mainBatch.inputDeclaration = m_mainBatch.material->GetRenderPipeline()->GetVertexInputDeclaration()->Clone();
		m_mainBatch.inputDeclaration->Assemble(*(m_mainBatch.vertexBuffer->GetVertexDeclaration().get()));
		//*/
	}

	void GuiManager::Update(u32 delta)
	{

	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();

		driver->SetRenderPipeline(m_mainBatch.material->GetRenderPipeline());
		driver->SetGpuParams(m_mainBatch.material->GetCommonGpuParams());
		driver->SetTopology(Topology::TOP_TRIANGLE_LIST);
		driver->SetVertexBuffer(m_mainBatch.vertexBuffer);
		driver->SetVertexDeclaration(m_mainBatch.inputDeclaration);
		driver->SetIndexBuffer(m_mainBatch.indexBuffer);				
		driver->DrawIndexed(0, m_mainBatch.indexBuffer->GetIndicesCount());
	}

	void GuiManager::LoadGui(const char* filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename);
			
		
		pugi::xml_node child = doc.first_child();
		sh::String texFilename = child.attribute("val").as_string();
		sh::video::TexturePtr texture = sh::Device::GetInstance()->GetResourceManager()->GetTexture(texFilename);
		SH_ASSERT(!!texture, "Can not load texture!");

		m_mainBatch.material->GetCommonGpuParams()->SetSampler(ST_FRAGMENT, "diffuse", texture);

		child = child.next_sibling();

		while (child)
		{
			// Read buttons
			if (child.name() == sh::String("button"))
			{
				ButtonPtr button(new Button());
				button->Load(child);

				m_children.push_back(button);
			}

			child = child.next_sibling();
		}
	}

	void GuiManager::AddChild(const SPtr<GuiElement>& child)
	{
		m_children.push_back(child);
	}


	void GuiManager::OnMouseEvent(int x, int y, MouseEventType type, MouseCode code)
	{
		for (auto& child : m_children)
		{
			child->ProcessInput(x, y, type);
		}
	}

	void GuiManager::OnKeyboardEvent(KeyboardEventType type, KeyCode code)
	{

	}

} // gui

} // sh