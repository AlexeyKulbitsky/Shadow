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

			void SetFromEulerXYZ(const Vector3<T>& angles)
			{
				Vector3<T> r(angles * T(0.5));
				Vector3<T> sr(Sin(r.x), Sin(r.y), Sin(r.z));
				Vector3<T> cr(Cos(r.x), Cos(r.y), Cos(r.z));
				
				x = cr.z * sr.x * cr.y + sr.z * cr.x * sr.y;
				y = cr.z * cr.x * sr.y - sr.z * sr.x * cr.y;
				z = sr.z * cr.x * cr.y + cr.z * sr.x * sr.y;
				w = cr.z * cr.x * cr.y - sr.z * sr.x * sr.y;
				
				//x = sr.x * cr.y * cr.z - cr.x * sr.y * sr.z;
				//y = cr.x * sr.y * cr.z + sr.x * cr.y * sr.z;
				//z = cr.x * cr.y * sr.z - sr.x * sr.y * cr.z;
				//w = cr.x * cr.y * cr.z + sr.x * sr.y * sr.z;			
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
			Quaternion<T>& operator*=(T s)
			{
				x *= s;
				y *= s;
				z *= s;
				w *= s;
				return *this;
			}

			Quaternion<T> operator/(T s) const { return Quaternion<T>(x / s, y / s, z / s, w / s); }
						
			void Normalize()
			{
				T n = x*x + y*y + z*z + w*w;
				assert(n != 0);
				n = 1 / (T)math::Sqrt(n);
				*this *= n;
			}

			void RotationFromTo(const Vector3<T>& a, const Vector3<T>& b)
			{
				Vector3<T> axis = a.Cross(b);
				T dot = a.Dot(b);
				if (dot < (T)(-1.0) + std::numeric_limits<T>::epsilon())// vectors are parallel and facing in the opposite direction
				{
					// Try crossing with x axis.
					Vector3<T> t = a.Cross(Vector3<T>(1, 0, 0));
					// If not ok, cross with y axis.
					if (t.GetLength() == 0)
					{
						t = a.Cross(Vector3<T>(0, 1, 0));
					}

					t.Normalize();
					x = t.x;
					y = t.y;
					z = t.z;
					w = 0;
				}

				// if vectors are parallel and are facing in the same direction
				//	the axis is zero and quaternion is the identity
				Quaternion<T> result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (dot + 1.0f) * 0.5f);
				x = axis.x * 0.5f;
				y = axis.y * 0.5f;
				z = axis.z * 0.5f;
				w = (dot + 1.0f) * 0.5f;
				Normalize();
			}

			Matrix4<T> GetAsMatrix4() const
			{
				Matrix4<T> m(GetAsMatrix3());
			
				return m;
			}

			Matrix3<T> GetAsMatrix3() const
			{				
				const T fTx = x + x;
				const T fTy = y + y;
				const T fTz = z + z;
				const T fTwx = fTx*w;
				const T fTwy = fTy*w;
				const T fTwz = fTz*w;
				const T fTxx = fTx*x;
				const T fTxy = fTy*x;
				const T fTxz = fTz*x;
				const T fTyy = fTy*y;
				const T fTyz = fTz*y;
				const T fTzz = fTz*z;

				Matrix3<T> r;

				r.m[0][0] = 1.0f - (fTyy + fTzz);
				r.m[0][1] = fTxy - fTwz;
				r.m[0][2] = fTxz + fTwy;
				r.m[1][0] = fTxy + fTwz;
				r.m[1][1] = 1.0f - (fTxx + fTzz);
				r.m[1][2] = fTyz - fTwx;
				r.m[2][0] = fTxz - fTwy;
				r.m[2][1] = fTyz + fTwx;
				r.m[2][2] = 1.0f - (fTxx + fTyy);

				return r;
			}

			template <typename T>
			void GetAsEulerXYZ(Vector3<T>& res) const
			{
				const T s = T(2) * (z * x + w * y);
				if (s < T(0.9999))
				{
					if (s > -T(0.9999))
					{
						res.y = Asin(s);
						res.x = Atan2(-T(2) * (z * y - w * x), T(1) - T(2) * (x * x + y * y));
						res.z = Atan2(-T(2) * (x * y - w * z), T(1) - T(2) * (z * z + y * y));
					}
					else
					{
						res.y = -(T)k_pi_2;
						res.x = -Atan2(T(2) * (x * y + w * z), T(1) - T(2) * (z * z + x * x));
						res.z = 0;
					}
				}
				else
				{
					res.y = (T)k_pi_2;
					res.x = Atan2(T(2) * (x * y + w * z), T(1) - T(2) * (z * z + x * x));
					res.z = 0;
				}
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
				uv = qvec.Cross(v);
				uuv = qvec.Cross(uv);
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
