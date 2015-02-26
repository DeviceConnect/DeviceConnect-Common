/*
WearTouchProfileActivity.java
Copyright (c) 2015 NTT DOCOMO,INC.
Released under the MIT license
http://opensource.org/licenses/mit-license.php
 */
package org.deviceconnect.android.deviceplugin.wear;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.wearable.view.WatchViewStub;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

/**
 * WearTouchProfileActivity.
 *
 * @author NTT DOCOMO, INC.
 */
public class WearTouchProfileActivity extends Activity {
    /** Broadcast receiver. */
    MyBroadcastReceiver mReceiver;

    /** Intent filter. */
    IntentFilter mIntentFilter;

    /** Gesture detector. */
    GestureDetector mGestureDetector;

    /** Event flag. */
    private int mRegisterEvent = 0;
    /** Event flag define (touch). */
    private static final int REGIST_FLAG_TOUCH_TOUCH = 0x01;
    /** Event flag define (touchstart). */
    private static final int REGIST_FLAG_TOUCH_TOUCHSTART = 0x02;
    /** Event flag define (touchend). */
    private static final int REGIST_FLAG_TOUCH_TOUCHEND = 0x04;
    /** Event flag define (doubletap). */
    private static final int REGIST_FLAG_TOUCH_DOUBLETAP = 0x08;
    /** Event flag define (touchmove). */
    private static final int REGIST_FLAG_TOUCH_TOUCHMOVE = 0x10;
    /** Event flag define (touchcancel). */
    private static final int REGIST_FLAG_TOUCH_TOUCHCANCEL = 0x20;

    /**
     * Constructor.
     */
    public WearTouchProfileActivity() {
        mGestureDetector = new GestureDetector(this, new SimpleOnGestureListener() {

            @Override
            public boolean onDoubleTap(final MotionEvent event) {
                if ((mRegisterEvent & REGIST_FLAG_TOUCH_DOUBLETAP) == 0) {
                    sendEventData(event);
                }
                return super.onDoubleTap(event);
            }
        });
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Get intent data.
        Intent intent = getIntent();
        setRegisterEvent(intent.getStringExtra(WearConst.PARAM_TOUCH_REGIST));

        setContentView(R.layout.activity_wear_touch_profile);
        final WatchViewStub stub = (WatchViewStub) findViewById(R.id.watch_view_stub_touch);
        stub.setOnLayoutInflatedListener(new WatchViewStub.OnLayoutInflatedListener() {
            @Override
            public void onLayoutInflated(final WatchViewStub stub) {
            }
        });

        mReceiver = new MyBroadcastReceiver();
        mIntentFilter = new IntentFilter(WearConst.PARAM_DC_WEAR_KEYEVENT_SVC_TO_ACT);

        // For service destruction suppression.
        Intent i = new Intent(WearConst.ACTION_WEAR_PING_SERVICE);
        startService(i);
    }

    @Override
    public void onResume() {
        super.onResume();
        registerReceiver(mReceiver, mIntentFilter);
    }

    @Override
    public void onPause() {
        super.onPause();
        unregisterReceiver(mReceiver);
    }

    @Override
    public boolean onTouchEvent(final MotionEvent event) {
        boolean execFlag = false;
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: // 1st touch only.
            case MotionEvent.ACTION_POINTER_DOWN: // Others touch.
                // "ontouch" event processing.
                if ((mRegisterEvent & REGIST_FLAG_TOUCH_TOUCH) != 0) {
                    sendEventData(event);
                }

                // "ontouchstart" event processing.
                if ((mRegisterEvent & REGIST_FLAG_TOUCH_TOUCHSTART) != 0) {
                    execFlag = true;
                }
                break;
            case MotionEvent.ACTION_UP: // Last touch remove only.
            case MotionEvent.ACTION_POINTER_UP: // Others touch move.
                // "ontouchend" event processing.
                if ((mRegisterEvent & REGIST_FLAG_TOUCH_TOUCHEND) != 0) {
                    execFlag = true;
                }
                break;
            case MotionEvent.ACTION_MOVE:
                // "ontouchmove" event processing.
                if ((mRegisterEvent & REGIST_FLAG_TOUCH_TOUCHMOVE) != 0) {
                    execFlag = true;
                }
                break;
            case MotionEvent.ACTION_CANCEL:
                // "ontouchcancel" event processing.
                if ((mRegisterEvent & REGIST_FLAG_TOUCH_TOUCHCANCEL) != 0) {
                    execFlag = true;
                }
                break;
            default:
                return mGestureDetector.onTouchEvent(event);
        }

