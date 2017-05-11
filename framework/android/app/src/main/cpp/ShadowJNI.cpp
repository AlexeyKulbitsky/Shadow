#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>
#include <Shadow.h>
#include <thread>
#include <mutex>

static ANativeWindow *window = 0;
static sh::Device* device = 0;

bool isDeviceCreated = false;

std::thread shadowThread;
std::mutex shadowMutex;

enum class ShadowMessage
{
    Stop,
    Run,
    Create,
    None
};

ShadowMessage shMessage = ShadowMessage::None;

void shadowThreadFunction(sh::Device* _device)
{
    bool looping = true;
    while (looping)
    {
        shadowMutex.lock();

        switch (shMessage)
        {
            case ShadowMessage::Create:
            {
                if (!device->GetContextManager()->IsContextCreated())
                {
                    _device->SetWindow(window);
                    _device->GetContextManager()->AttachWindow(device->GetWinId());
                    _device->GetContextManager()->CreateContext(true);
                    _device->GetDriver()->Init();
                    _device->GetDriver()->SetViewport(0, 0, 500, 500);
                    _device->GetDriver()->SetClearColor(sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
                }
                else
                {
                    _device->GetContextManager()->DestroyContext(false);
                    _device->SetWindow(window);
                    _device->GetContextManager()->AttachWindow(device->GetWinId());
                    _device->GetContextManager()->CreateContext(false);
                    _device->GetDriver()->SetViewport(0, 0, 500, 500);
                    _device->GetDriver()->SetClearColor(sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
                }
                shMessage = ShadowMessage::Run;
            }
            break;
            case ShadowMessage::Stop:
            {
                looping = false;
            }
            break;
            case ShadowMessage::Run:
            {
                _device->GetDriver()->BeginRendering();
                _device->GetDriver()->EndRendering();
            }
            break;
            default:
                break;
        }

        shadowMutex.unlock();
    }
}


extern "C"
{

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnCreate(JNIEnv *env, jclass type)
{
    if (!isDeviceCreated)
    {
        device = sh::CreateDevice();
        isDeviceCreated = true;

        shadowThread = std::thread(shadowThreadFunction, device);
    }
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnDestroy(JNIEnv *env, jclass type)
{
    // This method is called even if orientation changes
    /*
    if (isDeviceCreated)
    {
        shadowMutex.lock();
        shMessage = ShadowMessage::Stop;
        shadowMutex.unlock();

        shadowThread.join();
        isDeviceCreated = false;
        delete device;
        device = nullptr;
    }
    */
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

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceChanged(JNIEnv *env, jclass type, jobject surface, jint width, jint height)
{
    shadowMutex.lock();
    window = ANativeWindow_fromSurface(env, surface);
    shMessage = ShadowMessage::Create;
    shadowMutex.unlock();
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceDestroyed(JNIEnv *env, jclass type)
{
    shadowMutex.lock();
    shMessage = ShadowMessage::None;
    shadowMutex.unlock();
}

}