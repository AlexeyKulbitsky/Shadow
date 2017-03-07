#include "RenderTechnique.h"
#include "RenderPipeline.h"
#include "Driver.h"
#include "../Device.h"
#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		RenderTechnique::RenderTechnique()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////////

		RenderTechnique::~RenderTechnique()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::Load(const String& filePath)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.c_str());
			pugi::xml_node techniqueNode = doc.first_child();
			pugi::xml_attribute nameAttr = techniqueNode.attribute("name");
			if (nameAttr)
			{
				m_name = nameAttr.as_string();
			}

			pugi::xml_node renderPipelineNode = techniqueNode.child("pipeline");
			while (renderPipelineNode)
			{		
				RenderPipelinePtr renderPipeline = Device::GetInstance()->GetDriver()->CreateRenderPipeline();
				//RenderPipelinePtr renderPipeline = RenderPipelinePtr(new RenderPipeline());
				renderPipeline->Load(renderPipelineNode);

				m_renderPipelines.push_back(renderPipeline);
				
				renderPipelineNode = renderPipelineNode.next_sibling();
			}
			
		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::Unload()
		{
			for (size_t i = 0; i < m_renderPipelines.size(); ++i)
			{
				m_renderPipelines[i]->Unload();
			}
			m_renderPipelines.clear();
		}
	}
}