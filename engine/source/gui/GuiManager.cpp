#include "GuiManager.h"

#include "../font/Font.h"

#include "Button.h"
#include "SpriteManager.h"

#include "../video/Driver.h"
#include "../video/Vulkan/VulkanDriver.h"
#include "../video/Vulkan/VulkanCommandBuffer.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../video/Material.h"
#include "../video/VertexDeclaration.h"
#include "../video/RenderPipeline.h"

#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../Device.h"


using namespace std::placeholders;

namespace sh
{

namespace gui
{

	GuiManager::GuiManager()
	{
		
	}

	GuiManager::~GuiManager()
	{
	}

	void GuiManager::Init()
	{
		InitMainBatch();
		InitTextBatch();
	}

	void GuiManager::Update(u32 delta)
	{

	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		
		m_mainBatchData.vertices.clear();
		m_mainBatchData.indices.clear();
		m_mainBatchData.verticesCount = 0U;
		for (u32 i = 0U; i < m_children.size(); ++i)
		{
			m_children[i]->GetGeometry(m_mainBatchData);
		}

		const void* verticesPointer = m_mainBatchData.vertices.data();
		size_t verticesDataSize = m_mainBatchData.vertices.size() * sizeof(float);
		m_mainBatch.vertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
		m_mainBatch.vertexBuffer->SetVerticesCount(m_mainBatchData.verticesCount);

		const void* indicesPointer = m_mainBatchData.indices.data();
		size_t indicesDataSize = m_mainBatchData.indices.size() * sizeof(u32);
		m_mainBatch.indexBuffer->SetData(0U, indicesDataSize, indicesPointer);
		m_mainBatch.indexBuffer->SetIndicesCount(m_mainBatchData.indices.size());

		/*
		if( driver->GetApiName() == "Vulkan" )
		{
			video::VulkanDriver* vulkanDriver = static_cast<video::VulkanDriver*>(driver);

			m_mainBatch.commandBuffer->Begin();

			driver->SetVertexDeclaration(m_mainBatch.inputDeclaration, m_mainBatch.commandBuffer);
			driver->SetRenderPipeline(m_mainBatch.material->GetRenderPipeline(), m_mainBatch.commandBuffer);
			driver->SetGpuParams(m_mainBatch.material->GetCommonGpuParams(), m_mainBatch.commandBuffer);
			driver->SetTopology(Topology::TOP_TRIANGLE_LIST, m_mainBatch.commandBuffer);
			driver->SetVertexBuffer(m_mainBatch.vertexBuffer, m_mainBatch.commandBuffer);
			driver->SetIndexBuffer(m_mainBatch.indexBuffer, m_mainBatch.commandBuffer);				
			driver->DrawIndexed(0, m_mainBatch.indexBuffer->GetIndicesCount(), 1U, m_mainBatch.commandBuffer);

			m_mainBatch.commandBuffer->End();

			video::VulkanCommandBuffer* cmdBuf = static_cast<video::VulkanCommandBuffer*>(m_mainBatch.commandBuffer.get());
			vulkanDriver->GetPrimaryCommandBuffer()->Append(cmdBuf);
		}
		else
			*/
		{
			m_mainBatch.commandBuffer->Begin();

			driver->SetRenderPipeline(m_mainBatch.material->GetRenderPipeline(), m_mainBatch.commandBuffer);
			driver->SetGpuParams(m_mainBatch.material->GetCommonGpuParams(), m_mainBatch.commandBuffer);
			driver->SetTopology(Topology::TOP_TRIANGLE_LIST, m_mainBatch.commandBuffer);
			driver->SetVertexBuffer(m_mainBatch.vertexBuffer, m_mainBatch.commandBuffer);
			driver->SetVertexDeclaration(m_mainBatch.inputDeclaration, m_mainBatch.commandBuffer);
			driver->SetIndexBuffer(m_mainBatch.indexBuffer, m_mainBatch.commandBuffer);				
			driver->DrawIndexed(0, m_mainBatch.indexBuffer->GetIndicesCount(), 1U, m_mainBatch.commandBuffer);

			m_mainBatch.commandBuffer->End();

			driver->SubmitCommandBuffer(m_mainBatch.commandBuffer);
		}


		////////////////////////////////////////////////////////////////////////////////////////////

		m_textBatchData.vertices.clear();
		m_textBatchData.indices.clear();
		m_textBatchData.verticesCount = 0U;
		for (u32 i = 0U; i < m_children.size(); ++i)
		{
			m_children[i]->GetTextGeometry(m_textBatchData);
		}

		verticesPointer = m_textBatchData.vertices.data();
		verticesDataSize = m_textBatchData.vertices.size() * sizeof(float);
		m_textBatch.vertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
		m_textBatch.vertexBuffer->SetVerticesCount(m_textBatchData.verticesCount);

		indicesPointer = m_textBatchData.indices.data();
		indicesDataSize = m_textBatchData.indices.size() * sizeof(u32);
		m_textBatch.indexBuffer->SetData(0U, indicesDataSize, indicesPointer);
		m_textBatch.indexBuffer->SetIndicesCount(m_textBatchData.indices.size());

		/*
		if( driver->GetApiName() == "Vulkan" )
		{
			video::VulkanDriver* vulkanDriver = static_cast<video::VulkanDriver*>(driver);

			m_textBatch.commandBuffer->Begin();

			driver->SetVertexDeclaration(m_textBatch.inputDeclaration, m_textBatch.commandBuffer);
			driver->SetRenderPipeline(m_textBatch.material->GetRenderPipeline(), m_textBatch.commandBuffer);
			driver->SetGpuParams(m_textBatch.material->GetCommonGpuParams(), m_textBatch.commandBuffer);
			driver->SetTopology(Topology::TOP_TRIANGLE_LIST, m_textBatch.commandBuffer);
			driver->SetVertexBuffer(m_textBatch.vertexBuffer, m_textBatch.commandBuffer);
			driver->SetIndexBuffer(m_textBatch.indexBuffer, m_textBatch.commandBuffer);				
			driver->DrawIndexed(0, m_textBatch.indexBuffer->GetIndicesCount(), 1U, m_textBatch.commandBuffer);

			m_textBatch.commandBuffer->End();

			video::VulkanCommandBuffer* cmdBuf = static_cast<video::VulkanCommandBuffer*>(m_textBatch.commandBuffer.get());
			vulkanDriver->GetPrimaryCommandBuffer()->Append(cmdBuf);
		}
		else
			*/
		{
			m_textBatch.commandBuffer->Begin();

			driver->SetRenderPipeline(m_textBatch.material->GetRenderPipeline(), m_textBatch.commandBuffer);
			driver->SetGpuParams(m_textBatch.material->GetCommonGpuParams(), m_textBatch.commandBuffer);
			driver->SetTopology(Topology::TOP_TRIANGLE_LIST, m_textBatch.commandBuffer);
			driver->SetVertexBuffer(m_textBatch.vertexBuffer, m_textBatch.commandBuffer);
			driver->SetVertexDeclaration(m_textBatch.inputDeclaration, m_textBatch.commandBuffer);
			driver->SetIndexBuffer(m_textBatch.indexBuffer, m_textBatch.commandBuffer);				
			driver->DrawIndexed(0, m_textBatch.indexBuffer->GetIndicesCount(), 1U, m_textBatch.commandBuffer);

			m_textBatch.commandBuffer->End();

			driver->SubmitCommandBuffer(m_textBatch.commandBuffer);
		}
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

		child = doc.child("sprites");
		SpriteManager::GetInstance()->Load(child);

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

	void GuiManager::SetFont(const FontPtr& font)
	{
		m_font = font;

		const auto& texture = m_font->GetTextureAtlas();
		SH_ASSERT(!!texture, "Can not set empty font to Gui manager!");

		m_textBatch.material->GetCommonGpuParams()->SetSampler(ST_FRAGMENT, "fontAtlas", texture);
	}

	bool GuiManager::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		for (auto& child : m_children)
		{
			if (child->ProcessInput(x, y, type))
				return true;
		}
		return false;
	}