        if (execFlag) {
            sendEventData(event);
        }
        return mGestureDetector.onTouchEvent(event);
    }

//    /**
//     * Gesture Listener.
//     */
//    private final SimpleOnGestureListener mSimpleOnGestureListener = new SimpleOnGestureListener() {
//
//        @Override
//        public boolean onDoubleTap(final MotionEvent event) {
//            if ((mRegisterEvent & REGIST_FLAG_TOUCH_DOUBLETAP) == 0) {
//                sendEventData(event);
//            }
//            return super.onDoubleTap(event);
//        }
//    };

    /**
     * Send event data.
     *
     * @param event MotionEvent.
     */
    private void sendEventData(final MotionEvent event) {
        int dataCount = event.getPointerCount();
        StringBuffer data = new StringBuffer(dataCount);

        for (int n = 0; n < dataCount; n++) {
            int pointerId = event.getPointerId(n);
            data.append(",").append(pointerId).append(",").append(event.getX(n)).append(",").append(event.getY(n));
        }

        // Send key event data to service.
        Intent i = new Intent(WearConst.PARAM_DC_WEAR_KEYEVENT_ACT_TO_SVC);
        i.putExtra(WearConst.PARAM_KEYEVENT_DATA, new String(data));
        sendBroadcast(i);
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    /**
     * Set register event.
     *
     * @param regist Request event.
     */
    private void setRegisterEvent(final String regist) {
        if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCH_REGISTER.equals(regist)) {
            mRegisterEvent |= REGIST_FLAG_TOUCH_TOUCH;
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHSTART_REGISTER.equals(regist)) {
            mRegisterEvent |= REGIST_FLAG_TOUCH_TOUCHSTART;
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHEND_REGISTER.equals(regist)) {
            mRegisterEvent |= REGIST_FLAG_TOUCH_TOUCHEND;
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONDOUBLETAP_REGISTER.equals(regist)) {
            mRegisterEvent |= REGIST_FLAG_TOUCH_DOUBLETAP;
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHMOVE_REGISTER.equals(regist)) {
            mRegisterEvent |= REGIST_FLAG_TOUCH_TOUCHMOVE;
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHCANCEL_REGISTER.equals(regist)) {
            mRegisterEvent |= REGIST_FLAG_TOUCH_TOUCHCANCEL;
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCH_UNREGISTER.equals(regist)) {
            mRegisterEvent &= ~(REGIST_FLAG_TOUCH_TOUCH);
            if (mRegisterEvent == 0) {
                finish();
            }
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHSTART_UNREGISTER.equals(regist)) {
            mRegisterEvent &= ~(REGIST_FLAG_TOUCH_TOUCHSTART);
            if (mRegisterEvent == 0) {
                finish();
            }
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHEND_UNREGISTER.equals(regist)) {
            mRegisterEvent &= ~(REGIST_FLAG_TOUCH_TOUCHEND);
            if (mRegisterEvent == 0) {
                finish();
            }
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONDOUBLETAP_UNREGISTER.equals(regist)) {
            mRegisterEvent &= ~(REGIST_FLAG_TOUCH_DOUBLETAP);
            if (mRegisterEvent == 0) {
                finish();
            }
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHMOVE_UNREGISTER.equals(regist)) {
            mRegisterEvent &= ~(REGIST_FLAG_TOUCH_TOUCHMOVE);
            if (mRegisterEvent == 0) {
                finish();
            }
        } else if (WearConst.DEVICE_TO_WEAR_TOUCH_ONTOUCHCANCEL_UNREGISTER.equals(regist)) {
            mRegisterEvent &= ~(REGIST_FLAG_TOUCH_TOUCHCANCEL);
            if (mRegisterEvent == 0) {
                finish();
            }
        }
    }

    /**
     * Broadcast receiver.
     */
    public class MyBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(final Context context, final Intent i) {
            setRegisterEvent(i.getStringExtra(WearConst.PARAM_TOUCH_REGIST));
        }
    }
}
