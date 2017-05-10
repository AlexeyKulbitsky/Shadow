package com.shadow.alexeykulbitsky.myapplication;

import android.app.Activity;
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

    static
    {
        System.loadLibrary("shadow-lib");
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

        ShadowJNI.OnCreate();
    }

    @Override
    protected void onStart() {
        super.onStart();
        ShadowJNI.OnStart();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ShadowJNI.OnResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        ShadowJNI.OnPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        ShadowJNI.OnStop();
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        Log.d("---SHADOW", "SURFACE CREATED");
        ShadowJNI.SurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder)
    {
        Log.d("---SHADOW", "SURFACE DESTROYED");
        ShadowJNI.SurfaceDestroyed();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {
        Log.d("---SHADOW", "SURFACE CHANGED");
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
                //cross.ActionDown(event.getX(actionIndex), event.getY(actionIndex), actionId);
                return true;
            }
            case MotionEvent.ACTION_MOVE:
            {
                for(int i = 0; i < event.getPointerCount(); i++)
                {
                   // cross.ActionMove(event.getX(i), event.getY(i), event.getPointerId(i));
                }
                return true;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                {
                //cross.ActionUp(event.getX(actionIndex), event.getY(actionIndex), actionId);
                return true;
            }
        }
        return super.onTouchEvent(event);
    }
}