	bool GuiManager::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		for (auto& child : m_children)
		{
			if (child->ProcessKeyboardInput(type, code))
				return true;
		}
		return false;
	}

	void GuiManager::InitMainBatch()
	{
		video::VertexBufferDecription desc;
		desc.usage = USAGE_DYNAMIC;
		m_mainBatch.vertexBuffer = video::VertexBuffer::Create(desc);

		video::IndexBufferDescription indexDesc;
		indexDesc.indexType = IT_32_BIT;
		indexDesc.usage = USAGE_DYNAMIC;
		m_mainBatch.indexBuffer = video::IndexBuffer::Create(indexDesc);

		m_mainBatch.material.reset(new video::Material());
		m_mainBatch.material->SetRenderTechnique("ui_base.xml");
		m_mainBatch.material->GetCommonGpuParams()->GetParam("orthoMat", m_mainBatch.orthoMatrix);
		const auto& mat = sh::Device::GetInstance()->GetSceneManager()->GetCamera()->Get2DProjectionMatrix();
		m_mainBatch.orthoMatrix.Set(mat);

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

		m_mainBatch.material->GetRenderPipeline()->Init(m_mainBatch.inputDeclaration);

		video::CommandBufferDescription commandBufferDesc;
		commandBufferDesc.type = COMMAND_BUFFER_TYPE_SECONDARY;
		m_mainBatch.commandBuffer = video::CommandBuffer::Create(commandBufferDesc);

		u32 rectSizeForVertices = vertexDeclaration->GetStride() / sizeof(float);
		m_mainBatchData.vertices.reserve(rectSizeForVertices * 1000U);
		m_mainBatchData.indices.reserve(6 * 1000U);
	}

