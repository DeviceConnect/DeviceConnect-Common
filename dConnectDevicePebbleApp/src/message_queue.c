/*
 message_queue.c
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#include "message_queue.h"

#define QUEUE_SIZE 5
#define KEY_SIZE 7

typedef struct key_value_data {
    int key;
    int value;
} key_value_data_t;

static key_value_data_t queue[QUEUE_SIZE][KEY_SIZE] = {};

static int front;
static int rear;
static int current;
static int set_index;
static int get_index;

/*!
 @brief 値をクリア。
 */
void mq_kv_clear()
{
    for (int i = 0; i < KEY_SIZE; i++) {
        key_value_data_t kv = {0};
        queue[current][i] = kv;
    }
    set_index = 0;
}

/*!
 @brief 値を設定。

 @param[in] key キー。
 @param[in] value 値。
 */
void mq_kv_set(int key, int value)
{
    if (set_index < KEY_SIZE) {
        key_value_data_t kv;
        kv.key = key;
        kv.value = value;
        queue[current][set_index++] = kv;
    } else {
        //printf("*ERROR!!*\n");
        // assertion!!
    }
}

/*!
 @brief 次の値を取得。

 @param[in] key キー。
 @param[in] value 値。

 @return true:値あり、false:値無し。
 */
bool mq_kv_get_next(int *key, int *value)
{
    if (get_index < KEY_SIZE) {
        key_value_data_t kv = queue[front][get_index++];
        *key = kv.key;
        *value = kv.value;
    } else {
        return false;
    }
    return true;
}

/*!
 @brief 最初の値を取得。

 @param[in] key キー。
 @param[in] value 値。

 @return true:値あり、false:値無し。
 */
bool mq_kv_get_first(int *key, int *value)
{
    get_index = 0;
    return mq_kv_get_next(key, value);
}

/*!
 @brief キューを初期化。
*/
void mq_init()
{
    front = rear = current = 0;
    mq_kv_clear();
}

/*!
 @brief 次の要素の添え字を求める。

 @param[in] index インデックス。

 @return 要素。
 */
int mq_next(int index)
{
    return (index + 1) % QUEUE_SIZE;
}

/*!
 @brief キューをプッシュ。

 @return true:成功、false:失敗。
 */
bool mq_push()
{
    if (mq_next(rear) == front) {
        // キューが一杯
        return false;
    }
    current = rear;
    rear = mq_next(rear);
    mq_kv_clear();
    return true;
}

/*!
 @brief キューをポップ。

 @return true:成功、false:キューが空。
 */
bool mq_pop()
{
    if (mq_next(front) == rear) {
        // キューが空
        //printf("emp!!\n");
        front = rear;
        return false;
    }
    front = mq_next(front);
    return true;
}
