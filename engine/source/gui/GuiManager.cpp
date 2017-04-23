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
		video::VertexBufferDecription desc;
		desc.usage = USAGE_DYNAMIC;
		m_vertexBuffer = video::VertexBuffer::Create(desc);

		video::IndexBufferDescription indexDesc;
		indexDesc.indexType = IT_32_BIT;
		indexDesc.usage = USAGE_DYNAMIC;
		m_indexBuffer = video::IndexBuffer::Create(indexDesc);

		m_material.reset(new video::Material());
		m_material->SetRenderTechnique("ui_base.xml");


		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
		vertexDeclaration->AddAttribute(positionAttribute);	
		vertexDeclaration->AddAttribute(uvAttribute);	

		m_vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());
		m_vertexBuffer->SetVertexDeclaration(vertexDeclaration);

		m_inputDeclaration = m_material->GetRenderPipeline()->GetVertexInputDeclaration()->Clone();
		m_inputDeclaration->Assemble(*(m_vertexBuffer->GetVertexDeclaration().get()));

		math::Rectf rect(0.0f, 0.0f, 40.0f, 15.0f);

		Button* button = new Button(rect);
		SPtr<GuiElement> child(button);
		m_children.push_back(child);
	}

	void GuiManager::Update(u32 delta)
	{

	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();

		driver->SetRenderPipeline(m_material->GetRenderPipeline());
		driver->SetTopology(Topology::TOP_TRIANGLE_LIST);
		driver->SetVertexBuffer(m_vertexBuffer);
		driver->SetVertexDeclaration(m_inputDeclaration);
		driver->SetIndexBuffer(m_indexBuffer);				
		driver->DrawIndexed(0, m_indexBuffer->GetIndicesCount());
	}


	void GuiManager::OnMouseEvent(int x, int y, MouseEventType type, MouseCode code)
	{
		if (type != MouseEventType::ButtonPressed)
			return;

		for (auto& child : m_children)
		{
			child->ProcessInput(x, y);
		}
	}

	void GuiManager::OnKeyboardEvent(KeyboardEventType type, KeyCode code)
	{

	}

} // gui

} // sh