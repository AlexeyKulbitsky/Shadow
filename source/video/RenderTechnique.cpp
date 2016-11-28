#include "RenderTechnique.h"
#include "RenderPass.h"
#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		RenderTechnique::RenderTechnique()
		{

		}

		RenderTechnique::~RenderTechnique()
		{

		}

		void RenderTechnique::Load(const char* filename)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filename);
			pugi::xml_node techniqueNode = doc.first_child();

			pugi::xml_node renderPassNode = techniqueNode.child("pass");
			while (renderPassNode)
			{
				RenderPass* renderPass = new RenderPass();
				renderPass->Load(renderPassNode);

				m_renderPasses.push_back(renderPass);
				
				renderPassNode = renderPassNode.next_sibling();
			}
			
		}
	}
}