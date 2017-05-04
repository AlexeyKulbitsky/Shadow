#include <Shadow.h>

using namespace sh;
using namespace math;


template <typename T>
Quaternion<T> Mult(const Quaternion<T>& a, const Quaternion<T>& b)
{
	Quaternion<T> result;
	result.x = ( b.w * a.x ) + ( b.x * a.w ) + ( b.y * a.z ) - ( b.y * a.z );
	result.y = ( b.w * a.y ) + ( b.y * a.w ) + ( b.z * a.x ) - ( b.z * a.x );
	result.z = ( b.w * a.z ) + ( b.z * a.w ) + ( b.y * a.x ) - ( b.x * a.y );
	result.w = ( b.w * a.w ) - ( b.x * a.x ) - ( b.y * a.y ) - ( b.z * a.z );
	return result;
}


template <typename T>
Vector3<T> Mult(const Quaternion<T>& q, const Vector3<T>& v)
{
	// nVidia SDK implementation

	Vector3<T> uv, uuv;
	Vector3<T> qvec(q.x, q.y, q.z);
	uv = v.Cross(qvec);
	uuv = uv.Cross(qvec);
	uv *= ( 2 * q.w );
	uuv *= 2;

	return v + uv + uuv;
}

int main()
{
	/*
	sh::Device* device = sh::CreateDevice();
	sh::video::Driver* driver = device->GetDriver();
	driver->SetClearColor(sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));

	sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
	device->SetSceneManager(sceneMgr);

	if (device)
	{
	while (device->Run())
	{
	driver->BeginRendering();

	driver->EndRendering();
	}
	}
	*/
	Quaternionf xAxisQuat, yAxisQuat, zAxisQuat;
	xAxisQuat.SetFromAxisAngle(Vector3f(1.0f, 0.0f, 0.0f), k_pi / 2.0f);
	yAxisQuat.SetFromAxisAngle(Vector3f(0.0f, 1.0f, 0.0f), k_pi / 2.0f);
	zAxisQuat.SetFromAxisAngle(Vector3f(0.0f, 0.0f, 1.0f), k_pi / 2.0f);

	Quaternionf resQuatA = xAxisQuat * yAxisQuat;
	Quaternionf resQuatB = Mult(xAxisQuat, yAxisQuat);
	Vector3f pointA(1.0f, 0.0f, 0.0f);
	Vector3f pointB(1.0f, 0.0f, 0.0f);

	pointA = resQuatA * pointA;
	pointB = Mult(resQuatA, pointB);

	int a = 0;
	a++;

	return 0;
}



