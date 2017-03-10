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
			class Attribute
			{
			public:
				Uniform::Type type;
				String name;
				u32 size;
				u32 offset;
			};

			
			~UniformBufferObject();
			void AddAttribute(const Attribute& attribute);
			void Init();

			u32 GetSize() const { return m_size; }
			u8* GetData() const { return m_data; }

		private:
			std::vector<Attribute> m_attributes;
			u32 m_size = 0U;
			u8* m_data = nullptr;
		};
	}
}


#endif
