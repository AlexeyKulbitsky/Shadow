namespace sh
{
	namespace math
	{
		///! Creates a RH orthographic projection matrix centered at the origin
		template<typename T>
		inline mat4<T> orthoCenter(T width, T height, T zNear, T zFar)
		{
			mat4<T> mat;
			T* M = mat.m;

			M[0] = (T)(2 / width);
			M[1] = 0;
			M[2] = 0;
			M[3] = 0;

			M[4] = 0;
			M[5] = (T)(2 / height);
			M[6] = 0;
			M[7] = 0;

			M[8] = 0;
			M[9] = 0;
			M[10] = (T)(-2) / (zFar - zNear);
			M[11] = 0;

			M[12] = 0;
			M[13] = 0;
			M[14] = -(zFar + zNear) / (zFar - zNear);
			M[15] = 1;
			return mat;
		}

		///! Creates a RH orthographic projection matrix
		template<typename T>
		inline mat4<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
		{
			mat4<T> mat;
			T* M = mat.m;
			M[0] = 2 / (right - left);
			M[1] = 0;
			M[2] = 0;
			M[3] = 0;

			M[4] = 0;
			M[5] = 2 / (top - bottom);
			M[6] = 0;
			M[7] = 0;

			M[8] = 0;
			M[9] = 0;
			M[10] = (T)(-2) / (zFar - zNear);
			M[11] = 0;

			M[12] = -(right + left) / (right - left);
			M[13] = -(top + bottom) / (top - bottom);
			M[14] = -(zFar + zNear) / (zFar - zNear);
			M[15] = 1;
			return mat;
		}

		///! Creates a RH perspective projection matrix
		template<typename T>
		inline mat4<T> perspective(T fovRadians, T aspectRatio, T zNear, T zFar)
		{
			const T yFac = math::tan(fovRadians / 2);
			const T xFac = yFac*aspectRatio;
			mat4<T> mat;
			T* M = mat.m;

			M[0] = 1 / xFac;
			M[1] = 0;
			M[2] = 0;
			M[3] = 0;

			M[4] = 0;
			M[5] = 1 / yFac;
			M[6] = 0;
			M[7] = 0;

			M[8] = 0;
			M[9] = 0;
			M[10] = -(zFar + zNear) / (zFar - zNear);
			M[11] = -1;

			M[12] = 0;
			M[13] = 0;
			M[14] = -(2 * zFar*zNear) / (zFar - zNear);
			M[15] = 0;
			return mat;
		}

		///! Creates a RH asymmetric frustum projection matrix
		template<typename T>
		inline mat4<T> perspective(T left, T right, T bottom, T top, T zNear, T zFar)
		{
			// glFrustum call (column order)
			//
			// |    2n/(r-l)       0         (r+l)/(r-l)     0       |
			// |      0          2n/(t-b)    (t+b)/(t-b)     0       |
			// |      0            0         (n+f)/(n-f) (2nf)/(n-f) |
			// |      0            0              -1         0       |
			//
			T diffX = 1 / (right - left);
			T diffY = 1 / (top - bottom);
			T diffZ = 1 / (zNear - zFar);

			mat4<T> mat;
			T* m = mat.m;
			m[0] = 2 * zNear*diffX;
			m[1] = 0;
			m[2] = 0;
			m[3] = 0;

			m[4] = 0;
			m[5] = 2 * zNear*diffY;
			m[6] = 0;
			m[7] = 0;

			m[8] = (right + left)*diffX;
			m[9] = (top + bottom)*diffY;
			m[10] = (zFar + zNear)*diffZ;
			m[11] = -1;

			m[12] = 0;
			m[13] = 0;
			m[14] = 2 * zNear*zFar*diffZ;
			m[15] = 0;

			return mat;
		}

	}
}