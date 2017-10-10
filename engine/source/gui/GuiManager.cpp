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
		m_defaultMaterial.reset(new video::Material());
		m_defaultMaterial->SetRenderTechnique("ui_base.rt");

		m_textMaterial.reset(new video::Material());
		m_textMaterial->SetRenderTechnique("text_base.rt");
	}

	void GuiManager::Update(u32 delta)
	{
		if (!m_focusWidgetsQueue.empty())
		{
			m_focusWidget = m_focusWidgetsQueue.front();
			m_focusWidgetsQueue.pop();
		}
	}

	void GuiManager::Render()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		auto painter = driver->GetPainter();

		for (auto child = m_children.rbegin(); child != m_children.rend(); ++child)
		{
			// Draw only unfocused widgets
			if (!(*child)->IsInFocus())
				(*child)->Render(painter);
		}

		// The only focus widget must be drawn after all widgets are ready
		if (m_focusWidget)
		{
			m_focusWidget->Render(painter);
		}

		painter->Flush();
	}

	void GuiManager::LoadGui(const String& name)
	{
		auto fileSystem = sh::Device::GetInstance()->GetFileSystem();
		const auto file = fileSystem->LoadFile(name);

		
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer(file.GetData().data(), file.GetData().size());
			
		pugi::xml_node child = doc.first_child();
		sh::String texFilename = child.attribute("val").as_string();
		//sh::video::TexturePtr texture = sh::Device::GetInstance()->GetResourceManager()->GetTexture(texFilename);
		sh::video::TexturePtr texture = video::TextureLoader::GetInstance()->GetWhiteTexture();
		SH_ASSERT(!!texture, "Can not load texture!");

		m_defaultMaterial->GetCommonGpuParams()->SetSampler("diffuse", texture);

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

	void GuiManager::AddChildFront(const SPtr<Widget>& child)
	{
		m_children.push_front(child);
	}

	void GuiManager::RemoveChild(size_t index)
	{
		if (index >= m_children.size())
			return;

		m_children.erase(m_children.begin());
	}

	void GuiManager::SetFont(const FontPtr& font)
	{
		m_font = font;

		const auto& texture = m_font->GetTextureAtlas();
		SH_ASSERT(!!texture, "Can not set empty font to Gui manager!");

		m_textMaterial->GetCommonGpuParams()->SetSampler("fontAtlas", texture);
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
	}

	void GuiManager::SetFocusWidget(const SPtr<Widget>& widget) 
	{ 
		m_focusWidgetsQueue.push(widget);
	}

	bool GuiManager::ProcessEvent(GUIEvent& ev)
	{
		if (m_focusWidget)
		{
			return m_focusWidget->ProcessEvent(ev);
		}

		for (auto child = m_children.begin(); child != m_children.end(); ++child)
		{
			if ((*child)->IsInFocus())
				continue;

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