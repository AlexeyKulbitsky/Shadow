#ifndef SHADOW_UNIFORM_INCLUDE
#define SHADOW_UNIFORM_INCLUDE

#include <string>

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
				UNKNOWN
			};

		public:
			
			template<typename T>
			Uniform(const T& value);

			~Uniform() { delete m_placeHolder; }

			template<typename T>
			void Set(const T& value);

			template<typename T>
			void Get(T& value);

		private:
			class UniformPlaceHolder
			{
			public:
				virtual ~UniformPlaceHolder() {}
				virtual void* GetData() = 0;
			};

			template <typename T>
			class UniformHolder : public UniformPlaceHolder
			{
			public:
				UniformHolder(const T& value) : m_value(value) {}
				virtual void* GetData() override { return (void*)(&m_value); }
			public:
				T m_value;
			};

			virtual void Load() = 0;

		protected:
			Type m_type;
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

		/////////////////////////////////////////////////////////////////////////////////
		// Setters

		template<typename T>
		inline void Uniform::Set(const T& value)
		{
			memcpy(m_placeHolder->GetData(), (const void*)(&value), sizeof(value));
		}

		template<>
		inline void Uniform::Set(const float& value)
		{
			memcpy(m_placeHolder->GetData(), (const float*)(&value), sizeof(float));
		}

		template<>
		inline void Uniform::Set(const int& value)
		{
			memcpy(m_placeHolder->GetData(), (const int*)(&value), sizeof(int));
		}

		/////////////////////////////////////////////////////////////////////////////////
		// Getters

		template<typename T>
		inline void Uniform::Get(T& value)
		{
			memcpy((void*)(&value), m_placeHolder->GetData(), sizeof(value));
		}

		template<>
		inline void Uniform::Get(float& value)
		{
			memcpy((float*)(&value), m_placeHolder->GetData(), sizeof(float));
		}

		template<>
		inline void Uniform::Get(int& value)
		{
			memcpy((int*)(&value), m_placeHolder->GetData(), sizeof(int));
		}
	}
}

#endif
