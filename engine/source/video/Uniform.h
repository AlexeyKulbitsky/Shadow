#ifndef SHADOW_UNIFORM_INCLUDE
#define SHADOW_UNIFORM_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		enum class GlobalUniformName
		{
			NONE = -1,

			// Model uniforms
			MODEL_WORLD_MATRIX = 0,
			MODEL_WORLD_VIEW_PROJECTION_MATRIX,
			MODEL_NORMAL_MATRIX,

			// Camera uniforms			
			CAMERA_VIEW_MATRIX,
			CAMERA_VIEW_TRANSLATION_MATRIX,
			CAMERA_VIEW_ROTATION_MATRIX,
			CAMERA_PROJECTION_MATRIX,
			CAMERA_VIEW_PROJECTION_MATRIX,
			CAMERA_VIEW_ROTATION_PROJECTION_MATRIX,
			CAMERA_POSITION,

			// Light uniforms
			LIGHT_DIRECTIONAL_LIGHTS_COUNT,
			LIGHT_DIRECTIONAL_DIRECTION,
			LIGHT_DIRECTIONAL_COLOR,

			COUNT
		};

		static const String k_globalUniformsStringNames[] =
		{
			// Model uniforms
			String("model.worldMatrix"),
			String("model.worldViewProjectionMatrix"),
			String("model.normalMatrix"),

			// Camera uniforms
			String("camera.viewMatrix"),
			String("camera.viewTranslationMatrix"),
			String("camera.viewRotationMatrix"),
			String("camera.projectionMatrix"),
			String("camera.viewProjectionMatrix"),		
			String("camera.viewRotationProjectionMatrix"),
			String("camera.position"),

			// Light uniforms
			String("light.directional.count"),
			String("light.directional.direction"),
			String("light.directional.color")
		};

		class Uniform
		{
			public:	
			/*
			Base implementation used just for holding values
			More specific type of Uniform that includes renderindg system
			information is defined in derived classes for each renser system
			*/
			enum class Type
			{
				FLOAT,
				INT,
				VEC2,
				VEC3,
				VEC4,
				MAT4,
				VEC3_ARRAY,
				UNKNOWN
			};
		
			enum class Usage
			{
				UNKNOWN
			};

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
			void SetUsage(Usage usage) { m_usage = usage; }
			Usage GetUsage() const { return m_usage; }
			void SetGlobalUniformName(GlobalUniformName name) { m_globalName = name; }
			GlobalUniformName GetGlobalUniformName() const { return m_globalName; }
			Type GetType() const { return m_type; }

			virtual void Upload() {}
			virtual void Init() {}
			virtual Uniform* Clone() { return nullptr; }

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
				UniformHolder() {}
				UniformHolder(const T& value) : m_value(value) {}
				virtual const std::type_info& GetTypeInfo() const override { return typeid(m_value); }
				void SetValue(const T& value) { m_value = value; }
				const T& GetValue() const { return m_value; }
			public:
				T m_value;
			};

		protected:
			Type m_type;
			Usage m_usage;
			GlobalUniformName m_globalName = GlobalUniformName::NONE;
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

		template<>
		inline Uniform::Uniform(const std::vector<math::Vector3f>& value)
			: m_placeHolder(new UniformHolder<std::vector<math::Vector3f> >(value))
			, m_type(Type::VEC3_ARRAY)
		{
		}
	}
}


#endif