	void GuiManager::InitTextBatch()
	{
		video::VertexBufferDecription desc;
		desc.usage = USAGE_DYNAMIC;
		m_textBatch.vertexBuffer = video::VertexBuffer::Create(desc);

		video::IndexBufferDescription indexDesc;
		indexDesc.indexType = IT_32_BIT;
		indexDesc.usage = USAGE_DYNAMIC;
		m_textBatch.indexBuffer = video::IndexBuffer::Create(indexDesc);

		m_textBatch.material.reset(new video::Material());
		m_textBatch.material->SetRenderTechnique("text_base.xml");
		m_textBatch.material->GetCommonGpuParams()->GetParam("orthoMat", m_textBatch.orthoMatrix);
		const auto& mat = sh::Device::GetInstance()->GetSceneManager()->GetCamera()->Get2DProjectionMatrix();
		m_textBatch.orthoMatrix.Set(mat);

		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
		vertexDeclaration->AddAttribute(positionAttribute);	
		vertexDeclaration->AddAttribute(uvAttribute);

		m_textBatch.vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());
		m_textBatch.vertexBuffer->SetVertexDeclaration(vertexDeclaration);

		m_textBatch.inputDeclaration = m_textBatch.material->GetRenderPipeline()->GetVertexInputDeclaration()->Clone();
		m_textBatch.inputDeclaration->Assemble(*(m_textBatch.vertexBuffer->GetVertexDeclaration().get()));

		m_textBatch.material->GetRenderPipeline()->Init(m_textBatch.inputDeclaration);

		video::CommandBufferDescription commandBufferDesc;
		commandBufferDesc.type = COMMAND_BUFFER_TYPE_SECONDARY;
		m_textBatch.commandBuffer = video::CommandBuffer::Create(commandBufferDesc);

		u32 rectSizeForVertices = vertexDeclaration->GetStride() / sizeof(float);
		m_textBatchData.vertices.reserve(rectSizeForVertices * 1000U);
		m_textBatchData.indices.reserve(6 * 1000U);
	}

} // gui

} // sh