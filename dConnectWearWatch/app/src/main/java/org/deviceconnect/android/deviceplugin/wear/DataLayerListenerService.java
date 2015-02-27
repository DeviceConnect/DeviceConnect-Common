/*
DataLayerListenerService.java
Copyright (c) 2014 NTT DOCOMO,INC.
Released under the MIT license
http://opensource.org/licenses/mit-license.php
 */
package org.deviceconnect.android.deviceplugin.wear;

import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Vibrator;
import android.util.Log;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.wearable.Asset;
import com.google.android.gms.wearable.DataEvent;
import com.google.android.gms.wearable.DataEventBuffer;
import com.google.android.gms.wearable.DataMap;
import com.google.android.gms.wearable.DataMapItem;
import com.google.android.gms.wearable.MessageApi;
import com.google.android.gms.wearable.MessageEvent;
import com.google.android.gms.wearable.Node;
import com.google.android.gms.wearable.Wearable;
import com.google.android.gms.wearable.WearableListenerService;

import org.deviceconnect.android.deviceplugin.wear.activity.CanvasActivity;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * DataLayerListenerService.
 *
 * @author NTT DOCOMO, INC.
 */
public class DataLayerListenerService extends WearableListenerService implements SensorEventListener {

    /** SensorManager. */
    private SensorManager mSensorManager;

    /** Gyro x. */
    private float mGyroX;

    /** Gyro y. */
    private float mGyroY;

    /** Gyro z. */
    private float mGyroZ;

    /** Device NodeID . */
    private String mId;

    /** GyroSensor. */
    private Sensor mGyroSensor;

    /** AcceleratorSensor. */
    private Sensor mAccelerometer;

    /** The start time for measuring the interval. */
    private long mStartTime;

    /**
     * スレッド管理用クラス.
     */
    private final ExecutorService mExecutorService = Executors.newSingleThreadExecutor();

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onDataChanged(DataEventBuffer dataEvents) {
        super.onDataChanged(dataEvents);
        for (DataEvent event : dataEvents) {
            if (event.getType() == DataEvent.TYPE_CHANGED &&
                    event.getDataItem().getUri().getPath().equals(WearConst.PATH_CANVAS)) {
                DataMapItem dataMapItem = DataMapItem.fromDataItem(event.getDataItem());
                DataMap map = dataMapItem.getDataMap();

                Asset profileAsset = map.getAsset(WearConst.PARAM_BITMAP);
                int x = map.getInt(WearConst.PARAM_X);
                int y = map.getInt(WearConst.PARAM_Y);
                int mode = map.getInt(WearConst.PARAM_MODE);

                Intent intent = new Intent();
                intent.setClass(this, CanvasActivity.class);
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                intent.putExtra(WearConst.PARAM_BITMAP, profileAsset);
                intent.putExtra(WearConst.PARAM_X, x);
                intent.putExtra(WearConst.PARAM_Y, y);
                intent.putExtra(WearConst.PARAM_MODE, mode);
                startActivity(intent);
            }
        }
    }

    @Override
    public void onMessageReceived(final MessageEvent messageEvent) {
        // get id of wear device
        mId = messageEvent.getSourceNodeId();
        if (messageEvent.getPath().equals(WearConst.DEVICE_TO_WEAR_VIBRATION_RUN)) {
            startVibration(messageEvent);
        } else if (messageEvent.getPath().equals(WearConst.DEVICE_TO_WEAR_VIBRATION_DEL)) {
            stopVibration();
        } else if (messageEvent.getPath().equals(WearConst.DEVICE_TO_WEAR_DEIVCEORIENTATION_REGISTER)) {
            registerSensor();
        } else if (messageEvent.getPath().equals(WearConst.DEVICE_TO_WEAR_DEIVCEORIENTATION_UNREGISTER)) {
            unregisterSensor();
        } else {
            if (BuildConfig.DEBUG) {
                Log.e("Wear", "unknown event");
            }
        }
    }

    @Override
    public void onPeerConnected(final Node peer) {
    }

