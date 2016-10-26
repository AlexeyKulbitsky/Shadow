namespace sh
{
	namespace math
	{
		//////////////////////////////////////////////////////////////////////////

		inline bool equals(float v1, float v2, float tolerance)
		{
			assert(tolerance >= 0);
			// http://realtimecollisiondetection.net/pubs/Tolerances/
			// Abs(x - y) <= Max(absTol, relTol * Max(Abs(x), Abs(y)))
			// we assume absTol=relTol, this leaves
			// Abs(x - y) <= absTol * Max(1.0f, Abs(x), Abs(y))
			return math::abs(v1 - v2) <= tolerance * math::max_(1.0f, abs(v1), abs(v2));
		}

		inline bool equals(double v1, double v2, double tolerance)
		{
			assert(tolerance >= 0);
			return math::abs(v1 - v2) <= tolerance * math::max_(1.0, abs(v1), abs(v2));
		}

		inline bool equals(int8_t v1, int8_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(uint8_t v1, uint8_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(int16_t v1, int16_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(uint16_t v1, uint16_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(int32_t v1, int32_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(uint32_t v1, uint32_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(int64_t v1, int64_t v2)
		{
			return v1 == v2;
		}
		inline bool equals(uint64_t v1, uint64_t v2)
		{
			return v1 == v2;
		}

		inline bool equals(const vec2<float>& v1, const vec2<float>& v2, float tolerance)
		{
			return equals(v1.x, v2.x, tolerance) && equals(v1.y, v2.y, tolerance);
		}

		inline bool equals(const vec2<double>& v1, const vec2<double>& v2, double tolerance)
		{
			return equals(v1.x, v2.x, tolerance) && equals(v1.y, v2.y, tolerance);
		}

		inline bool equals(const vec3<float>& v1, const vec3<float>& v2, float tolerance)
		{
			return
				equals(v1.x, v2.x, tolerance) &&
				equals(v1.y, v2.y, tolerance) &&
				equals(v1.z, v2.z, tolerance);
		}

		inline bool equals(const vec3<double>& v1, const vec3<double>& v2, double tolerance)
		{
			return
				equals(v1.x, v2.x, tolerance) &&
				equals(v1.y, v2.y, tolerance) &&
				equals(v1.z, v2.z, tolerance);
		}

		inline bool equals(const vec4<float>& v1, const vec4<float>& v2, float tolerance)
		{
			return
				equals(v1.x, v2.x, tolerance) &&
				equals(v1.y, v2.y, tolerance) &&
				equals(v1.z, v2.z, tolerance) &&
				equals(v1.w, v2.w, tolerance);
		}

		inline bool equals(const vec4<double>& v1, const vec4<double>& v2, double tolerance)
		{
			return
				equals(v1.x, v2.x, tolerance) &&
				equals(v1.y, v2.y, tolerance) &&
				equals(v1.z, v2.z, tolerance) &&
				equals(v1.w, v2.w, tolerance);
		}

		inline bool equals(const mat2<float>& v1, const mat2<float>& v2, float tolerance)
		{
			for (uint32_t i = 0; i < 4; i++)
			{
				if (!equals(v1[i], v2[i], tolerance))
					return false;
			}
			return true;
		}
		inline bool equals(const mat2<double>& v1, const mat2<double>& v2, double tolerance)
		{
			for (uint32_t i = 0; i < 4; i++)
			{
				if (!equals(v1[i], v2[i], tolerance))
					return false;
			}
			return true;
		}

		inline bool equals(const mat3<float>& v1, const mat3<float>& v2, float tolerance)
		{
			for (uint32_t i = 0; i < 9; i++)
			{
				if (!equals(v1[i], v2[i], tolerance))
					return false;
			}
			return true;
		}

		inline bool equals(const mat3<double>& v1, const mat3<double>& v2, double tolerance)
		{
			for (uint32_t i = 0; i < 9; i++)
			{
				if (!equals(v1[i], v2[i], tolerance))
					return false;
			}
			return true;
		}

		inline bool equals(const mat4<float>& v1, const mat4<float>& v2, float tolerance)
		{
			for (uint32_t i = 0; i < 16; i++)
			{
				if (!equals(v1[i], v2[i], tolerance))
					return false;
			}
			return true;
		}

		inline bool equals(const mat4<double>& v1, const mat4<double>& v2, double tolerance)
		{
			for (uint32_t i = 0; i < 16; i++)
			{
				if (!equals(v1[i], v2[i], tolerance))
					return false;
			}
			return true;
		}

		inline bool equals(const quat<float>& v1, const quat<float>& v2, float tolerance)
		{
			return
				equals(v1.x, v2.x, tolerance) &&
				equals(v1.y, v2.y, tolerance) &&
				equals(v1.z, v2.z, tolerance) &&
				equals(v1.w, v2.w, tolerance);
		}

		inline bool equals(const quat<double>& v1, const quat<double>& v2, double tolerance)
		{
			return
				equals(v1.x, v2.x, tolerance) &&
				equals(v1.y, v2.y, tolerance) &&
				equals(v1.z, v2.z, tolerance) &&
				equals(v1.w, v2.w, tolerance);
		}

		//////////////////////////////////////////////////////////////////////////
		// zero with tolerance

		inline bool isZero(float v, float tolerance)
		{
			assert(tolerance >= 0);
			return math::abs(v) <= tolerance;
		}
		inline bool isZero(double v, double tolerance)
		{
			assert(tolerance >= 0);
			return math::abs(v) <= tolerance;
		}
		inline bool isZero(int8_t v)
		{
			return v == 0;
		}
		inline bool isZero(uint8_t v)
		{
			return v == 0;
		}
		inline bool isZero(int16_t v)
		{
			return v == 0;
		}
		inline bool isZero(uint16_t v)
		{
			return v == 0;
		}
		inline bool isZero(int32_t v)
		{
			return v == 0;
		}
		inline bool isZero(uint32_t v)
		{
			return v == 0;
		}
		inline bool isZero(int64_t v)
		{
			return v == 0;
		}
		inline bool isZero(uint64_t v)
		{
			return v == 0;
		}

		//////////////////////////////////////////////////////////////////////////
		// isZero floats

		inline bool isZero(const vec2<float>& v, float tolerance)
		{
			return isZero(v.x, tolerance) && isZero(v.y, tolerance);
		}

		inline bool isZero(const vec3<float>& v, float tolerance)
		{
			return
				isZero(v.x, tolerance) &&
				isZero(v.y, tolerance) &&
				isZero(v.z, tolerance);
		}

		inline bool isZero(const vec4<float>& v, float tolerance)
		{
			return
				isZero(v.x, tolerance) &&
				isZero(v.y, tolerance) &&
				isZero(v.z, tolerance) &&
				isZero(v.w, tolerance);
		}

		inline bool isZero(const mat2<float>& v, float tolerance)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				if (!isZero(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}
		inline bool isZero(const mat3<float>& v, float tolerance)
		{
			for (int32_t i = 0; i < 9; i++)
			{
				if (!isZero(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		inline bool isZero(const mat4<float>& v, float tolerance)
		{
			for (int32_t i = 0; i < 16; i++)
			{
				if (!isZero(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// isZero double

		inline bool isZero(const vec2<double>& v, double tolerance)
		{
			return isZero(v.x, tolerance) && isZero(v.y, tolerance);
		}

		inline bool isZero(const vec3<double>& v, double tolerance)
		{
			return
				isZero(v.x, tolerance) &&
				isZero(v.y, tolerance) &&
				isZero(v.z, tolerance);
		}

		inline bool isZero(const vec4<double>& v, double tolerance)
		{
			return
				isZero(v.x, tolerance) &&
				isZero(v.y, tolerance) &&
				isZero(v.z, tolerance) &&
				isZero(v.w, tolerance);
		}

		inline bool isZero(const mat2<double>& v, double tolerance)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				if (!isZero(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}
		inline bool isZero(const mat3<double>& v, double tolerance)
		{
			for (int32_t i = 0; i < 9; i++)
			{
				if (!isZero(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		inline bool isZero(const mat4<double>& v, double tolerance)
		{
			for (int32_t i = 0; i < 16; i++)
			{
				if (!isZero(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// one with tolerance

		inline bool isOne(float v, float tolerance)
		{
			assert(tolerance >= 0);
			return math::abs(v - 1.0f) <= tolerance;
		}
		inline bool isOne(double v, double tolerance)
		{
			assert(tolerance >= 0);
			return math::abs(v - 1.0) <= tolerance;
		}
		inline bool isOne(int8_t v)
		{
			return v == 1;
		}
		inline bool isOne(uint8_t v)
		{
			return v == 1;
		}
		inline bool isOne(int16_t v)
		{
			return v == 1;
		}
		inline bool isOne(uint16_t v)
		{
			return v == 1;
		}
		inline bool isOne(int32_t v)
		{
			return v == 1;
		}
		inline bool isOne(uint32_t v)
		{
			return v == 1;
		}
		inline bool isOne(int64_t v)
		{
			return v == 1;
		}
		inline bool isOne(uint64_t v)
		{
			return v == 1;
		}

		//////////////////////////////////////////////////////////////////////////
		// isOne floats

		inline bool isOne(const vec2<float>& v, float tolerance)
		{
			return isOne(v.x, tolerance) && isOne(v.y, tolerance);
		}

		inline bool isOne(const vec3<float>& v, float tolerance)
		{
			return
				isOne(v.x, tolerance) &&
				isOne(v.y, tolerance) &&
				isOne(v.z, tolerance);
		}

		inline bool isOne(const vec4<float>& v, float tolerance)
		{
			return
				isOne(v.x, tolerance) &&
				isOne(v.y, tolerance) &&
				isOne(v.z, tolerance) &&
				isOne(v.w, tolerance);
		}

		inline bool isOne(const mat2<float>& v, float tolerance)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				if (!isOne(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}
		inline bool isOne(const mat3<float>& v, float tolerance)
		{
			for (int32_t i = 0; i < 9; i++)
			{
				if (!isOne(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		inline bool isOne(const mat4<float>& v, float tolerance)
		{
			for (int32_t i = 0; i < 16; i++)
			{
				if (!isOne(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// isOne double

		inline bool isOne(const vec2<double>& v, double tolerance)
		{
			return isOne(v.x, tolerance) && isOne(v.y, tolerance);
		}

		inline bool isOne(const vec3<double>& v, double tolerance)
		{
			return
				isOne(v.x, tolerance) &&
				isOne(v.y, tolerance) &&
				isOne(v.z, tolerance);
		}

		inline bool isOne(const vec4<double>& v, double tolerance)
		{
			return
				isOne(v.x, tolerance) &&
				isOne(v.y, tolerance) &&
				isOne(v.z, tolerance) &&
				isOne(v.w, tolerance);
		}

		inline bool isOne(const mat2<double>& v, double tolerance)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				if (!isOne(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}
		inline bool isOne(const mat3<double>& v, double tolerance)
		{
			for (int32_t i = 0; i < 9; i++)
			{
				if (!isOne(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		inline bool isOne(const mat4<double>& v, double tolerance)
		{
			for (int32_t i = 0; i < 16; i++)
			{
				if (!isOne(v.m[i], tolerance))
				{
					return false;
				}
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// identity with tolerance

		inline bool isIdentity(const mat2<float>& v, float tolerance)
		{
			return
				isOne(v[0], tolerance) &&
				isZero(v[1], tolerance) &&
				isZero(v[2], tolerance) &&
				isOne(v[3], tolerance);
		}
		inline bool isIdentity(const mat3<float>& v, float tolerance)
		{
			return
				isOne(v[0], tolerance) &&
				isZero(v[1], tolerance) &&
				isZero(v[2], tolerance) &&

				isZero(v[3], tolerance) &&
				isOne(v[4], tolerance) &&
				isZero(v[5], tolerance) &&

				isZero(v[6], tolerance) &&
				isZero(v[7], tolerance) &&
				isOne(v[8], tolerance);
		}

		inline bool isIdentity(const mat4<float>& v, float tolerance)
		{
			return
				isOne(v[0], tolerance) &&
				isZero(v[1], tolerance) &&
				isZero(v[2], tolerance) &&
				isZero(v[3], tolerance) &&

				isZero(v[4], tolerance) &&
				isOne(v[5], tolerance) &&
				isZero(v[6], tolerance) &&
				isZero(v[7], tolerance) &&

				isZero(v[8], tolerance) &&
				isZero(v[9], tolerance) &&
				isOne(v[10], tolerance) &&
				isZero(v[11], tolerance) &&

				isZero(v[12], tolerance) &&
				isZero(v[13], tolerance) &&
				isZero(v[14], tolerance) &&
				isOne(v[15], tolerance);
		}

		inline bool isIdentity(const quat<float>& v, float tolerance)
		{
			return
				isZero(v.x, tolerance) &&
				isZero(v.y, tolerance) &&
				isZero(v.z, tolerance) &&
				isOne(v.w, tolerance);
		}

		inline bool isIdentity(const mat2<double>& v, double tolerance)
		{
			return
				isOne(v[0], tolerance) &&
				isZero(v[1], tolerance) &&
				isZero(v[2], tolerance) &&
				isOne(v[3], tolerance);
		}
		inline bool isIdentity(const mat3<double>& v, double tolerance)
		{
			return
				isOne(v[0], tolerance) &&
				isZero(v[1], tolerance) &&
				isZero(v[2], tolerance) &&

				isZero(v[3], tolerance) &&
				isOne(v[4], tolerance) &&
				isZero(v[5], tolerance) &&

				isZero(v[6], tolerance) &&
				isZero(v[7], tolerance) &&
				isOne(v[8], tolerance);
		}

		inline bool isIdentity(const mat4<double>& v, double tolerance)
		{
			return
				isOne(v[0], tolerance) &&
				isZero(v[1], tolerance) &&
				isZero(v[2], tolerance) &&
				isZero(v[3], tolerance) &&

				isZero(v[4], tolerance) &&
				isOne(v[5], tolerance) &&
				isZero(v[6], tolerance) &&
				isZero(v[7], tolerance) &&

				isZero(v[8], tolerance) &&
				isZero(v[9], tolerance) &&
				isOne(v[10], tolerance) &&
				isZero(v[11], tolerance) &&

				isZero(v[12], tolerance) &&
				isZero(v[13], tolerance) &&
				isZero(v[14], tolerance) &&
				isOne(v[15], tolerance);
		}

		inline bool isIdentity(const quat<double>& v, double tolerance)
		{
			return
				isZero(v.x, tolerance) &&
				isZero(v.y, tolerance) &&
				isZero(v.z, tolerance) &&
				isOne(v.w, tolerance);
		}


		inline bool isNan(float value)
		{
#ifdef _WIN32
			return _isnan(value) != 0;
#else
			return std::isnan(value);
#endif
		}

		inline bool isNan(double value)
		{
#ifdef _WIN32
			return _isnan(value) != 0;
#else
			return std::isnan(value);
#endif
		}
	}
}