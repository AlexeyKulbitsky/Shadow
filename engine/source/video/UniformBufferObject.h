#ifndef SHADOW_UNIFORM_BUFFER_OBJECT_INCLUDE
#define SHADOW_UNIFORM_BUFFER_OBJECT_INCLUDE

#include "Uniform.h"

namespace sh
{
	namespace video
	{
		class UniformBufferObject
		{
		public:
			class Element
			{
			public:
				Uniform::Type type = Uniform::Type::UNKNOWN;
				String name = "noname";
				u32 size = 0U;
				u32 offset = 0U;
				GlobalUniformName globalName = GlobalUniformName::NONE;
			};

			
			~UniformBufferObject();
			virtual void AddElement(const Element& element);
			virtual void Init();
			virtual UniformBufferObject* Clone();

			u32 GetSize() const { return m_size; }
			//u8* GetData() const { return m_data; }

		protected:
			std::vector<Element> m_elements;
			u32 m_size = 0U;
			//u8* m_data = nullptr;
		};
	}
}


#endif
