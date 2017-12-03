#include "Quaternion.h"


namespace sh
{

namespace math
{

	Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion::Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Quaternion::Quaternion(float s) : x(s), y(s), z(s), w(s) {}
	Quaternion::Quaternion(const Quaternion& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	Quaternion::Quaternion(const Vector3& axis, float angle)
	{
		SetFromAxisAngle(axis, angle);
	}

	void Quaternion::SetFromAxisAngle(const Vector3& axis, float angle)
	{
		float halfAngle = angle / 2.0f;
		float cosA = Cos(halfAngle);
		float sinA = Sin(halfAngle);
		x = axis.x * sinA;
		y = axis.y * sinA;
		z = axis.z * sinA;
		w = cosA;
	}

	void Quaternion::SetFromEulerXYZ(const Vector3& angles)
	{
		Vector3 r(angles * 0.5f);
		Vector3 sr(Sin(r.x), Sin(r.y), Sin(r.z));
		Vector3 cr(Cos(r.x), Cos(r.y), Cos(r.z));

		x = cr.z * sr.x * cr.y + sr.z * cr.x * sr.y;
		y = cr.z * cr.x * sr.y - sr.z * sr.x * cr.y;
		z = sr.z * cr.x * cr.y + cr.z * sr.x * sr.y;
		w = cr.z * cr.x * cr.y - sr.z * sr.x * sr.y;

		//x = sr.x * cr.y * cr.z - cr.x * sr.y * sr.z;
		//y = cr.x * sr.y * cr.z + sr.x * cr.y * sr.z;
		//z = cr.x * cr.y * sr.z - sr.x * sr.y * cr.z;
		//w = cr.x * cr.y * cr.z + sr.x * sr.y * sr.z;			
	}

	Quaternion& Quaternion::operator=(const Quaternion& q) { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }
	Quaternion Quaternion::operator*(const Quaternion& other) const
	{
		Quaternion result;
		result.x = (other.w * x) + (other.x * w) + (other.z * y) - (other.y * z);
		result.y = (other.w * y) + (other.y * w) + (other.x * z) - (other.z * x);
		result.z = (other.w * z) + (other.z * w) + (other.y * x) - (other.x * y);
		result.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
		return result;
	}
	Quaternion& Quaternion::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	Quaternion Quaternion::operator/(float s) const { return Quaternion(x / s, y / s, z / s, w / s); }

	void Quaternion::Normalize()
	{
		float n = x*x + y*y + z*z + w*w;
		//assert(n != 0);
		n = 1.0f / Sqrt(n);
		*this *= n;
	}

	void Quaternion::RotationFromTo(const Vector3& a, const Vector3& b)
	{
		Vector3 axis = a.Cross(b);
		float dot = a.Dot(b);
		//if (dot < (T)(-1.0) + std::numeric_limits<T>::epsilon())// vectors are parallel and facing in the opposite direction
		if (dot < -1.0f + 0.00001f)
		{
			// Try crossing with x axis.
			Vector3 t = a.Cross(Vector3(1.0f, 0.0f, 0.0f));
			// If not ok, cross with y axis.
			if (t.GetLength() == 0)
			{
				t = a.Cross(Vector3(0.0f, 1.0f, 0.0f));
			}

			t.Normalize();
			x = t.x;
			y = t.y;
			z = t.z;
			w = 0;
		}

		// if vectors are parallel and are facing in the same direction
		//	the axis is zero and quaternion is the identity
		Quaternion result(axis.x * 0.5f, axis.y * 0.5f, axis.z * 0.5f, (dot + 1.0f) * 0.5f);
		x = axis.x * 0.5f;
		y = axis.y * 0.5f;
		z = axis.z * 0.5f;
		w = (dot + 1.0f) * 0.5f;
		Normalize();
	}

	void Quaternion::LookAt(const Vector3& direction, const Vector3& up)
	{
		Vector3 forward = direction.GetNormalized();
		Vector3 right = up.Cross(forward);

		Quaternion ret;
		w = Sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
		float w4_recip = 1.0f / (4.0f * w);
		x = (forward.y - up.z) * w4_recip;
		y = (right.z - forward.x) * w4_recip;
		z = (up.x - right.y) * w4_recip;
	}

	Matrix4 Quaternion::GetAsMatrix4() const
	{
		Matrix4 m(GetAsMatrix3());

		return m;
	}

	Matrix3 Quaternion::GetAsMatrix3() const
	{
		// Calculate coefficients				
		const float x2 = x + x;
		const float y2 = y + y;
		const float z2 = z + z;
		const float xx = x * x2;
		const float xy = x * y2;
		const float xz = x * z2;
		const float yy = y * y2;
		const float yz = y * z2;
		const float zz = z * z2;
		const float wx = w * x2;
		const float wy = w * y2;
		const float wz = w * z2;

		Matrix3 r;

		/*
		r.m[0][0] = 1 - (yy + zz);
		r.m[0][1] = xy + wz;
		r.m[0][2] = xz - wy;

		r.m[1][0] = xy - wz;
		r.m[1][1] = 1 - (xx + zz);
		r.m[1][2] = yz + wx;

		r.m[2][0] = xz + wy;
		r.m[2][1] = yz - wx;
		r.m[2][2] = 1 - (xx + yy);
		*/

		r.m[0][0] = 1.0f - (yy + zz);
		r.m[0][1] = xy - wz;
		r.m[0][2] = xz + wy;

		r.m[1][0] = xy + wz;
		r.m[1][1] = 1.0f - (xx + zz);
		r.m[1][2] = yz - wx;

		r.m[2][0] = xz - wy;
		r.m[2][1] = yz + wx;
		r.m[2][2] = 1.0f - (xx + yy);
		return r;
	}

	void Quaternion::GetAsEulerXYZ(Vector3& res) const
	{
		const float s = 2.0f * (z * x + w * y);
		if (s < 0.9999f)
		{
			if (s > -0.9999f)
			{
				res.y = Asin(s);
				res.x = Atan2(-(2.0f) * (z * y - w * x), (1.0f) - (2.0f) * (x * x + y * y));
				res.z = Atan2(-(2.0f) * (x * y - w * z), (1.0f) - (2.0f) * (z * z + y * y));
			}
			else
			{
				res.y = -k_pi_2;
				res.x = -Atan2((2.0f) * (x * y + w * z), (1.0f) - (2.0f) * (z * z + x * x));
				res.z = 0;
			}
		}
		else
		{
			res.y = k_pi_2;
			res.x = Atan2((2.0f) * (x * y + w * z), (1.0f) - (2.0f) * (z * z + x * x));
			res.z = 0.0f;
		}
	}

	Quaternion& Quaternion::Invert()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	Quaternion Quaternion::GetInverted() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	float Quaternion::Norm() const
	{
		return x * x + y * y + z * z + w * w;
	}

	Vector3 Quaternion::operator*(const Vector3& v) const
	{
		// nVidia SDK implementation				
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.Cross(v);
		uuv = qvec.Cross(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return v + uv + uuv;
	}

	Vector3 Quaternion::Rotate(const Vector3& v) const
	{
		Quaternion q = (*this) * Quaternion(v.x, v.y, v.z, (0.0f)) * GetInverted();
		return Vector3(q.x, q.y, q.z);
	}

	Quaternion& Quaternion::SetIndentity()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
		return *this;
	}

} // math

} // sh