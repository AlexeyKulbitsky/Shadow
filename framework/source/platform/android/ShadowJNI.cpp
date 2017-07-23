#include "../../TestApplication.h"
#include <jni.h>
#include <android/native_window_jni.h>
#include <Shadow.h>
#include <thread>
#include <mutex>

#include <io/android/AndroidFileSystem.h>
#include <platform/android/AndroidDevice.h>
#include <platform/android/AndroidEventsQueue.h>

static sh::AndroidDevice *androidDevice = nullptr;

////////////////////////////////////////////////

void AndroidMainFunction()
{
    sh::Device *device = sh::CreateDevice();
    androidDevice = static_cast<sh::AndroidDevice*>(device);
    TestApp* application = new TestApp();
    device->SetApplication(application);
    device->Init();
    device->Run();
    device->Destroy();
    delete application;
}


////////////////////////////////////////////////

bool isDeviceCreated = false;

std::thread shadowThread;

extern "C"
{

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnCreate(JNIEnv *env, jclass type, jobject mainActivity, jstring apkPath)
{
    sh::AndroidDevice::params.apkPath = env->GetStringUTFChars(apkPath, NULL);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnDestroy(JNIEnv *env, jclass type)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnStart(JNIEnv *env, jclass type)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnResume(JNIEnv *env, jclass type)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnPause(JNIEnv *env, jclass type)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnStop(JNIEnv *env, jclass type)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceCreated(JNIEnv *env, jclass type, jobject surface)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceChanged(JNIEnv *env, jclass type, jobject surface, jint _width, jint _height)
{
    if (!isDeviceCreated)
    {
		sh::AndroidDevice::params.width = static_cast<sh::u32>(_width);
		sh::AndroidDevice::params.height = static_cast<sh::u32>(_height);
		sh::AndroidDevice::params.WinId = ANativeWindow_fromSurface(env, surface);
		sh::AndroidDevice::params.driverType = sh::video::DriverType::OPENGL_ES_2_0;

		shadowThread = std::thread(AndroidMainFunction);

        isDeviceCreated = true;
    }
    else
    {
        sh::AndroidEvent ev;
        ev.type = sh::AndroidEvent::Type::SurfaceChanged;
        ev.event.surfaceChangedEvent.width = _width;
        ev.event.surfaceChangedEvent.height = _height;
        ev.event.surfaceChangedEvent.winId = ANativeWindow_fromSurface(env, surface);

        androidDevice->OnEvent(ev);
    }
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceDestroyed(JNIEnv *env, jclass type, jobject surface)
{
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnPress(JNIEnv *env, jclass type, jint x, jint y)
{
    if (!androidDevice)
        return;

    sh::AndroidEvent ev;
    ev.type = sh::AndroidEvent::Type::Press;
    ev.event.pressEvent.x = x;
    ev.event.pressEvent.y = y;

    androidDevice->OnEvent(ev);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnRelease(JNIEnv *env, jclass type, jint x, jint y)
{
    if (!androidDevice)
        return;

    sh::AndroidEvent ev;
    ev.type = sh::AndroidEvent::Type::Release;
    ev.event.releaseEvent.x = x;
    ev.event.releaseEvent.y = y;

    androidDevice->OnEvent(ev);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnMove(JNIEnv *env, jclass type, jint x, jint y)
{
    if (!androidDevice)
        return;

    sh::AndroidEvent ev;
    ev.type = sh::AndroidEvent::Type::Move;
    ev.event.moveEvent.x = x;
    ev.event.moveEvent.y = y;

    androidDevice->OnEvent(ev);
}

}
//#endif
