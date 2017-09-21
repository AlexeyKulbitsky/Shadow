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

#include <iostream>


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
		m_defaultMaterial->SetRenderTechnique("ui_base.rt");
		m_defaultMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);

		m_textMaterial.reset(new video::Material());
		m_textMaterial->SetRenderTechnique("text_base.rt");
		m_textMaterial->GetCommonGpuParams()->SetParam("orthoMat", mat);
	}

	void GuiManager::Update(u32 delta)
	{

	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		auto painter = driver->GetPainter();

		for (auto child = m_children.rbegin(); child != m_children.rend(); ++child)
		{
			(*child)->Render(painter);
		}

		if (m_toolBar)
			m_toolBar->Render(painter);

		if (m_menuBar)
			m_menuBar->Render(painter);

		if (m_focusWidget && m_focusWidget->IsInFocus())
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

		m_defaultMaterial->GetCommonGpuParams()->SetSampler("diffuse", texture);
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

		m_textMaterial->GetCommonGpuParams()->SetSampler("fontAtlas", texture);
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

		video::SamplerDescription samplerDesc;
		samplerDesc.type = GPOT_SAMPLER_2D;
		samplerDesc.tilingU = TEX_TILING_CLAMP_TO_EDGE;
		samplerDesc.tilingV = TEX_TILING_CLAMP_TO_EDGE;
		samplerDesc.magFilter = TEX_FILT_NEAREST;
		samplerDesc.minFilter = TEX_FILT_NEAREST;

		auto sampler = video::Sampler::Create(samplerDesc);
		sampler->Set(m_style->GetTexure());
		m_defaultMaterial->GetCommonGpuParams()->SetSampler("diffuse", sampler);

		//m_defaultMaterial->GetCommonGpuParams()->SetSampler("diffuse", m_style->GetTexure());
	}

	bool GuiManager::ProcessEvent(GUIEvent& ev)
	{
		if (m_menuBar && m_menuBar->ProcessEvent(ev))
			return true;

		if (m_toolBar && m_toolBar->ProcessEvent(ev))
			return true;

		if (m_focusWidget)
		{
			if (m_focusWidget->IsInFocus())
			{
				return m_focusWidget->ProcessEvent(ev);
			}
			else
			{
				SetFocusWidget(nullptr);
				return true;
			}
		}

		for (auto child = m_children.begin(); child != m_children.end(); ++child)
		{
			if ((*child)->ProcessEvent(ev))
			{
				if (ev.type == EventType::PointerDown ||
					ev.type == EventType::PointerDown)
				{
					auto childWidget = *child;
					m_children.erase(child);
					m_children.push_front(childWidget);
				}
				
				return true;
			}
		}
		return false;
	}

} // gui

} // sh