#ifndef SHADOW_QUATERNION_INCLUDE
#define SHADOW_QUATERNION_INCLUDE

#include "Matrix4.h"
#include "Matrix3.h"

namespace sh
{
	namespace math
	{
		template<typename T>
		struct Quaternion
		{
			T x, y, z, w;

			Quaternion() : x(0), y(0), z(0), w(1) {}
			Quaternion(T _x, T _y, T _z, T _w) : x(_x), y(_x), z(_x), w(_w) {}
			explicit Quaternion(T s) : x(s), y(s), z(s), w(s) {}
			Quaternion(const Quaternion<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
			Quaternion(const Vector3<T>& axis, T angle)
			{
				SetFromAxisAngle(axis, angle);
			}

			void SetFromAxisAngle(const Vector3<T>& axis, T angle)
			{
				T halfAngle = angle / 2;
				T cosA = Cos(halfAngle);
				T sinA = Sin(halfAngle);
				x = axis.x * sinA;
				y = axis.y * sinA;
				z = axis.z * sinA;
				w = cosA;
			}

			Quaternion<T>& operator=(const Quaternion<T>& q) { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }
			Quaternion<T> operator*(const Quaternion<T>& other) const
			{
				Quaternion<T> result;
				result.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
				result.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
				result.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);
				result.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
				return result;
			}
			Quaternion<T> operator/(T s) const { return Quaternion<T>(x / s, y / s, z / s, w / s); }
			

			Matrix4<T> GetAsMatrix4() const
			{
				Matrix4<T> m(GetAsMatrix3());
			
				return m;
			}

			Matrix3<T> GetAsMatrix3() const
			{				
				T x2 = (T)(x * 2);
				T y2 = (T)(y * 2);
				T z2 = (T)(z * 2);

				const T _2xx = x2*x;
				const T _2yy = y2*y;
				const T _2zz = z2*z;
				const T _2xy = x2*y;
				const T _2xz = x2*z;
				const T _2xw = x2*w;
				const T _2yz = y2*z;
				const T _2yw = y2*w;
				const T _2zw = z2*w;

				Matrix3<T> ret;
				T *m = ret.m;

				m[0] = 1 - _2yy - _2zz;
				m[3] = _2xy + _2zw;
				m[6] = _2xz - _2yw;

				m[1] = _2xy - _2zw;
				m[4] = 1 - _2xx - _2zz;
				m[7] = _2yz + _2xw;

				m[2] = _2xz + _2yw;
				m[5] = _2yz - _2xw;
				m[8] = 1 - _2yy - _2xx;

				return ret;
			}

			Quaternion<T>& Invert()
			{
				x = -x;
				y = -y;
				z = -z;
				return *this;
			}

			Quaternion<T> GetInverted() const
			{
				return Quaternion<T>(-x, -y, -z, w);
			}

			T Norm() const
			{
				return x * x + y * y + z * z + w * w;
			}

			Vector3<T> operator*(const Vector3<T>& v) const
			{
				// nVidia SDK implementation				
				Vector3<T> uv, uuv;
				Vector3<T> qvec(x, y, z);
				uv = v.Cross(qvec);
				uuv = uv.Cross(qvec);
				uv *= (2 * w);
				uuv *= 2;

				return v + uv + uuv;
			}

			Quaternion<T>& SetIndentity()
			{
				x = (T)0;
				y = (T)0;
				z = (T)0;
				w = (T)1;
				return *this;
			}
		};
	}
}

#endif
