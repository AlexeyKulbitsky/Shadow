#ifndef SHADOW_UNIFORM_INCLUDE
#define SHADOW_UNIFORM_INCLUDE

#include <string>
#include "../pempek_assert.h"
#include "../Globals.h"

namespace sh
{
	namespace video
	{
		
		class Uniform
		{
			enum class Type
			{
				FLOAT,
				INT,
				VEC2,
				VEC3,
				VEC4,
				MAT4,
				UNKNOWN
			};

		public:		
			template<typename T>
			Uniform(const T& value);

			~Uniform() { delete m_placeHolder; }

			template<typename T>
			void Set(const T& value)
			{
				SH_ASSERT(typeid(T) == m_placeHolder->GetTypeInfo(), "Error types!");
				static_cast<UniformHolder<T>*>(m_placeHolder)->SetValue(value);
			}

			template<typename T>
			const T& Get() const
			{
				SH_ASSERT(typeid(T) == m_placeHolder->GetTypeInfo(), "Error types!");
				return static_cast<UniformHolder<T>*>(m_placeHolder)->GetValue();
			}

			void SetName(const std::string& name) { m_name = name; }
			const std::string& GetName() const { return m_name; }

			virtual void Load() = 0;
			virtual void Init() = 0;
			virtual Uniform* Clone() = 0;

		private:
			class UniformPlaceHolder
			{
			public:
				virtual ~UniformPlaceHolder() {}
				virtual const std::type_info& GetTypeInfo() const = 0;
			};

			template <typename T>
			class UniformHolder : public UniformPlaceHolder
			{
			public:
				UniformHolder(){}
				UniformHolder(const T& value) : m_value(value) {}
				virtual const std::type_info& GetTypeInfo() const override { return typeid(m_value); }
				void SetValue(const T& value) { m_value = value; }
				const T& GetValue() const { return m_value; }
			public:
				T m_value;
			};		

		protected:
			Type m_type;
			std::string m_name;
			UniformPlaceHolder* m_placeHolder;
		};

		/////////////////////////////////////////////////////////////////////////////////
		// Constructors

		template<typename T>
		inline Uniform::Uniform(const T& value)
			: m_placeHolder(new UniformHolder<T>(value))
			, m_type(Type::UNKNOWN)
		{		
		}

		template<>
		inline Uniform::Uniform(const float& value)
			: m_placeHolder(new UniformHolder<float>(value))
			, m_type(Type::FLOAT)
		{
		}

		template<>
		inline Uniform::Uniform(const int& value)
			: m_placeHolder(new UniformHolder<int>(value))
			, m_type(Type::INT)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Vector2f& value)
			: m_placeHolder(new UniformHolder<math::Vector2f>(value))
			, m_type(Type::VEC2)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Vector3f& value)
			: m_placeHolder(new UniformHolder<math::Vector3f>(value))
			, m_type(Type::VEC3)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Vector4f& value)
			: m_placeHolder(new UniformHolder<math::Vector4f>(value))
			, m_type(Type::VEC4)
		{
		}

		template<>
		inline Uniform::Uniform(const math::Matrix4f& value)
			: m_placeHolder(new UniformHolder<math::Matrix4f>(value))
			, m_type(Type::MAT4)
		{
		}
	}
}

#endif
