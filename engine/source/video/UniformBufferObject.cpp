#include "UniformBufferObject.h"

namespace sh
{
	namespace video
	{

		UniformBufferObject::~UniformBufferObject()
		{
			/*
			if (m_size != 0U && !!m_data)
			{
				delete m_data;
				m_data = nullptr;
				m_size = 0U;
			}
			*/
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void UniformBufferObject::AddElement(const Element& element)
		{ 
			m_elements.push_back(element);
			m_elements[m_elements.size() - 1].offset = m_size;
			m_size += element.size;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void UniformBufferObject::Init()
		{
			//m_data = new u8[m_size];
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		UniformBufferObject* UniformBufferObject::Clone()
		{
			UniformBufferObject* res = new UniformBufferObject();
			res->m_size = m_size;
			res->m_elements = m_elements;

			return res;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	}
}