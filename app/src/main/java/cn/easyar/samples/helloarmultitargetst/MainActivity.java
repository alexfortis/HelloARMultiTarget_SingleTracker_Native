/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

package cn.easyar.samples.helloarmultitargetst;

import android.content.res.Configuration;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.view.WindowManager;

import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.content.BroadcastReceiver;
import android.os.Binder;
import android.os.IBinder;
import android.os.Bundle;

import android.util.Log;

import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothDevice;

import cn.easyar.engine.EasyAR;

public class MainActivity extends AppCompatActivity {

    static String key = "znzLr0VMrkwdexA464yl7sR4MJ3xWj37MCfymnWb4nm7GbXRoDwFDBGWA780SrPWDxPfUv0dN"
            + "meu1mXkSm2JuiQHdvng17oRLjXD31d1901503924a9f5225729f7af4b9d0ooPsHpOBWtqpqKw8VWJqUT3X"
            + "Sr31kMvdiMzzK0vAI7gG2NWroBPi4MAgJUxyERJmHpdR";

    static {
        System.loadLibrary("EasyAR");
        System.loadLibrary("HelloARNative");
        //System.loadLibrary("HelloARMultiTargetSTNative");
    }

    public static native void nativeInitGL();
    public static native void nativeResizeGL(int w, int h);
    public static native void nativeRender();
    private native boolean nativeInit();
    private native void nativeDestory();
    private native void nativeRotationChange(boolean portrait);
    private native void contactChanged(boolean contact);


    private static final String TAG = "MainActivity";
    private static final String PNAME = "HMSoft";
    private static final String mDevAddr = "20:91:48:4C:25:B3";

    private int REQUEST_ENABLE_BT = 1;
    private boolean contact = false;

    private BluetoothAdapter mAdapt;
    private BLEService mBLEService;

    private final ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName cName, IBinder service) {
            mBLEService = ((BLEService.LocalBinder) service).getService();
            if (!mBLEService.initialize()) {
                Log.e(TAG, "Unable to initialize bluetooth");
                finish();
            }

            mBLEService.connect(mDevAddr);
        }

        @Override
        public void onServiceDisconnected(ComponentName cName) {
            mBLEService = null;
        }
    };

    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            //Log.i(TAG, "BC Recv with action: " + action);
            if (BLEService.ACTION_DATA_AVAILABLE.equals(action)) {

                boolean newContact = intent.getBooleanExtra(BLEService.CONTACT_STATE, false);


                if (contact != newContact) {
                    contact = newContact;
                    contactChanged(contact);
                    Log.i(TAG, "Contact pad status has changed");
                    Log.i(TAG, "Contact value: " + contact);
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        /* Bluetooth initialization and setup */
        // Get the BT adapter
        final BluetoothManager btMan = 
            (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mAdapt = btMan.getAdapter();

        // Start the BLEService
        Intent gattServiceIntent = new Intent(this, BLEService.class);
        bindService(gattServiceIntent, mServiceConnection, Context.BIND_AUTO_CREATE);


        /* EASYAR Initialization and setup */
        EasyAR.initialize(this, key);
        nativeInit();

        GLView glView = new GLView(this);
        glView.setRenderer(new Renderer(this));
        glView.setZOrderMediaOverlay(true);

        ((ViewGroup) findViewById(R.id.preview)).addView(glView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        nativeRotationChange(getWindowManager().getDefaultDisplay().getRotation() == android.view.Surface.ROTATION_0);

    }

    @Override
    public void onConfigurationChanged(Configuration config) {
        super.onConfigurationChanged(config);
        nativeRotationChange(getWindowManager().getDefaultDisplay().getRotation() == android.view.Surface.ROTATION_0);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBLEService = null;
        nativeDestory();
    }
    @Override
    protected void onResume() {
        super.onResume();

        // Check if the BT adapter is set
        if (mAdapt == null || !mAdapt.isEnabled()) {
            Intent enableBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBT, REQUEST_ENABLE_BT);
        } 

        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBLEService != null) {
            final boolean result = mBLEService.connect(mDevAddr);
            Log.d(TAG, "Connect request result=" + result);
        }

        // Resume easyar
        EasyAR.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        EasyAR.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BLEService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }
}
