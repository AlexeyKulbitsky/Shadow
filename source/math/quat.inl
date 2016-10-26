namespace sh
{
	namespace math
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// constructors
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline quat<T>::quat() : x(0), y(0), z(0), w(1) {}

		template <typename T>
		inline quat<T>::quat(ZUninitialized) {}

		template <typename T>
		inline quat<T>::quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

		template <typename T>
		inline quat<T>::quat(const quat<T>& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

		template <typename T>
		inline quat<T>::quat(const mat3<T>& mat)
		{
			setFromMat3(mat);
		}

		template <typename T>
		inline quat<T>::quat(const mat4<T>& mat)
		{
			setFromMat4(mat);
		}

		template <typename T>
		inline quat<T>::quat(ZAxisX, T angle) : y(0), z(0)
		{
			const T halfa = angle * T(0.5);
			math::sincos(halfa, x, w);
		}
		template <typename T>
		inline quat<T>::quat(ZAxisY, T angle) : x(0), z(0)
		{
			const T halfa = angle * T(0.5);
			math::sincos(halfa, y, w);
		}
		template <typename T>
		inline quat<T>::quat(ZAxisZ, T angle) : x(0), y(0)
		{
			const T halfa = angle * T(0.5);
			math::sincos(halfa, z, w);
		}
		template <typename T>
		inline quat<T>::quat(ZAngleAxis, T angle, const vec3<T>& axis)
		{
			setFromAngleAxis(angle, axis);
		}
		template <typename T>
		inline quat<T>::quat(ZEulerXYZ, const vec3<T>& angles)
		{
			setFromEulerXYZ(angles);
		}
		template <typename T>
		inline quat<T>::quat(ZEulerXZY, const vec3<T>& angles)
		{
			setFromEulerXZY(angles);
		}
		template <typename T>
		inline quat<T>::quat(ZEulerYXZ, const vec3<T>& angles)
		{
			setFromEulerYXZ(angles);
		}
		template <typename T>
		inline quat<T>::quat(ZEulerYZX, const vec3<T>& angles)
		{
			setFromEulerYZX(angles);
		}
		template <typename T>
		inline quat<T>::quat(ZEulerZXY, const vec3<T>& angles)
		{
			setFromEulerZXY(angles);
		}
		template <typename T>
		inline quat<T>::quat(ZEulerZYX, const vec3<T>& angles)
		{
			setFromEulerZYX(angles);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// member functions
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// sets new quat
		template <typename T>
		inline quat<T>& quat<T>::set(T _x, T _y, T _z, T _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
			return *this;
		}

		template <typename T>
		inline quat<T>& quat<T>::normalize()
		{
			T n = x*x + y*y + z*z + w*w;
			assert(n != 0);
			n = 1 / (T)math::sqrt(n);
			return (*this *= n);
		}

		template <typename T>
		inline T quat<T>::getLength() const
		{
			return math::sqrt(getLengthSq());
		}

		template <typename T>
		inline T quat<T>::getLengthSq() const
		{
			return x*x + y*y + z*z + w*w;
		}

		template <typename T>
		inline quat<T>& quat<T>::invert()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		template <typename T>
		inline quat<T>& quat<T>::setIdentity()
		{
			x = (T)0;
			y = (T)0;
			z = (T)0;
			w = (T)1;
			return *this;
		}

		template<typename T>
		inline quat<T>& quat<T>::setFromMat3(const mat3<T>& mat)
		{
			T trace = mat(0, 0) + mat(1, 1) + mat(2, 2);
			if (trace > 0)
			{
				// |w| > 1/2, may as well choose w > 1/2
				T fRoot = math::sqrt(trace + 1);  // 2w
				w = T(0.5)*fRoot;
				fRoot = T(0.5) / fRoot;  // 1/(4w)
				x = (mat(2, 1) - mat(1, 2))*fRoot;
				y = (mat(0, 2) - mat(2, 0))*fRoot;
				z = (mat(1, 0) - mat(0, 1))*fRoot;
			}
			else
			{
				// |w| <= 1/2
				int i = 0;
				if (mat(1, 1) > mat(0, 0))
					i = 1;
				if (mat(2, 2) > mat(i, i))
					i = 2;
				int j = (i + 1) % 3;
				int k = (j + 1) % 3;

				T fRoot = math::sqrt(mat(i, i) - mat(j, j) - mat(k, k) + 1);
				T* apfQuat[3] = { &x, &y, &z };
				*apfQuat[i] = T(0.5)*fRoot;
				fRoot = T(0.5) / fRoot;
				w = (mat(k, j) - mat(j, k))*fRoot;
				*apfQuat[j] = (mat(j, i) + mat(i, j))*fRoot;
				*apfQuat[k] = (mat(k, i) + mat(i, k))*fRoot;
			}

			return *this;
		}

		template <typename T>
		inline mat3<T> quat<T>::getAsMat3() const
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

			mat3<T> ret(mat3<T>::uninitialized);
			T *m = ret;

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

		template <typename T>
		inline quat<T>& quat<T>::setFromMat4(const mat4<T>& m)
		{
			return setFromMat3(mat3<T>(m));
		}

		template <typename T>
		inline mat4<T> quat<T>::getAsMat4() const
		{
			return mat4<T>(getAsMat3());
		}

		template <typename T>
		inline quat<T>& quat<T>::setFromEulerXYZ(const vec3<T>& angles)
		{
			vec3<T> r(angles*T(0.5));
			vec3<T> sr, cr;
			math::sincos(r, sr, cr);
			x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
			y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
			z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
			w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
			return *this;
		}
		template <typename T>
		inline quat<T>& quat<T>::setFromEulerXZY(const vec3<T>& angles)
		{
			vec3<T> r(angles*T(0.5));
			vec3<T> sr, cr;
			math::sincos(r, sr, cr);
			x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
			y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
			z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
			w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
			return *this;
		}
		template <typename T>
		inline quat<T>& quat<T>::setFromEulerYXZ(const vec3<T>& angles)
		{
			vec3<T> r(angles*T(0.5));
			vec3<T> sr, cr;
			math::sincos(r, sr, cr);
			x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
			y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
			z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
			w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
			return *this;
		}
		template <typename T>
		inline quat<T>& quat<T>::setFromEulerYZX(const vec3<T>& angles)
		{
			vec3<T> r(angles*T(0.5));
			vec3<T> sr, cr;
			math::sincos(r, sr, cr);
			x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
			y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
			z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
			w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
			return *this;
		}
		template <typename T>
		inline quat<T>& quat<T>::setFromEulerZXY(const vec3<T>& angles)
		{
			vec3<T> r(angles*T(0.5));
			vec3<T> sr, cr;
			math::sincos(r, sr, cr);
			x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
			y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
			z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
			w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
			return *this;
		}
		template <typename T>
		inline quat<T>& quat<T>::setFromEulerZYX(const vec3<T>& angles)
		{
			vec3<T> r(angles*T(0.5));
			vec3<T> sr, cr;
			math::sincos(r, sr, cr);
			x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
			y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
			z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
			w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
			return *this;
		}

		template <typename T>
		inline vec3<T> quat<T>::getAsEulerXYZ() const
		{
			vec3<T> res;
			const T s = T(2)*(z*x + w*y);
			if (s < T(0.9999))
			{
				if (s > -T(0.9999))
				{
					res.y = math::asin(s);
					res.x = math::atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(x*x + y*y));
					res.z = math::atan2(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + y*y));
				}
				else
				{
					res.y = -(T)k_pi2;
					res.x = -math::atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
					res.z = 0;
				}
			}
			else
			{
				res.y = (T)k_pi2;
				res.x = math::atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
				res.z = 0;
			}
			return res;
		}
		template <typename T>
		inline vec3<T> quat<T>::getAsEulerXZY() const
		{
			vec3<T> res;
			const T s = -T(2)*(x*y - w*z);
			if (s < T(0.9999))
			{
				if (s > -T(0.9999))
				{
					res.y = math::atan2(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
					res.x = math::atan2(T(2)*(z*y + w*x), T(1) - T(2)*(z*z + x*x));
					res.z = math::asin(s);
				}
				else
				{
					res.y = -math::atan2(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
					res.x = 0;
					res.z = -(T)k_pi2;
				}
			}
			else
			{
				res.y = -math::atan2(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
				res.x = 0;
				res.z = (T)k_pi2;
			}
			return res;
		}
		template <typename T>
		inline vec3<T> quat<T>::getAsEulerYXZ() const
		{
			vec3<T> res;
			const T s = -T(2)*(z*y - w*x);
			if (s < T(0.9999))
			{
				if (s > -T(0.9999))
				{
					res.y = math::atan2(T(2)*(z*x + w*y), T(1) - T(2)*(x*x + y*y));
					res.x = math::asin(s);
					res.z = math::atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
				}
				else
				{
					res.y = 0;
					res.x = -(T)k_pi2;
					res.z = math::atan2(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
				}
			}
			else
			{
				res.y = 0;
				res.x = (T)k_pi2;
				res.z = math::atan2(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
			}
			return res;
		}
		template <typename T>
		inline vec3<T> quat<T>::getAsEulerYZX() const
		{
			vec3<T> res;
			const T s = T(2)*(x*y + w*z);
			if (s < T(0.9999))
			{
				if (s > -T(0.9999))
				{
					res.y = math::atan2(-T(2)*(z*x - w*y), T(1) - T(2)*(z*z + y*y));
					res.x = math::atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
					res.z = math::asin(s);
				}
				else
				{
					res.y = -math::atan2(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
					res.x = 0;
					res.z = -(T)k_pi2;
				}
			}
			else
			{
				res.y = math::atan2(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
				res.x = 0;
				res.z = (T)k_pi2;
			}
			return res;
		}

		template <typename T>
		inline vec3<T> quat<T>::getAsEulerZXY() const
		{
			vec3<T> res;
			const T s = T(2)*(z*y + w*x);
			if (s < T(0.9999))
			{
				if (s > -T(0.9999))
				{
					res.y = math::atan2(-T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
					res.x = math::asin(s);
					res.z = math::atan2(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + x*x));
				}
				else
				{
					res.y = 0;
					res.x = -(T)k_pi2;
					res.z = -math::atan2(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
				}
			}
			else
			{
				res.y = 0;
				res.x = (T)k_pi2;
				res.z = math::atan2(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
			}
			return res;
		}

		template <typename T>
		inline vec3<T> quat<T>::getAsEulerZYX() const
		{
			vec3<T> res;
			const T s = -T(2)*(z*x - w*y);
			if (s < T(0.9999))
			{
				if (s > -T(0.9999))
				{
					res.y = math::asin(s);
					res.x = math::atan2(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
					res.z = math::atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + y*y));
				}
				else
				{
					res.y = -(T)k_pi2;
					res.x = 0;
					res.z = math::atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
				}
			}
			else
			{
				res.y = (T)k_pi2;
				res.x = 0;
				res.z = -math::atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
			}
			return res;
		}

		//! axis must be unit length
		//! angle in radians
		template <typename T>
		inline quat<T>& quat<T>::setFromAngleAxis(T angle, const vec3<T>& axis)
		{
			const T fHalfAngle = T(0.5)*angle;
			const T fSin = math::sin(fHalfAngle);
			x = fSin*axis.x;
			y = fSin*axis.y;
			z = fSin*axis.z;
			w = math::cos(fHalfAngle);
			return *this;
		}

		template <typename T>
		inline void quat<T>::getAsAngleAxis(T& angle, vec3<T> &axis) const
		{
			const T safeW = math::clamp(w, (T)-1, (T)1);
			const T scale = math::sqrt((T)1 - safeW * safeW);
			angle = 2 * math::acos(safeW);
			if (scale < std::numeric_limits<T>::epsilon())
			{
				axis.x = x;
				axis.y = y;
				axis.z = z;
			}
			else
			{
				const T invscale = 1 / scale;
				axis.x = x * invscale;
				axis.y = y * invscale;
				axis.z = z * invscale;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// operators
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T>
		inline bool quat<T>::operator==(const quat<T>& v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}

		template<typename T>
		inline bool quat<T>::operator!=(const quat<T>& v) const
		{
			return !operator==(v);
		}

		template <typename T>
		inline quat<T>& quat<T>::operator=(const quat<T>& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		template <typename T>
		inline quat<T> quat<T>::operator+(const quat<T>& b) const
		{
			return quat<T>(x + b.x, y + b.y, z + b.z, w + b.w);
		}

		template <typename T>
		inline quat<T> quat<T>::operator-(const quat<T>& b) const
		{
			return quat<T>(x - b.x, y - b.y, z - b.z, w - b.w);
		}


		template <typename T>
		inline quat<T> quat<T>::operator*(const quat<T>& other) const
		{
			quat<T> tmp(uninitialized);

			tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
			tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
			tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);
			tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);

			return tmp;
		}

		template <typename T>
		inline quat<T> quat<T>::operator*(T s) const
		{
			return quat<T>(s*x, s*y, s*z, s*w);
		}

		template <typename T>
		inline quat<T>& quat<T>::operator*=(T s)
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		template <typename T>
		inline quat<T>& quat<T>::operator*=(const quat<T>& other)
		{
			return (*this = (*this) * other);
		}

		template <typename T>
		inline quat<T> quat<T>::operator-() const
		{
			return quat<T>(-x, -y, -z, -w);
		}

		template <typename T>
		inline quat<T> quat<T>::operator~() const
		{
			return quat<T>(-x, -y, -z, +w);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// global methods
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		inline T dot(const quat<T>& q1, const quat<T>& q2)
		{
			return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
		}


		template <typename T, typename U>
		vec3<U> operator*(const quat<T>& q, const vec3<U>& v)
		{
			// nVidia SDK implementation

			vec3<U> uv, uuv;
			vec3<U> qvec(q.x, q.y, q.z);
			uv = cross(v, qvec);
			uuv = cross(uv, qvec);
			uv *= (2 * q.w);
			uuv *= 2;

			return v + uv + uuv;
		}


	}
}