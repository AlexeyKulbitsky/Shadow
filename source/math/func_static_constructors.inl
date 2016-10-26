namespace sh
{
	namespace math
	{
		///! Creates a world space lookat matrix (front axis is -yaxis, up axis is zaxis)
		/// This lookAt method computes a look at matrix in jet coordinate system (3dmax biped). 
		/// This means that when you send a front of (0,-1,0) and an up of (0,0,1) the resulting matrix is identity.
		template<class T>
		inline mat3<T> lookAt(const vec3<T>& front, const vec3<T>& up)
		{
			vec3<T> axisY = -normalized(front);
			vec3<T> axisX = cross(axisY, normalized(up));
			axisX.normalize(); //this normalize is mandatory because axisY and up may be unitary but they hardly are orthogonal
			vec3<T> axisZ = cross(axisX, axisY);

			mat3<T> mat;
			T* m = mat.m;
			m[0] = axisX.x;
			m[1] = axisX.y;
			m[2] = axisX.z;

			m[3] = axisY.x;
			m[4] = axisY.y;
			m[5] = axisY.z;

			m[6] = axisZ.x;
			m[7] = axisZ.y;
			m[8] = axisZ.z;

			return mat;
		}

		template<class T>
		inline quat<T> quatLookAt(const vec3<T>& front, const vec3<T>& up)
		{
			mat3<T> mat = lookAt(front, up);
			return quat<T>(mat);
		}

		template<typename T>
		inline quat<T> quatAxisX(T angle)
		{
			return quat<T>(quat<T>::axisX, angle);
		}

		template<typename T>
		inline quat<T> quatAxisY(T angle)
		{
			return quat<T>(quat<T>::axisY, angle);
		}

		template<typename T>
		inline quat<T> quatAxisZ(T angle)
		{
			return quat<T>(quat<T>::axisZ, angle);
		}

		template<typename T>
		inline quat<T> quatZXY(T rotZ, T rotX, T rotY)
		{
			MDEPRECATED("WILL BE REMOVED SOON. USE THE QUAT CONSTRUCTOR");
			const T x = T(0.5) * rotX;
			const T y = T(0.5) * rotY;
			const T z = T(0.5) * rotZ;
			T sx, cx, sy, cy, sz, cz;
			math::sincos(x, sx, cx);
			math::sincos(y, sy, cy);
			math::sincos(z, sz, cz);

			return quat<T>(
				cz * sx * cy + sz * cx * sy,
				cz * cx * sy - sz * sx * cy,
				sz * cx * cy - cz * sx * sy,
				cz * cx * cy + sz * sx * sy
				);
		}

		template <typename T>
		void quatZXY(const quat<T>& q, T& outZ, T& outX, T& outY)
		{
			MDEPRECATED("WILL BE REMOVED SOON. USE THE QUAT CONSTRUCTOR");
			T x = q.x;
			T y = q.y;
			T z = q.z;
			T w = q.w;

			const T s = -2 * (z * y - w * x);
			if (s < 1)
			{
				if (s > -1)
				{
					outY = math::atan2(2 * (z * x + w * y), 1 - 2 * (x * x + y * y));
					outX = math::asin(s);
					outZ = math::atan2(2 * (x * y + w * z), 1 - 2 * (z * z + x * x));
				}
				else
				{
					outY = -math::atan2(-2 * (z * x - w * y), 2 * (z * y + w * x));
					outX = -(T)k_pi2;
					outZ = 0;
				}
			}
			else
			{
				outY = math::atan2(-2 * (z * x - w * y), 2 * (z * y + w * x));
				outX = (T)k_pi2;
				outZ = 0;
			}
		}

		template <typename T>
		quat<T> quatRotationFromTo(const vec3<T>& a, const vec3<T>& b)
		{
			vec3<T> axis = cross(a, b);
			T dot = math::dot(a, b);
			if (dot < (T)(-1.0) + std::numeric_limits<T>::epsilon())// vectors are parallel and facing in the opposite direction
			{
				// Try crossing with x axis.
				vec3<T> t = cross(a, vec3<T>(1, 0, 0));
				// If not ok, cross with y axis.
				if (isZero(t.getLength()))
				{
					t = cross(a, vec3<T>(0, 1, 0));
				}

				t.normalize();
				return quat<T>(t[0], t[1], t[2], 0);
			}

			// if vectors are parallel and are facing in the same direction
			//	the axis is zero and quaternion is the identity
			quat<T> result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (dot + 1.0f) * 0.5f);
			result.normalize();
			return result;
		}


	}
}