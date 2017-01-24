#include "RenderTechnique.h"
#include "RenderPipeline.h"
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

			pugi::xml_node renderPassNode = techniqueNode.child("pipeline");
			while (renderPassNode)
			{
				RenderPipeline* renderPass = new RenderPipeline();
				renderPass->Load(renderPassNode);

				m_renderPasses.push_back(renderPass);
				
				renderPassNode = renderPassNode.next_sibling();
			}
			
		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::Unload()
		{

		}
	}
}