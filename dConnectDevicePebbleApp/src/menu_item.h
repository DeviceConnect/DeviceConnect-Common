/*
 menu_item.h
 Copyright (c) 2014 NTT DOCOMO,INC.
 Released under the MIT license
 http://opensource.org/licenses/mit-license.php
 */

#ifndef menu_item_h
#define menu_item_h


typedef struct __attribute__((__packed__)) {
    bool is_gbitmap ; // Gbitmap(image), text
    char* title ;      // title
    void* bitmap_or_text ;
} MenuItem ;

/*!
 @brief メニューの初期化。
 */
void init_menu_item( void ) ;

/*!
 @brief メニューを追加する。

 @param[in] titleStr メニューのタイトル
 @param[in] contents 文字列または gbitmap
 @param[in] is_gbitmap ;
 */
void entry_menu_item( char* titleStr, void* contents, bool is_gbitmap );

/*!
 @brief 最新のメニューをリプレースする。

 @param[in] titleStr メニューのタイトル
 @param[in] contents 文字列または gbitmap
 @param[in] is_gbitmap ;
 */
void replace_menu_item( char* title, void* contents, bool is_gbitmap );

/*!
 @brief indexで指定されたMenuItem構造体を返す。

 @param[in] index メニューの番号
 @return[out] index に対応する、MenuItem構造体
 */
MenuItem* get_menu_item( int index );

/*!
 @brief メニューの総数を返す。

 @return[out] メニューの総数を返す
 */
int how_many_menu_item( void );


/*!
 @brief メニュー使用後の処理を行う。
 */
void menu_cleanup( void );


#endif	/* menu_item_h */
