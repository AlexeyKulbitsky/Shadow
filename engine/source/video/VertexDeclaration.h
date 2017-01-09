#ifndef SHADOW_VERTEX_DECLARATION_INCLUDE
#define SHADOW_VERTEX_DECLARATION_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		enum class AttributeSemantic
		{
			POSITION,
			COLOR,
			UV,
			NORMAL,
			BINORMAL,
			TANGENT
		};

		enum class AttributeType
		{
			FLOAT = 4,
			INT = 4,
			UNSIGNED_INT = 4,
			BYTE = 1,
			UNSIGNED_BYTE = 1
		};

		struct Attribute
		{
			Attribute(AttributeSemantic _semantic, AttributeType _type, size_t _componentsCount)
				: semantic(_semantic)
				, type(_type)
				, componentsCount(_componentsCount)
			{}

			AttributeSemantic semantic;
			AttributeType type;
			// Attribute offset (in bytes)
			size_t offset;
			size_t componentsCount;
		};

		class VertexDeclaration
		{
		public:
			VertexDeclaration() {}
			VertexDeclaration(const VertexDeclaration& other)
				: m_attributes(other.m_attributes)
				, m_stride(other.m_stride)
			{
			}

			void AddAttribute(Attribute& attribute)
			{
				attribute.offset = CalculateAttributeOffset(attribute);
				m_attributes.push_back(attribute);
				m_stride += static_cast<size_t>(attribute.type) * attribute.componentsCount;
			}
			Attribute* GetAttribute(AttributeSemantic sematic)
			{
				for (size_t i = 0, sz = m_attributes.size(); i < sz; ++i)
				{
					if (m_attributes[i].semantic == sematic)
						return &m_attributes[i];
				}
				return nullptr;
			}

			size_t GetStride() const { return m_stride; }
			size_t GetAttributesCount() const { return m_attributes.size(); }
			Attribute* GetAttribute(size_t attributeIndex) { return &m_attributes[attributeIndex]; }

			VertexDeclaration& operator=(const VertexDeclaration& other)
			{
				m_attributes = other.m_attributes;
				m_stride = other.m_stride;
				return *this;
			}

		private:
			size_t CalculateAttributeOffset(const Attribute& attribute)
			{
				size_t offset = 0U;
				for (auto attr : m_attributes)
				{
					offset += static_cast<size_t>(attr.type) * attr.componentsCount;
				}
				return offset;
			}

		private:
			std::vector<Attribute> m_attributes;
			size_t m_stride = 0U;
		};

		class ShaderProgram;

		class VertexInputDeclaration
		{
		public:
			virtual void Load(const pugi::xml_node &node){}
			virtual void Init(){}
			virtual VertexInputDeclaration* Clone() { return nullptr; }
			virtual void Assemble(VertexDeclaration& declaration){}
			virtual void SetShaderProgram(ShaderProgram* shaderProgram){}
		};
	}
}

#endif // !SHADOW_VERTEX_DECLARATION_INCLUDE