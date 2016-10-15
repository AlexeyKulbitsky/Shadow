#ifndef SHADOW_VEC3_INCLUDE
#define SHADOW_VEC3_INCLUDE

#include "shMath.h"

namespace sh
{
	namespace math
	{
		template <class T>
		class vec3
		{
		public:
			
			// constructors
			vec3() : x(0), y(0), z(0) {}			
			vec3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}			
			explicit vec3(T n) : x(n), y(n), z(n) {}			
			vec3(const vec3<T>& other) : x(other.x), y(other.y), z(other.z) {}

			// operators
			vec3<T> operator-() const { return vec3<T>(-x, -y, -z); }

			vec3<T>& operator=(const vec3<T>& other) { x = other.x; y = other.y; z = other.z; return *this; }

			vec3<T> operator+(const vec3<T>& other) const { return vec3<T>(x + other.x, y + other.y, z + other.z); }
			vec3<T>& operator+=(const vec3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
			vec3<T> operator+(const T val) const { return vec3<T>(x + val, y + val, z + val); }
			vec3<T>& operator+=(const T val) { x += val; y += val; z += val; return *this; }

			vec3<T> operator-(const vec3<T>& other) const { return vec3<T>(x - other.x, y - other.y, z - other.z); }
			vec3<T>& operator-=(const vec3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
			vec3<T> operator-(const T val) const { return vec3<T>(x - val, y - val, z - val); }
			vec3<T>& operator-=(const T val) { x -= val; y -= val; z -= val; return *this; }

			vec3<T> operator*(const vec3<T>& other) const { return vec3<T>(x * other.x, y * other.y, z * other.z); }
			vec3<T>& operator*=(const vec3<T>& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
			vec3<T> operator*(const T v) const { return vec3<T>(x * v, y * v, z * v); }
			vec3<T>& operator*=(const T v) { x *= v; y *= v; z *= v; return *this; }

			vec3<T> operator/(const vec3<T>& other) const { return vec3<T>(x / other.x, y / other.y, z / other.z); }
			vec3<T>& operator/=(const vec3<T>& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
			vec3<T> operator/(const T v) const { T i = (T)1.0 / v; return vec3<T>(x * i, y * i, z * i); }
			vec3<T>& operator/=(const T v) { T i = (T)1.0 / v; x *= i; y *= i; z *= i; return *this; }

			//! use weak float compare
			bool operator==(const vec3<T>& other) const
			{
				return x == other.x && y == other.y && z == other.z;
			}

			bool operator!=(const vec3<T>& other) const
			{
				return !(*this == other);
			}

			// functions

			vec3<T>& set(const T nx, const T ny, const T nz) { x = nx; y = ny; z = nz; return *this; }
			vec3<T>& set(const vec3<T>& p) { x = p.x; y = p.y; z = p.z; return *this; }

			//! Get length of the vector.
			T getLength() const { return sqrt(x*x + y*y + z*z); }

			//! Get squared length of the vector.
			/** This is useful because it is much faster than getLength().
			\return Squared length of the vector. */
			T getLengthSQ() const { return x*x + y*y + z*z; }

			//! Get the dot product with another vector.
			T dotProduct(const vec3<T>& other) const
			{
				return x*other.x + y*other.y + z*other.z;
			}

			//! Get distance from another point.
			/** Here, the vector is interpreted as point in 3 dimensional space. */
			T getDistanceFrom(const vec3<T>& other) const
			{
				return vec3<T>(x - other.x, y - other.y, z - other.z).getLength();
			}

			//! Returns squared distance from another point.
			/** Here, the vector is interpreted as point in 3 dimensional space. */
			T getDistanceFromSQ(const vec3<T>& other) const
			{
				return vec3<T>(x - other.x, y - other.y, z - other.z).getLengthSQ();
			}

			//! Calculates the cross product with another vector.
			/** \param p Vector to multiply with.
			\return Crossproduct of this vector with p. */
			vec3<T> crossProduct(const vec3<T>& p) const
			{
				return vec3<T>(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
			}

			//! Returns if this vector interpreted as a point is on a line between two other points.
			/** It is assumed that the point is on the line.
			\param begin Beginning vector to compare between.
			\param end Ending vector to compare between.
			\return True if this vector is between begin and end, false if not. */
			bool isBetweenPoints(const vec3<T>& begin, const vec3<T>& end) const
			{
				const T f = (end - begin).getLengthSQ();
				return getDistanceFromSQ(begin) <= f &&
					getDistanceFromSQ(end) <= f;
			}

			//! Normalizes the vector.
			/** In case of the 0 vector the result is still 0, otherwise
			the length of the vector will be 1.
			\return Reference to this vector after normalization. */
			vec3<T>& normalize()
			{
				f64 length = x*x + y*y + z*z;
				if (length == 0) // this check isn't an optimization but prevents getting NAN in the sqrt.
					return *this;
				length = reciprocal_squareroot(length);

				x = (T)(x * length);
				y = (T)(y * length);
				z = (T)(z * length);
				return *this;
			}

			//! Sets the length of the vector to a new value
			vec3<T>& setLength(T newlength)
			{
				normalize();
				return (*this *= newlength);
			}

			//! Inverts the vector.
			vec3<T>& invert()
			{
				x *= -1;
				y *= -1;
				z *= -1;
				return *this;
			}

			//! Rotates the vector by a specified number of degrees around the y axis and the specified center.
			/** \param degrees Number of degrees to rotate around the y axis.
			\param center The center of the rotation. */
			void rotatexzBy(f64 degrees, const vec3<T>& center = vec3<T>())
			{
				degrees *= DEGTORAD64;
				f64 cs = cos(degrees);
				f64 sn = sin(degrees);
				x -= center.x;
				z -= center.z;
				set((T)(x*cs - z*sn), y, (T)(x*sn + z*cs));
				x += center.x;
				z += center.z;
			}

			//! Rotates the vector by a specified number of degrees around the z axis and the specified center.
			/** \param degrees: Number of degrees to rotate around the z axis.
			\param center: The center of the rotation. */
			void rotatexyBy(f64 degrees, const vec3<T>& center = vec3<T>())
			{
				degrees *= DEGTORAD64;
				f64 cs = cos(degrees);
				f64 sn = sin(degrees);
				x -= center.x;
				y -= center.y;
				set((T)(x*cs - y*sn), (T)(x*sn + y*cs), z);
				x += center.x;
				y += center.y;
			}

			//! Rotates the vector by a specified number of degrees around the x axis and the specified center.
			/** \param degrees: Number of degrees to rotate around the x axis.
			\param center: The center of the rotation. */
			void rotateyzBy(f64 degrees, const vec3<T>& center = vec3<T>())
			{
				degrees *= DEGTORAD64;
				f64 cs = cos(degrees);
				f64 sn = sin(degrees);
				z -= center.z;
				y -= center.y;
				set(x, (T)(y*cs - z*sn), (T)(y*sn + z*cs));
				z += center.z;
				y += center.y;
			}

			//! Creates an interpolated vector between this vector and another vector.
			/** \param other The other vector to interpolate with.
			\param d Interpolation value between 0.0f (all the other vector) and 1.0f (all this vector).
			Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
			\return An interpolated vector.  This vector is not modified. */
			vec3<T> getInterpolated(const vec3<T>& other, f64 d) const
			{
				const f64 inv = 1.0 - d;
				return vec3<T>((T)(other.x*inv + x*d), (T)(other.y*inv + y*d), (T)(other.z*inv + z*d));
			}

			//! Creates a quadratically interpolated vector between this and two other vectors.
			/** \param v2 Second vector to interpolate with.
			\param v3 Third vector to interpolate with (maximum at 1.0f)
			\param d Interpolation value between 0.0f (all this vector) and 1.0f (all the 3rd vector).
			Note that this is the opposite direction of interpolation to getInterpolated() and interpolate()
			\return An interpolated vector. This vector is not modified. */
			vec3<T> getInterpolated_quadratic(const vec3<T>& v2, const vec3<T>& v3, f64 d) const
			{
				// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
				const f64 inv = (T) 1.0 - d;
				const f64 mul0 = inv * inv;
				const f64 mul1 = (T) 2.0 * d * inv;
				const f64 mul2 = d * d;

				return vec3<T>((T)(x * mul0 + v2.x * mul1 + v3.x * mul2),
					(T)(y * mul0 + v2.y * mul1 + v3.y * mul2),
					(T)(z * mul0 + v2.z * mul1 + v3.z * mul2));
			}

			//! Sets this vector to the linearly interpolated vector between a and b.
			/** \param a first vector to interpolate with, maximum at 1.0f
			\param b second vector to interpolate with, maximum at 0.0f
			\param d Interpolation value between 0.0f (all vector b) and 1.0f (all vector a)
			Note that this is the opposite direction of interpolation to getInterpolated_quadratic()
			*/
			vec3<T>& interpolate(const vec3<T>& a, const vec3<T>& b, f64 d)
			{
				x = (T)((f64)b.x + ((a.x - b.x) * d));
				y = (T)((f64)b.y + ((a.y - b.y) * d));
				z = (T)((f64)b.z + ((a.z - b.z) * d));
				return *this;
			}


			//! Get the rotations that would make a (0,0,1) direction vector point in the same direction as this direction vector.
			/** Thanks to Arras on the Irrlicht forums for this method.  This utility method is very useful for
			orienting scene nodes towards specific targets.  For example, if this vector represents the difference
			between two scene nodes, then applying the result of getHorizontalAngle() to one scene node will point
			it at the other one.
			Example code:
			// Where target and seeker are of type ISceneNode*
			const vector3df toTarget(target->getAbsolutePosition() - seeker->getAbsolutePosition());
			const vector3df requiredRotation = toTarget.getHorizontalAngle();
			seeker->setRotation(requiredRotation);

			\return A rotation vector containing the x (pitch) and y (raw) rotations (in degrees) that when applied to a
			+z (e.g. 0, 0, 1) direction vector would make it point in the same direction as this vector. The z (roll) rotation
			is always 0, since two Euler rotations are sufficient to point in any given direction. */
			vec3<T> getHorizontalAngle() const
			{
				vec3<T> angle;

				// tmp avoids some precision troubles on some compilers when working with T=s32
				f64 tmp = (atan2((f64)x, (f64)z) * RADTODEG64);
				angle.y = (T)tmp;

				if (angle.y < 0)
					angle.y += 360;
				if (angle.y >= 360)
					angle.y -= 360;

				const f64 z1 = core::squareroot(x*x + z*z);

				tmp = (atan2((f64)z1, (f64)y) * RADTODEG64 - 90.0);
				angle.x = (T)tmp;

				if (angle.x < 0)
					angle.x += 360;
				if (angle.x >= 360)
					angle.x -= 360;

				return angle;
			}

			//! Get the spherical coordinate angles
			/** This returns Euler degrees for the point represented by
			this vector.  The calculation assumes the pole at (0,1,0) and
			returns the angles in x and y.
			*/
			vec3<T> getSphericalCoordinateAngles() const
			{
				vec3<T> angle;
				const f64 length = x*x + y*y + z*z;

				if (length)
				{
					if (x != 0)
					{
						angle.y = (T)(atan2((f64)z, (f64)x) * RADTODEG64);
					}
					else if (z<0)
						angle.y = 180;

					angle.x = (T)(acos(y * core::reciprocal_squareroot(length)) * RADTODEG64);
				}
				return angle;
			}

			//! Builds a direction vector from (this) rotation vector.
			/** This vector is assumed to be a rotation vector composed of 3 Euler angle rotations, in degrees.
			The implementation performs the same calculations as using a matrix to do the rotation.

			\param[in] forwards  The direction representing "forwards" which will be rotated by this vector.
			If you do not provide a direction, then the +z axis (0, 0, 1) will be assumed to be forwards.
			\return A direction vector calculated by rotating the forwards direction by the 3 Euler angles
			(in degrees) represented by this vector. */
			vec3<T> rotationToDirection(const vec3<T> & forwards = vec3<T>(0, 0, 1)) const
			{
				const f64 cr = cos(core::DEGTORAD64 * x);
				const f64 sr = sin(core::DEGTORAD64 * x);
				const f64 cp = cos(core::DEGTORAD64 * y);
				const f64 sp = sin(core::DEGTORAD64 * y);
				const f64 cy = cos(core::DEGTORAD64 * z);
				const f64 sy = sin(core::DEGTORAD64 * z);

				const f64 srsp = sr*sp;
				const f64 crsp = cr*sp;

				const f64 pseudoMatrix[] = {
					(cp*cy), (cp*sy), (-sp),
					(srsp*cy - cr*sy), (srsp*sy + cr*cy), (sr*cp),
					(crsp*cy + sr*sy), (crsp*sy - sr*cy), (cr*cp) };

				return vec3<T>(
					(T)(forwards.x * pseudoMatrix[0] +
						forwards.y * pseudoMatrix[3] +
						forwards.z * pseudoMatrix[6]),
						(T)(forwards.x * pseudoMatrix[1] +
							forwards.y * pseudoMatrix[4] +
							forwards.z * pseudoMatrix[7]),
							(T)(forwards.x * pseudoMatrix[2] +
								forwards.y * pseudoMatrix[5] +
								forwards.z * pseudoMatrix[8]));
			}

			//! Fills an array of 4 values with the vector data (usually floats).
			/** Useful for setting in shader constants for example. The fourth value
			will always be 0. */
			void getAs4Values(T* array) const
			{
				array[0] = x;
				array[1] = y;
				array[2] = z;
				array[3] = 0;
			}

			//! Fills an array of 3 values with the vector data (usually floats).
			/** Useful for setting in shader constants for example.*/
			void getAs3Values(T* array) const
			{
				array[0] = x;
				array[1] = y;
				array[2] = z;
			}


			//! x coordinate of the vector
			T x;

			//! y coordinate of the vector
			T y;

			//! z coordinate of the vector
			T z;
		};

		//! partial specialization for integer vectors
		// Implementor note: inline keyword needed due to template specialization for s32. Otherwise put specialization into a .cpp
		template <>
		inline vec3<s32> vec3<s32>::operator /(s32 val) const { return vec3<s32>(x / val, y / val, z / val); }
		template <>
		inline vec3<s32>& vec3<s32>::operator /=(s32 val) { x /= val; y /= val; z /= val; return *this; }

		template <>
		inline vec3<s32> vec3<s32>::getSphericalCoordinateAngles() const
		{
			vec3<s32> angle;
			const f64 length = x*x + y*y + z*z;

			if (length)
			{
				if (x != 0)
				{
					angle.y = round32((f32)(atan2((f64)z, (f64)x) * RADTODEG64));
				}
				else if (z<0)
					angle.y = 180;

				angle.x = round32((f32)(acos(y * reciprocal_squareroot(length)) * RADTODEG64));
			}
			return angle;
		}

		//! Typedef for a f32 3d vector.
		typedef vec3<f32> vec3f;

		//! Typedef for an integer 3d vector.
		typedef vec3<s32> vec3i;

		//! Function multiplying a scalar and a vector component-wise.
		template<class S, class T>
		vec3<T> operator*(const S scalar, const vec3<T>& vector) { return vector*scalar; }
	}
}

#endif
