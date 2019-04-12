#include "math/Matrix3.h"

#include <utility>

namespace sh
{

namespace math
{

	Matrix3::Matrix3()
	{
	}

	Matrix3::Matrix3(float value)
	{
		m[0][0] = m[0][1] = m[0][2] = value;
		m[1][0] = m[1][1] = m[1][2] = value;
		m[2][0] = m[2][1] = m[2][2] = value;


	}
	Matrix3::Matrix3(const Matrix3& other)
	{
		operator=(other);
	}

	Matrix3& Matrix3::operator=(const Matrix3& m2)
	{
		m[0][0] = m2.m[0][0];
		m[0][1] = m2.m[0][1];
		m[0][2] = m2.m[0][2];

		m[1][0] = m2.m[1][0];
		m[1][1] = m2.m[1][1];
		m[1][2] = m2.m[1][2];

		m[2][0] = m2.m[2][0];
		m[2][1] = m2.m[2][1];
		m[2][2] = m2.m[2][2];
		return *this;
	}

	Matrix3 Matrix3::operator*(const Matrix3& m2) const
	{
		Matrix3 r;

		r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0];
		r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1];
		r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2];

		r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0];
		r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1];
		r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2];

		r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0];
		r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1];
		r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2];

		return r;
	}

	Matrix3 Matrix3::operator+(const Matrix3& m2) const
	{
		Matrix3 r;

		r.m[0][0] = m[0][0] + m2.m[0][0];
		r.m[0][1] = m[0][1] + m2.m[0][1];
		r.m[0][2] = m[0][2] + m2.m[0][2];

		r.m[1][0] = m[1][0] + m2.m[1][0];
		r.m[1][1] = m[1][1] + m2.m[1][1];
		r.m[1][2] = m[1][2] + m2.m[1][2];

		r.m[2][0] = m[2][0] + m2.m[2][0];
		r.m[2][1] = m[2][1] + m2.m[2][1];
		r.m[2][2] = m[2][2] + m2.m[2][2];

		return r;
	}

	Matrix3 Matrix3::operator-(const Matrix3& m2) const
	{
		Matrix3 r;
		r.m[0][0] = m[0][0] - m2.m[0][0];
		r.m[0][1] = m[0][1] - m2.m[0][1];
		r.m[0][2] = m[0][2] - m2.m[0][2];

		r.m[1][0] = m[1][0] - m2.m[1][0];
		r.m[1][1] = m[1][1] - m2.m[1][1];
		r.m[1][2] = m[1][2] - m2.m[1][2];

		r.m[2][0] = m[2][0] - m2.m[2][0];
		r.m[2][1] = m[2][1] - m2.m[2][1];
		r.m[2][2] = m[2][2] - m2.m[2][2];

		return r;
	}

	Matrix3 Matrix3::operator-() const
	{
		Matrix3 r;

		r.m[0][0] = -m[0][0];
		r.m[0][1] = -m[0][1];
		r.m[0][2] = -m[0][2];

		r.m[1][0] = -m[1][0];
		r.m[1][1] = -m[1][1];
		r.m[1][2] = -m[1][2];

		r.m[2][0] = -m[2][0];
		r.m[2][1] = -m[2][1];
		r.m[2][2] = -m[2][2];

		return r;
	}

	Vector3 Matrix3::operator*(const Vector3& v) const
	{
		return Vector3(
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
		);
	}

	void Matrix3::SetIdentity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
	}

	Matrix3& Matrix3::Transpose()
	{
		std::swap(m[0][1], m[1][0]);
		std::swap(m[0][2], m[2][0]);
		std::swap(m[1][2], m[2][1]);
		return *this;
	}

	Matrix3 Matrix3::GetTransposed() const
	{
		Matrix3 r(*this);
		r.Transpose();
		return r;
	}

} // math

} // sh