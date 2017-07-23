package com.shadow.alexeykulbitsky.myapplication;
import android.view.Surface;
import android.content.res.AssetManager;

public class ShadowJNI
{
    public static native void OnCreate(MainActivity activity, String apkPath);
    public static native void OnDestroy();
    public static native void OnStart();
    public static native void OnResume();
    public static native void OnPause();
    public static native void OnStop();
    public static native void SurfaceCreated(Surface surface);
    public static native void SurfaceChanged(Surface surface, int width, int height);
    public static native void SurfaceDestroyed(Surface surface);
    public static native void OnPress(int x, int y);
    public static native void OnRelease(int x, int y);
    public static native void OnMove(int x, int y);
}
