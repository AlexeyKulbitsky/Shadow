#if 0
#include <jni.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>
#include <Shadow.h>
#include <thread>
#include <mutex>

#include <io/android/AndroidFileSystem.h>
#include "AndroidDevice.h"

static AAssetManager *assetManager = nullptr;
static sh::String assetsDataPath = "";
static sh::AndroidDevice *androidDevice = nullptr;

////////////////////////////////////////////////

static ANativeWindow *window = 0;

static ANativeWindow *testWindow = 0;

static sh::Device* device = 0;
int width = 0;
int height = 0;

bool isDeviceCreated = false;
bool isDriverInited = false;

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
                if (!isDriverInited)
                {
                    _device->SetWinId(window);
                    _device->Init();

                    //_device->GetDriver()->SetClearColor(sh::math::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
                    auto sceneMgr = _device->GetSceneManager();
                    auto camera = sceneMgr->GetCamera();
                    camera->SetProjection(3.1415926535f / 3.0f, width, height, 0.1f, 1000.0f);
                    //sceneMgr->LoadScene("test_scene.xml");

                    isDriverInited = true;
                }
                else
                {
                    auto sceneMgr = _device->GetSceneManager();
                    auto camera = sceneMgr->GetCamera();
                    camera->SetProjection(3.1415926535f / 3.0f, width, height, 0.1f, 1000.0f);
                    _device->GetDriver()->SetWindow(window, width, height);
                    //_device->GetDriver()->SetClearColor(sh::math::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
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

                //_device->GetSceneManager()->Update();

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

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_OnCreate(JNIEnv *env, jclass type, jobject mainActivity, jobject assManager, jstring dataPath)
{
    if (!isDeviceCreated)
    {
        sh::CreationParameters params;
        params.driverType = sh::video::DriverType::VULKAN;
        device = sh::CreateDevice(params);

        AAssetManager *mng = AAssetManager_fromJava(env, assManager);
        sh::String stdDataPath = env->GetStringUTFChars(dataPath, NULL);

        sh::io::AndroidFileSystem * fs = static_cast<sh::io::AndroidFileSystem*>(device->GetFileSystem());
        fs->SetAssetManager(mng);
        fs->SetDataPath(stdDataPath);

        isDeviceCreated = true;

        shadowThread = std::thread(shadowThreadFunction, device);

        //////////////////////////////////////////////////////////////

        assetManager = AAssetManager_fromJava(env, assManager);
        assetsDataPath = env->GetStringUTFChars(dataPath, NULL);
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
    shadowMutex.lock();
    testWindow = ANativeWindow_fromSurface(env, surface);
    testWindow = 0;
    shadowMutex.unlock();
}

void Java_com_shadow_alexeykulbitsky_myapplication_ShadowJNI_SurfaceChanged(JNIEnv *env, jclass type, jobject surface, jint _width, jint _height)
{
    shadowMutex.lock();
    window = ANativeWindow_fromSurface(env, surface);
    width = _width;
    height = _height;
    shMessage = ShadowMessage::Create;
    shadowMutex.unlock();

    ///////////////////////////////////////////////////////////

    if (!isDeviceCreated)
    {
        sh::CreationParameters params;
        params.width = static_cast<sh::u32>(_width);
        params.height = static_cast<sh::u32>(_height);
        params.WinId = ANativeWindow_fromSurface(env, surface);
        params.driverType = sh::video::DriverType::OPENGL_ES_2_0;

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
    shadowMutex.lock();
    shMessage = ShadowMessage::None;
    shadowMutex.unlock();
}

}
#endif

#define SHADOW_MAIN_FUNCTION(ApplicationClassType, CreationParams)		\
void shadowMainFunction()												\
{																		\
	ApplicationClassType app(CreationParams);							\
	app.Init();															\
	app.Run();															\
	app.Destroy();														\
}																		