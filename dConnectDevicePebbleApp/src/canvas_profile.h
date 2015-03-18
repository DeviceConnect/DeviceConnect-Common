/*
 canvas_profile.h
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#ifndef canvas_profile_h
#define canvas_profile_h

/*!
 @brief バッテリープロファイルの処理を行う.

 @param received 受信したメッセージデータ
 @param iter レスポンスを格納するイテレータ

 @retval RETURN_SYNC 同期
 @retval RETURN_ASYNC 非同期
 */
int in_received_canvas_handler(DictionaryIterator *received);

#endif  /* canvas_profile_h */
