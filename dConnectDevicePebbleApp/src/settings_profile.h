/*
 settings_profile.h
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#ifndef settings_profile_h
#define settings_profile_h

/*!
 @brief Settingsプロファイルのメッセージを処理する.

 @param[in] received 受信したメッセージ
 @param[in] iter レスポンスを格納するイテレータ

 @retval RETURN_SYNC 同期
 @retval RETURN_ASYNC 非同期
 */
int in_received_setting_handler(DictionaryIterator *received);

#endif	/* settings_profile_h */
