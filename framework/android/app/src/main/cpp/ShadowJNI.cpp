#include <jni.h>
#include <android/native_window_jni.h>
#include <pthread.h>
#include <Shadow.h>
#include <iostream>

static ANativeWindow *window = 0;
static sh::Device* device = 0;

enum RenderThreadMessage
{
    MSG_NONE = 0,
    MSG_WINDOW_SET,
    MSG_WINDOW_RELEASE,
    MSG_RENDER_LOOP_EXIT,
    MSG_RENDER_LOOP
};
enum RenderThreadMessage _msg = MSG_NONE;
pthread_t _threadId;
pthread_mutex_t _mutex;


void* Main(void *myself)
{
    sh::Device* device = (sh::Device*)myself;
    bool renderingEnabled = true;


    while (renderingEnabled) {

        pthread_mutex_lock(&_mutex);

        // process incoming messages
        switch (_msg) {

            case MSG_WINDOW_SET:
            {
                if (!device->GetContextManager()->IsContextCreated())
                {
                    device->SetWindow(window);
                    device->GetContextManager()->AttachWindow(device->GetWinId());
                    device->GetContextManager()->CreateContext(true);
                    device->GetDriver()->Init();
                    device->GetDriver()->SetViewport(0, 0, 500, 500);
                    device->GetDriver()->SetClearColor(sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
                } else
                {
                    device->GetContextManager()->DestroyContext(false);
                    device->SetWindow(window);
                    device->GetContextManager()->AttachWindow(device->GetWinId());
                    device->GetContextManager()->CreateContext(false);
                    device->GetDriver()->SetViewport(0, 0, 500, 500);
                    device->GetDriver()->SetClearColor(sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
                }
                //device->GetDriver()->SetViewport(0, 0, 500, 500);
                //device->GetDriver()->SetClearColor(sh::math::Vector4f(1.0f, 0.0f, 1.0f, 1.0f));
                _msg = MSG_RENDER_LOOP;
            }
                //device->Init();
                //device->GetContextManager()->AttachWindow(device->GetWinId());
                //device->CreateWindowContext();
                //device->GetDriver()->SetViewport(0, 0, 500, 500);
                //device->GetDriver()->SetClearColor(sh::math::Vector4f(1.0f, 0.0f, 1.0f, 1.0f));
                //_msg = MSG_RENDER_LOOP;
                break;


            case MSG_RENDER_LOOP_EXIT:
                renderingEnabled = false;
                break;
            case MSG_RENDER_LOOP:
                device->GetDriver()->BeginRendering();
                device->GetDriver()->EndRendering();
                break;
            default:
                break;
        }

        pthread_mutex_unlock(&_mutex);
    }


    pthread_exit(0);

    return 0;
}



extern "C"
{

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnCreate(JNIEnv *env, jclass type)
{

}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnStart(JNIEnv *env, jclass type)
{
    pthread_mutex_init(&_mutex, 0);
    device = sh::CreateDevice();
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnResume(JNIEnv *env, jclass type)
{
    pthread_create(&_threadId, 0, Main, device);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnPause(JNIEnv *env, jclass type)
{
    pthread_mutex_lock(&_mutex);
    //_msg = MSG_RENDER_LOOP_EXIT;
    pthread_mutex_unlock(&_mutex);
    pthread_join(_threadId, 0);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnStop(JNIEnv *env, jclass type)
{
    pthread_mutex_destroy(&_mutex);
    delete device;
    device = 0;
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceCreated(JNIEnv *env, jclass type, jobject surface)
{
    //window = ANativeWindow_fromSurface(env, surface);

    //pthread_mutex_lock(&_mutex);
    //_msg = MSG_WINDOW_SET;
   // device->SetWindow(window);
   // pthread_mutex_unlock(&_mutex);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceChanged(JNIEnv *env, jclass type, jobject surface, jint width, jint height)
{
    window = ANativeWindow_fromSurface(env, surface);

    pthread_mutex_lock(&_mutex);
    _msg = MSG_WINDOW_SET;
    //device->SetWindow(window);
    pthread_mutex_unlock(&_mutex);

    /*
    pthread_mutex_lock(&_mutex);
    _msg = MSG_WINDOW_SET;
    if (!device->GetContextManager()->IsContextCreated())
    {
        device->SetWindow(window);
        device->GetContextManager()->CreateContext(true);
        device->GetDriver()->Init();
    } else
    {
        device->GetContextManager()->DestroyContext(false);
        device->SetWindow(window);
        device->GetContextManager()->CreateContext(false);
    }
    pthread_mutex_unlock(&_mutex);
     */

    //std::cout << "Changed\n";

    //pthread_mutex_lock(&_mutex);
    //device->GetDriver()->SetViewport(0, 0, width, height);
    //pthread_mutex_unlock(&_mutex);
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceDestroyed(JNIEnv *env, jclass type)
{
    //pthread_mutex_lock(&_mutex);
    //_msg = MSG_WINDOW_RELEASE;
    //pthread_mutex_unlock(&_mutex);
}

}