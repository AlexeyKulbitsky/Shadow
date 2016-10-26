#pragma once
namespace sh
{

	namespace math
	{
		float degrees(float radians);
		float radians(float degrees);

		double degrees(double radians);
		double radians(double degrees);

		template<typename T>
		T degrees(const T& radians);
		template<typename T>
		T radians(const T& degrees);

		//////////////////////////////////////////////////////////////////////////

		float cos(float angle);
		float sin(float angle);
		float tan(float angle);
		float acos(float angle);
		float asin(float angle);
		float atan(float angle);
		float atan2(float y, float x);
		void sincos(float angle, float& sin, float& cos);

		double cos(double angle);
		double sin(double angle);
		double tan(double angle);
		double acos(double angle);
		double asin(double angle);
		double atan(double angle);
		double atan2(double y, double x);
		void sincos(double angle, double& sin, double& cos);

		//component wise trig for vectors.
		//vec3 val = sin(angles)  is equivalent to
		//	val.x = sin(angles.x)
		//	val.y = sin(angles.y)
		//	val.z = sin(angles.z)
		template<typename T>
		T cos(const T& angle);
		template<typename T>
		T sin(const T& angle);
		template<typename T>
		T tan(const T& angle);
		template<typename T>
		T acos(const T& angle);
		template<typename T>
		T asin(const T& angle);
		template<typename T>
		T atan(const T& angle);
		template<typename T>
		T atan2(const T& y, const T& x);
		template<typename T>
		void sincos(const T& angle, T& sin, T& cos);

	}

}