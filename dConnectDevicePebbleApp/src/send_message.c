/*
 send_message.c
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#include "send_message.h"
#include "pebble_device_plugin.h"

static int retry_count = 0;

/*!
 @brief メッセージ送信処理。
 */
void send_message()
{
    DBG_LOG(APP_LOG_LEVEL_DEBUG, "send_message");

    DictionaryIterator *iter = NULL;
    app_message_outbox_begin(&iter);
    if (iter == NULL) {
        // 送信用イテレータの作成に失敗
        //entry_log( "error", "outbox_begin" ) ;
        DBG_LOG(APP_LOG_LEVEL_DEBUG, "send_message_error: outbox_begin");

        return;
    }

    int k = 0, v = 0;
    bool more = mq_kv_get_first(&k, &v);

    while (more) {
        if (k == KEY_PARAM_SETTING_DATE) {
            time_t timer = time(NULL);
            struct tm *local = localtime(&timer);
            char str[64];
            // ポインタにしないとTupletCStringがエラーを出す
            char *p = str;
            // RFC 3339に合わせて変換を行えないため、ISO8601の形式でデバイス側に渡す。デバイス側で変換する。
            strftime(str, sizeof(str), "%FT%T%z", local);
            entry_log("get setting/date", str);
            Tuplet dateTuple = TupletCString(KEY_PARAM_SETTING_DATE, p);
            dict_write_tuplet(iter, &dateTuple);
        } else {
            Tuplet tuple = TupletInteger(k, v);
            dict_write_tuplet(iter, &tuple);
        }

        more = mq_kv_get_next(&k, &v);
    }

    // データ終了
    dict_write_end(iter);
    // データ送信
    AppMessageResult res = app_message_outbox_send();
    DBG_LOG(APP_LOG_LEVEL_DEBUG, "res:%d", res);
}

/*!
 @brief メッセージ送信成功後の処理。
 */
void success_message()
{
    retry_count = 0;
    if (mq_pop()) {
        DBG_LOG(APP_LOG_LEVEL_DEBUG, "pop!!!");
        send_message();
    }
}

/*!
 @brief メッセージ送信リトライ処理。
 */
void retry_message()
{
    retry_count++;
    if (retry_count < 3) {
        send_message();
    }
}
