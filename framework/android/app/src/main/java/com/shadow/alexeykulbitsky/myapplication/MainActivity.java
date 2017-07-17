package com.shadow.alexeykulbitsky.myapplication;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.Surface;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.view.WindowManager;
import android.view.Window;
import android.util.Log;

public class MainActivity extends Activity implements SurfaceHolder.Callback
{
    private SurfaceView m_SurfaceView = null;
    private AssetManager m_assetManager = null;

    static
    {
        System.loadLibrary("framework");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        // Set view
        m_SurfaceView = new SurfaceView(this);
        m_SurfaceView.setEnabled(true);
        m_SurfaceView.setFocusable(true);
        m_SurfaceView.setFocusableInTouchMode(true);
        m_SurfaceView.getHolder().addCallback(this);
        setContentView(m_SurfaceView);

        String dataPath = getFilesDir().getPath() + "/";
        m_assetManager = getResources().getAssets();

        ShadowJNI.OnCreate(this, m_assetManager, dataPath);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        ShadowJNI.OnDestroy();
    }

    @Override
    protected void onStart()
    {
        super.onStart();
        ShadowJNI.OnStart();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        ShadowJNI.OnResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        ShadowJNI.OnPause();
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        ShadowJNI.OnStop();
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        ShadowJNI.SurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder)
    {
        ShadowJNI.SurfaceDestroyed(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {
        ShadowJNI.SurfaceChanged(holder.getSurface(), w, h);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        int actionIndex = event.getActionIndex();
        int actionId = event.getPointerId(actionIndex);
        int actionMasked = event.getActionMasked();
        switch (actionMasked)
        {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
            {
                return true;
            }
            case MotionEvent.ACTION_MOVE:
            {
                for(int i = 0; i < event.getPointerCount(); i++)
                {
                }
                return true;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            {
                return true;
            }
        }
        return super.onTouchEvent(event);
    }
}