    @Override
    public void onPeerDisconnected(final Node peer) {
    }

    @Override
    public void onSensorChanged(final SensorEvent sensorEvent) {
        if (sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            long time = System.currentTimeMillis();
            long interval = time - mStartTime;
            mStartTime = time;

            float mAccellX = sensorEvent.values[0];
            float mAccellY = sensorEvent.values[1];
            float mAccellZ = sensorEvent.values[2];
            final String data = mAccellX + "," + mAccellY + "," + mAccellZ
                    + "," + mGyroX + "," + mGyroY + "," + mGyroZ + "," + interval;
            mExecutorService.execute(new Runnable() {
                @Override
                public void run() {
                    GoogleApiClient client = getClient();
                    if (!client.isConnected()) {
                        client.connect();
                    } else {
                        MessageApi.SendMessageResult result = Wearable.MessageApi.sendMessage(client, mId,
                                WearConst.WEAR_TO_DEVICE_DEIVCEORIENTATION_DATA, data.getBytes()).await();
                        if (!result.getStatus().isSuccess()) {
                            if (BuildConfig.DEBUG) {
                                Log.e("WEAR", "Failed to send a sensor event.");
                            }
                        }
                    }
                }
            });
        } else if (sensorEvent.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
            mGyroX = sensorEvent.values[0];
            mGyroY = sensorEvent.values[1];
            mGyroZ = sensorEvent.values[2];
        }
    }

    @Override
    public void onAccuracyChanged(final Sensor sensor, final int accuracy) {
    }

    /**
     * バイブレーションを開始する.
     * @param messageEvent メッセージ
     */
    private void startVibration(final MessageEvent messageEvent) {
        // get vibration pattern
        String mPattern = new String(messageEvent.getData());

        // Make array of pattern
        String[] mPatternArray = mPattern.split(",", 0);
        long[] mPatternLong = new long[mPatternArray.length + 1];
        mPatternLong[0] = 0;
        for (int i = 1; i < mPatternLong.length; i++) {
            mPatternLong[i] = Integer.parseInt(mPatternArray[i - 1]);
        }

        // vibrate
        Vibrator vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
        vibrator.vibrate(mPatternLong, -1);
    }

    /**
     * バイブレーションを停止する.
     */
    private void stopVibration() {
        // stop vibrate
        Vibrator vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
        vibrator.cancel();
    }

    /**
     * センサーを登録する.
     */
    private void registerSensor() {
        GoogleApiClient client = getClient();
        if (client == null || !client.isConnected()) {
            client = new GoogleApiClient.Builder(this).addApi(Wearable.API).build();
            client.connect();
            ConnectionResult connectionResult = client.blockingConnect(30, TimeUnit.SECONDS);
            if (!connectionResult.isSuccess()) {
                if (BuildConfig.DEBUG) {
                    Log.e("WEAR", "Failed to connect google play service.");
                }
            }
        }

        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        List<Sensor> accelSensors = mSensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER);
        if (accelSensors.size() > 0) {
            mAccelerometer = accelSensors.get(0);
            mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        }

        List<Sensor> gyroSensors = mSensorManager.getSensorList(Sensor.TYPE_GYROSCOPE);
        if (gyroSensors.size() > 0) {
            mGyroSensor = gyroSensors.get(0);
            mSensorManager.registerListener(this, mGyroSensor, SensorManager.SENSOR_DELAY_NORMAL);
        }

        mStartTime = System.currentTimeMillis();
    }

    /**
     * センサーを解除する.
     */
    private void unregisterSensor() {
        if (mSensorManager != null) {
            mSensorManager.unregisterListener(this, mAccelerometer);
            mSensorManager.unregisterListener(this, mGyroSensor);
            mSensorManager.unregisterListener(this);
            mSensorManager = null;
        }
    }

    /**
     * GoogleApiClientを取得する.
     * @return GoogleApiClient
     */
    private GoogleApiClient getClient() {
        return ((WearApplication) getApplication()).getGoogleApiClient();
    }
}
