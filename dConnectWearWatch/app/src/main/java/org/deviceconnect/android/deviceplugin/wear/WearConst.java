/*
WearConst.java
Copyright (c) 2014 NTT DOCOMO,INC.
Released under the MIT license
http://opensource.org/licenses/mit-license.php
 */
package org.deviceconnect.android.deviceplugin.wear;

/**
 * Wear Const.
 *
 * @author NTT DOCOMO, INC.
 */
public final class WearConst {

    /** Broadcast Intent Key. */
    public static final String ACTION_WEAR_PING_SERVICE = "org.deviceconnect.wear.ping.service";

    /** Vibration start. */
    public static final String DEVICE_TO_WEAR_VIBRATION_RUN = "org.deviceconnect.wear.vibration.run";

    /** Vibration stop. */
    public static final String DEVICE_TO_WEAR_VIBRATION_DEL = "org.deviceconnect.wear.vibration.del";

    /** when DeviceOrientation register is opened. */
    public static final String DEVICE_TO_WEAR_DEIVCEORIENTATION_REGISTER
            = "org.deviceconnect.wear.deivceorienatation.regist";

    /** when DeviceOrientation unregister is opened. */
    public static final String DEVICE_TO_WEAR_DEIVCEORIENTATION_UNREGISTER
            = "org.deviceconnect.wear.deivceorienatation.unregist";

    /** Wear to Android. */
    public static final String WEAR_TO_DEVICE_DEIVCEORIENTATION_DATA = "org.deviceconnect.wear.deivceorienatation.data";

    /** Register Key Event (ondown). */
    public static final String DEVICE_TO_WEAR_KEYEVENT_ONDOWN_REGISTER
            = "org.deviceconnect.wear.keyevent.ondown.regist";

    /** Register Key Event (onup). */
    public static final String DEVICE_TO_WEAR_KEYEVENT_ONUP_REGISTER = "org.deviceconnect.wear.keyevent.onup.regist";

    /** Remove Key Event (ondown). */
    public static final String DEVICE_TO_WEAR_KEYEVENT_ONDOWN_UNREGISTER
            = "org.deviceconnect.wear.keyevent.ondown.unregist";

    /** Remove Key Event (onup). */
    public static final String DEVICE_TO_WEAR_KEYEVENT_ONUP_UNREGISTER
            = "org.deviceconnect.wear.keyevent.onup.unregist";

    /** Wear to Android. */
    public static final String WEAR_TO_DEVICE_KEYEVENT_DATA = "org.deviceconnect.wear.keyevent.data";

    /** Broadcast Intent Key. */
    public static final String PARAM_DC_WEAR_KEYEVENT_SVC_TO_ACT = "DC_WEAR_KEYEVENT_SVC_TO_ACT";

    /** Broadcast Intent Key. */
    public static final String PARAM_DC_WEAR_KEYEVENT_ACT_TO_SVC = "DC_WEAR_KEYEVENT_ACT_TO_SVC";

    /** Key Event (down). */
    public static final String PARAM_KEYEVENT_DOWN = "down";

    /** Key Event (up). */
    public static final String PARAM_KEYEVENT_UP = "up";

    /** Broadcast Data Key (Regist). */
    public static final String PARAM_KEYEVENT_REGIST = "keyeventRegist";

    /** Broadcast Data Key (Data). */
    public static final String PARAM_KEYEVENT_DATA = "data";

    /** Key Type (Standard Keyboard). */
    public static final int KEYTYPE_STD_KEY = 0x00000000;

    /** Key Type (Media Control). */
    public static final int KEYTYPE_MEDIA_CTRL = 0x00000200;

    /** Key Type (Directional Pad / Button). */
    public static final int KEYTYPE_DPAD_BUTTON = 0x00000400;

    /** Key Type (User Define). */
    public static final int KEYTYPE_USER = 0x00000800;

    /**
     * Constructor.
     */
    private WearConst() {
    }
}
