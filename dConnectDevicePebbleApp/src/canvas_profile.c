/*
 canvas_profile.c
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#include "pebble_device_plugin_defines.h"
#include "pebble_device_plugin.h"
#include "canvas_profile.h"

/*!
 @brief deleteメソッドのメッセージを処理する.

 @param[in] received 受信したメッセージ
 @param[in] iter レスポンスを格納するイテレータ

 */
static void in_received_delete_canvas_handler(DictionaryIterator *received) {
    DBG_LOG(APP_LOG_LEVEL_DEBUG, "in_received_delete_canvas_handler");

    Tuple *attributeTuple = dict_find(received, KEY_ATTRIBUTE);
    switch (attributeTuple->value->uint8) {
    case CANVAS_DRAW_IMAGE:
        pebble_hidden_bitmap();
        break;
    default:
        pebble_set_error_code(ERROR_NOT_SUPPORT_ATTRIBUTE);
        break;
    }
}

int in_received_canvas_handler(DictionaryIterator *received) {
    DBG_LOG(APP_LOG_LEVEL_DEBUG, "in_received_canvas_handler");

    Tuple *actionTuple = dict_find(received, KEY_ACTION);
    switch (actionTuple->value->uint8) {
    case ACTION_DELETE:
        entry_log("delete bitmap", "start");
        in_received_delete_canvas_handler(received);
        break;
    case ACTION_GET:
    case ACTION_PUT:
    case ACTION_POST:
    default:
        entry_log("canvas ", "NOT_SUPPORT");
        pebble_set_error_code(ERROR_NOT_SUPPORT_ACTION);
        break;
    }
    return RETURN_SYNC;
}
