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
		/*
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
		*/
	}

	void GuiManager::Update(u32 delta)
	{

	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();

		driver->SetRenderPipeline(m_mainBatch.material->GetRenderPipeline());
		driver->SetTopology(Topology::TOP_TRIANGLE_LIST);
		driver->SetVertexBuffer(m_mainBatch.vertexBuffer);
		driver->SetVertexDeclaration(m_mainBatch.inputDeclaration);
		driver->SetIndexBuffer(m_mainBatch.indexBuffer);				
		driver->DrawIndexed(0, m_mainBatch.indexBuffer->GetIndicesCount());
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