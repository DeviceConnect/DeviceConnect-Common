/*
 send_message.h
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#ifndef send_message_h
#define send_message_h

/*!
 @brief メッセージ送信処理。
 */
void send_message();

/*!
 @brief メッセージ送信成功後の処理。
 */
void success_message();

/*!
 @brief メッセージ送信リトライ処理。
 */
void retry_message();

#endif	/* send_message_h */
