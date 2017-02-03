package com.shadow.alexeykulbitsky.myapplication;
import android.view.Surface;

public class ShadowJNI
{
    public static native void OnCreate();
    public static native void OnStart();
    public static native void OnResume();
    public static native void OnPause();
    public static native void OnStop();
    public static native void SurfaceCreated(Surface surface);
    public static native void SurfaceChanged(Surface surface, int width, int height);
    public static native void SurfaceDestroyed();
}
