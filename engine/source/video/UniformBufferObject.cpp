#include "UniformBufferObject.h"

namespace sh
{
	namespace video
	{

		UniformBufferObject::~UniformBufferObject()
		{
			if (m_size != 0U && !!m_data)
			{
				delete m_data;
				m_data = nullptr;
				m_size = 0U;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void UniformBufferObject::AddAttribute(const Attribute& attribute) 
		{ 
			m_attributes.push_back(attribute); 
			m_attributes[m_attributes.size() - 1].offset = m_size;
			m_size += attribute.size;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void UniformBufferObject::Init()
		{
			m_data = new u8[m_size];
		}

		//////////////////////////////////////////////////////////////////////////////////////////////


	}
}