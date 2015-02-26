/*
 message_queue.h
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#ifndef message_queue_h
#define message_queue_h

#include <stdbool.h>

/*!
 @brief 値をクリア。
 */
void mq_kv_clear();

/*!
 @brief 値を設定。

 @param[in] key キー。
 @param[in] value 値。
 */
void mq_kv_set(int key, int value);

/*!
 @brief 次の値を取得。

 @param[in] key キー。
 @param[in] value 値。

 @return true:値あり、false:値無し。
 */
bool mq_kv_get_next(int *key, int *value);

/*!
 @brief 最初の値を取得。

 @param[in] key キー。
 @param[in] value 値。

 @return true:値あり、false:値無し。
 */
bool mq_kv_get_first(int *key, int *value);

/*!
 @brief キューを初期化。
*/
void mq_init();

/*!
 @brief 次の要素の添え字を求める。

 @param[in] index インデックス。

 @return 要素。
 */
int mq_next(int index);

/*!
 @brief キューをプッシュ。

 @return true:成功、false:失敗。
 */
bool mq_push();

/*!
 @brief キューをポップ。

 @return true:成功、false:キューが空。
 */
bool mq_pop();

#endif	/* message_queue_h */
