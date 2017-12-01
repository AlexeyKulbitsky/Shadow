#ifndef SHADOW_LIGHT_INCLUDE
#define SHADOW_LIGHT_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class SHADOW_API Light
		{
		public:
			enum class Type
			{
				POINT,
				DIRECTIONAL,
				SPOT,
				AMBIENT
			};

			Light();
			~Light();

			void SetType(Type type) { m_type = type; }
			Type GetType() const { return m_type; }

			void SetPosition(const math::Vector3f& position) { m_position = position; }
			const math::Vector3f& GetPosition() const { return m_position; }

			void SetDirection(const math::Vector3f& direction) { m_direction = direction; }
			const math::Vector3f& GetDirection() const { return m_direction; }

			void SetColor(const math::Vector3f& color) { m_color = color; }
			const math::Vector3f&  GetColor() const { return m_color; }

		private:
			Type m_type;
			math::Vector3f m_position;
			math::Vector3f m_direction;

			math::Vector3f m_color;
		};
	}
}

#endif