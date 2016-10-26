namespace sh
{
	namespace math
	{
		template<class T>
		inline T inverse(const T& v)
		{
			assert(v != 0);
			return 1 / v;
		}
		template<class T>
		inline mat3<T> inverse(const mat3<T>& v)
		{
			mat3<T> tmp(v);
			bool res = tmp.invert();
			assert(res);
			return tmp;
		}
		template<class T>
		inline mat4<T> inverse(const mat4<T>& v)
		{
			mat4<T> tmp(v);
			bool res = tmp.invert();
			assert(res);
			return tmp;
		}
		template<class T>
		inline quat<T> inverse(const quat<T>& v)
		{
			quat<T> tmp(v);
			tmp.invert();
			return tmp;
		}

		template<class T>
		inline mat3<T> transposed(const mat3<T>& v)
		{
			mat3<T> tmp(v);
			tmp.transpose();
			return tmp;
		}
		template<class T>
		inline mat4<T> transposed(const mat4<T>& v)
		{
			mat4<T> tmp(v);
			tmp.transpose();
			return tmp;
		}

		template<class T>
		inline vec2<T> normalized(const vec2<T>& v)
		{
			vec2<T> tmp(v);
			tmp.normalize();
			return tmp;
		}

		template<class T>
		inline vec3<T> normalized(const vec3<T>& v)
		{
			vec3<T> tmp(v);
			tmp.normalize();
			return tmp;
		}

		template<class T>
		inline vec4<T> normalized(const vec4<T>& v)
		{
			vec4<T> tmp(v);
			tmp.normalize();
			return tmp;
		}

		template<typename T>
		inline quat<T> normalized(const quat<T>& v)
		{
			quat<T> tmp(v);
			tmp.normalize();
			return tmp;
		}

		template<class T>
		inline vec2<T> normalizedFast(const vec2<T>& v)
		{
			vec2<T> tmp(v);
			T len = tmp.getLengthSq();
			T invLen = invSqrtFast(len);
			tmp = *invLen;
			return tmp;
		}

		template<class T>
		inline vec3<T> normalizedFast(const vec3<T>& v)
		{
			vec3<T> tmp(v);
			T len = tmp.getLengthSq();
			T invLen = invSqrtFast(len);
			tmp *= invLen;
			return tmp;
		}

		template<class T>
		inline vec4<T> normalizedFast(const vec4<T>& v)
		{
			vec4<T> tmp(v);
			T len = tmp.getLengthSq();
			T invLen = invSqrtFast(len);
			tmp = *invLen;
			return tmp;
		}

		template<typename T>
		inline quat<T> normalizedFast(const quat<T>& v)
		{
			quat<T> tmp(v);
			T len = tmp.getLengthSq();
			T invLen = invSqrtFast(len);
			tmp = *invLen;
			return tmp;
		}


		template <typename T>
		inline T dot(const vec2<T>& v1, const vec2<T>& v2)
		{
			return v1.x*v2.x + v1.y*v2.y;
		}

		template <typename T>
		inline T dot(const vec3<T>& v1, const vec3<T>& v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}

		template <typename T>
		inline T dot(const vec4<T>& v1, const vec4<T>& v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
		}

		template<class T>
		inline T cross(const vec2<T>& v1, const vec2<T>& v2)
		{
			return (v1.x*v2.y - v1.y*v2.x);
		}

		template<class T>
		inline vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2)
		{
			return vec3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
		}

		template<class T>
		inline vec4<T> cross(const vec4<T>& x1, const vec4<T>& x2, const vec4<T>& x3)
		{
			// 4d Cross product (GPG6, section 2.2)
#define DOT(AX,AY,AZ,BX,BY,BZ) AX*BX+AY*BY+AZ*BZ
#define CROSS(AX,AY,AZ,BX,BY,BZ,CX,CY,CZ) CX = AY*BZ - AZ*BY; CY = AZ*BX - AX*BZ; CZ = AX*BY - AY*BX;
			vec4<T> c1(vec4<T>::uninitialized);
			vec4<T> c2(vec4<T>::uninitialized);
			vec4<T> c3(vec4<T>::uninitialized);
			vec4<T> c4(vec4<T>::uninitialized);
			CROSS(x2.y, x2.z, x2.w, x3.y, x3.z, x3.w, c1.x, c1.y, c1.z);
			CROSS(x2.x, x2.z, x2.w, x3.x, x3.z, x3.w, c2.x, c2.y, c2.z);
			CROSS(x2.x, x2.y, x2.w, x3.x, x3.y, x3.w, c3.x, c3.y, c3.z);
			CROSS(x2.x, x2.y, x2.z, x3.x, x3.y, x3.z, c4.x, c4.y, c4.z);

			return vec4<T>(
				DOT(x1.y, x1.z, x1.w, c1.x, c1.y, c1.z),
				-DOT(x1.x, x1.z, x1.w, c2.x, c2.y, c2.z),
				DOT(x1.x, x1.y, x1.w, c3.x, c3.y, c3.z),
				-DOT(x1.x, x1.y, x1.z, c4.x, c4.y, c4.z)
				);

#undef DOT
#undef CROSS
		}

		template<class T>
		inline T square(const T& v)
		{
			return v*v;
		}

		template<class T>
		inline T distance(const vec2<T>& v1, const vec2<T>& v2)
		{
			return (v2 - v1).getLength();
		}

		template<class T>
		inline T distance(const vec3<T>& v1, const vec3<T>& v2)
		{
			return (v2 - v1).getLength();
		}

		template<class T>
		inline T distance(const vec4<T>& v1, const vec4<T>& v2)
		{
			return (v2 - v1).getLength();
		}

		template<class T>
		inline T distance(const T& v1, const T& v2)
		{
			return (v2 >= v1) ? (v2 - v1) : (v1 - v2);
		}

		template<class T>
		inline T distanceSq(const T& v1, const T& v2)
		{
			T a = v1*v1;
			T b = v2*v2;
			return (a >= b) ? a - b : b - a;
		}

		template<class T>
		inline T distanceSq(const vec2<T>& v1, const vec2<T>& v2)
		{
			return (v2 - v1).getLengthSq();
		}

		template<class T>
		inline T distanceSq(const vec3<T>& v1, const vec3<T>& v2)
		{
			return (v2 - v1).getLengthSq();
		}

		template<class T>
		inline T distanceSq(const vec4<T>& v1, const vec4<T>& v2)
		{
			return (v2 - v1).getLengthSq();
		}


	}
}