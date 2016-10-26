namespace sh
{
	namespace math
	{
		inline int8_t lerp(int8_t a, int8_t b, float t)
		{
			assert(t >= 0 && t <= 1);
			float x = (float)(b - a);
			return a + (int8_t)(x*t);
		}
		inline uint8_t lerp(uint8_t a, uint8_t b, float t)
		{
			assert(t >= 0 && t <= 1);
			return (uint8_t)((float)a*(1.0f - t) + (float)b*t);
		}
		inline int16_t lerp(int16_t a, int16_t b, float t)
		{
			assert(t >= 0 && t <= 1);
			float x = (float)(b - a);
			return a + (int16_t)(x*t);
		}
		inline uint16_t lerp(uint16_t a, uint16_t b, float t)
		{
			assert(t >= 0 && t <= 1);
			return (uint16_t)((float)a*(1.0f - t) + (float)b*t);
		}
		inline int32_t lerp(int32_t a, int32_t b, float t)
		{
			assert(t >= 0 && t <= 1);
			double t2 = t; //An uint32_t does not fit in float
			double x = b - a;
			return a + (int32_t)(x*t2);
		}
		inline uint32_t lerp(uint32_t a, uint32_t b, float t)
		{
			assert(t >= 0 && t <= 1);
			double t2 = t; //An uint32_t does not fit in float
			return (uint32_t)((double)a*(1.0 - t2) + (double)b*t2);
		}
		inline float lerp(float a, float b, float t)
		{
			assert(t >= 0 && t <= 1);
			float x = b - a;
			return a + x*t;
		}
		inline double lerp(double a, double b, float t)
		{
			assert(t >= 0 && t <= 1);
			double x = b - a;
			return a + x*t;
		}

		template <typename T>
		math::quat<T> lerp(const math::quat<T>& a, const math::quat<T>& b, float t)
		{
			T angle = dot(a, b);

			float scale;
			float invscale;
			if (angle > 0.998f)
			{
				scale = 1.0f - t;
				invscale = t;
				return quat<T>((a*scale) + (b*invscale));
			}
			else
			{
				if (angle < 0.0f)
				{
					if (angle <= -0.9999f)
					{
						return quat<T>(t < 0.5f ? a : b);
					}
					else
					{
						quat<T> qa(-a);
						angle = -1 * math::max(angle, T(-1));
						const float theta = (float)math::acos(angle);
						const float invsintheta = 1.0f / (float)math::sin(theta);
						scale = math::sin(theta * (1.0f - t)) * invsintheta;
						invscale = math::sin(theta * t) * invsintheta;
						return quat<T>((qa * scale) + (b*invscale));
					}
				}
				else
				{
					const float theta = math::acos(angle);
					const float invsintheta = 1.0f / math::sin(theta);
					scale = math::sin(theta * (1.0f - t)) * invsintheta;
					invscale = math::sin(theta * t) * invsintheta;
					return quat<T>((a*scale) + (b*invscale));
				}
			}
		}

		template<class T>
		inline T lerp(const T& a, const T& b, float t)
		{
			assert(t >= 0 && t <= 1);
			return (T)(a*(1.f - t) + b*t);
		}

		template<typename T>
		inline quat<T> nlerp(const quat<T>& a, const quat<T>& b, float t)
		{
			assert(t >= 0 && t <= 1);
			math::quat<T> result;
			T angle = dot(a, b);
			if (angle >= 0)
			{
				result = (a*(1 - t) + b*t);
			}
			else if (angle <= -0.9999)
			{
				result = t < T(0.5) ? a : b;
			}
			else if (angle < 0)
			{
				math::quat<T> r = b * -1;
				result = (a*(1 - t) + r*t);
			}

			result.normalize();
			return result;
		}

		template<typename T>
		inline quat<T> slerp(const quat<T>& a, const quat<T>& b, float t)
		{
			return lerp(a, b, t);
		}

	}
}