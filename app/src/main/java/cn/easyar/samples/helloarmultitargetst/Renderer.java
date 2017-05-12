/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

package cn.easyar.samples.helloarmultitargetst;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;

import cn.easyar.samples.helloarmultitargetst.PlatformAssetUtils;

public class Renderer implements GLSurfaceView.Renderer {

    private final Context context;
    
    public Renderer(Context context) {
        this.context = context;
    }
    
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        PlatformAssetUtils.init_asset_manager(context.getAssets());
        MainActivity.nativeInitGL();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        MainActivity.nativeResizeGL(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        MainActivity.nativeRender();
    }

}
