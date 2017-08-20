#include "GuiManager.h"

#include "../font/Font.h"

#include "Button.h"
#include "SpriteManager.h"
#include "Style.h"
#include "MenuBar.h"
#include "ToolBar.h"

#include "../video/Driver.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../video/Material.h"
#include "../video/VertexDeclaration.h"
#include "../video/RenderPipeline.h"

#include "../video/TextureLoader/TextureLoader.h"

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
		//InitMainBatch();
		//InitTextBatch();
		
		const auto& mat = sh::Device::GetInstance()->GetSceneManager()->GetCamera()->Get2DProjectionMatrix();

		m_defaultMaterial.reset(new video::Material());
		m_defaultMaterial->SetRenderTechnique("ui_base.xml");
		m_defaultMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);

		m_textMaterial.reset(new video::Material());
		m_textMaterial->SetRenderTechnique("text_base.xml");
		m_textMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);
	}

	void GuiManager::Update(u32 delta)
	{

	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		auto painter = driver->GetPainter();

		for (u32 i = 0U; i < m_children.size(); ++i)
		{
			m_children[i]->Render(painter);
		}

		if (m_toolBar)
			m_toolBar->Render(painter);

		if (m_menuBar)
			m_menuBar->Render(painter);

		if (m_focusWidget)
		{
			m_focusWidget->SetFocus(false);
			m_focusWidget->Render(painter);
			m_focusWidget->SetFocus(true);
		}

		painter->Flush();
	}

	void GuiManager::CreateMenuBar()
	{
		if (!m_menuBar)
		{
			m_menuBar.reset(new MenuBar());
		}
	}

	void GuiManager::CreateToolBar()
	{
		if (!m_toolBar)
		{
			m_toolBar.reset(new ToolBar());
		}
	}

	void GuiManager::LoadGui(const char* filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename);
			
		pugi::xml_node child = doc.first_child();
		sh::String texFilename = child.attribute("val").as_string();
		//sh::video::TexturePtr texture = sh::Device::GetInstance()->GetResourceManager()->GetTexture(texFilename);
		sh::video::TexturePtr texture = video::TextureLoader::GetInstance()->GetWhiteTexture();
		SH_ASSERT(!!texture, "Can not load texture!");

		m_defaultMaterial->GetCommonGpuParams()->SetSampler(ST_FRAGMENT, "diffuse", texture);
		//m_mainBatch.material->GetCommonGpuParams()->SetSampler(ST_FRAGMENT, "diffuse", texture);

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

	void GuiManager::AddChild(const SPtr<Widget>& child)
	{
		m_children.push_back(child);
	}

	void GuiManager::SetFont(const FontPtr& font)
	{
		m_font = font;

		const auto& texture = m_font->GetTextureAtlas();
		SH_ASSERT(!!texture, "Can not set empty font to Gui manager!");

		m_textMaterial->GetCommonGpuParams()->SetSampler(ST_FRAGMENT, "fontAtlas", texture);
		//m_textBatch.material->GetCommonGpuParams()->SetSampler(ST_FRAGMENT, "fontAtlas", texture);
	}

	void GuiManager::UpdateMatrices()
	{
		const auto& mat = sh::Device::GetInstance()->GetSceneManager()->GetCamera()->Get2DProjectionMatrix();
		//m_mainBatch.orthoMatrix.Set(mat);
		//m_textBatch.orthoMatrix.Set(mat);

		m_defaultMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);
		m_textMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);
	}

	void GuiManager::SetStyle(const StylePtr& style)
	{ 
		m_style = style; 
		m_defaultMaterial->GetCommonGpuParams()->SetSampler(
			ST_FRAGMENT, "diffuse", m_style->GetTexure());
		//m_mainBatch.material->GetCommonGpuParams()->SetSampler(
		//	ST_FRAGMENT, "diffuse", m_style->GetTexure());
	}

	bool GuiManager::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		if (m_menuBar && m_menuBar->ProcessInput(x, y, type))
			return true;

		if (m_toolBar && m_toolBar->ProcessInput(x, y, type))
			return true;

		for (auto& child : m_children)
		{
			if (child->ProcessInput(x, y, type))
				return true;
		}
		return false;
	}

	bool GuiManager::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		if (m_menuBar && m_menuBar->ProcessKeyboardInput(type, code))
			return true;

		if (m_toolBar && m_toolBar->ProcessKeyboardInput(type, code))
			return true;

		for (auto& child : m_children)
		{
			if (child->ProcessKeyboardInput(type, code))
				return true;
		}
		return false;
	}

#if 0

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

		m_defaultMaterial = m_mainBatch.material;
		m_defaultMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);

		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
		sh::video::Attribute colorAttribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 4U);
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

		m_textMaterial = m_textBatch.material;
		m_textMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);

		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
		sh::video::Attribute colorAttribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 4U);
		vertexDeclaration->AddAttribute(positionAttribute);	
		vertexDeclaration->AddAttribute(uvAttribute);
		vertexDeclaration->AddAttribute(colorAttribute);

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

#endif

} // gui

} // sh