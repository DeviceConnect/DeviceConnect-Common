/////////////////////////////////////////////////////////////////////
// key_event_profile.c
//
// Key Event
//
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// include
/////////////////////////////////////////////////////////////////////
#include "pebble_device_plugin_defines.h"
#include "pebble_device_plugin.h"
#include "key_event_profile.h"
#include "device_orientation_profile.h"
#include "message_queue.h"

/////////////////////////////////////////////////////////////////////
// local define
/////////////////////////////////////////////////////////////////////
// define for key type
#define KEYTYPE_STDKEY 1
#define KEYTYPE_MEDIA 2
#define KEYTYPE_DPAD_BUTTON 3
#define KEYTYPE_USER 4

/////////////////////////////////////////////////////////////////////
// prototype
/////////////////////////////////////////////////////////////////////
static void in_received_put_key_event_handler(DictionaryIterator *received);
static void in_received_delete_key_event_handler(DictionaryIterator *received);
static void key_event_window_init();
static void key_event_window_uninit();
static void window_load(Window *me);
static void window_unload(Window *window);
static void update_text();
static void click_config_provider(void *context);
static void up_click_down_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_up_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_down_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_up_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_down_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_up_handler(ClickRecognizerRef recognizer, void *context);
static void back_click_down_handler(ClickRecognizerRef recognizer, void *context);
static void back_click_up_handler(ClickRecognizerRef recognizer, void *context);
static void in_key_event_handler(int action, int keyID);

/////////////////////////////////////////////////////////////////////
// grobal variable
/////////////////////////////////////////////////////////////////////
// window
static Window *window = NULL;

// text layer
static TextLayer *title_text_layer;
static TextLayer *keyevent_text_layer;
static TextLayer *keytype_text_layer;

// event flag control
static bool event_flag_on_down = false;
static bool event_flag_on_up = false;

// key type
static int KeyType = KEYTYPE_STDKEY;


/////////////////////////////////////////////////////////////////////
// function
/////////////////////////////////////////////////////////////////////

/*!
 @brief Message processing for key event profile.

 @param[in] received Received message.

 @retval RETURN_SYNC Synchronous.
 @retval RETURN_ASYNC Asynchronous.
 */
int in_received_key_event_handler(DictionaryIterator *received) {
    Tuple *actionTuple = dict_find(received, KEY_ACTION);
    switch (actionTuple->value->uint8) {
    case ACTION_PUT:
        entry_log2( "put", "keyevent" ) ;
        in_received_put_key_event_handler(received);
        break;
    case ACTION_DELETE:
        entry_log( "delete", "keyevent" ) ;
        in_received_delete_key_event_handler(received);
        break;
    default:
        // not support.
        pebble_set_error_code(ERROR_NOT_SUPPORT_ACTION);
        break;
    }
    return RETURN_SYNC;
}

/*!
 @brief put method, Key Event Profile message handler.

 @param[in] received Received message.
 @param[in] iter Iterator to store the response.

 */
static void in_received_put_key_event_handler(DictionaryIterator *received) {
    Tuple *attributeTuple = dict_find(received, KEY_ATTRIBUTE);
    switch (attributeTuple->value->uint8) {
    case KEY_EVENT_ATTRIBUTE_ON_DOWN:
        entry_log2( "ON_DOWN", "put_handler" ) ;
		    if (window == NULL) {
            key_event_window_init();
        }
        event_flag_on_down = true;
		    break;
    case KEY_EVENT_ATTRIBUTE_ON_UP:
        entry_log2( "ON_UP", "put_handler" ) ;
		    if (window == NULL) {
            key_event_window_init();
        }
        event_flag_on_up = true;
    		break;
    default:
        // not support
        pebble_set_error_code(ERROR_NOT_SUPPORT_ATTRIBUTE);
        break;
    }
}

/*!
 @brief delete method, Key Event Profile message handler.

 @param[in] received Received message.

 */
static void in_received_delete_key_event_handler(DictionaryIterator *received) {

    Tuple *attributeTuple = dict_find(received, KEY_ATTRIBUTE);
    switch (attributeTuple->value->uint8) {
    case KEY_EVENT_ATTRIBUTE_ON_DOWN:
        entry_log2( "ON_DOWN", "del_handler" ) ;
        event_flag_on_down = false;
		    if (event_flag_on_up == false && window != NULL) {
            key_event_window_uninit();
        }
        break;
    case KEY_EVENT_ATTRIBUTE_ON_UP:
        entry_log2( "ON_UP", "del_handler" ) ;
        event_flag_on_up = false;
		    if (event_flag_on_down == false && window != NULL) {
            key_event_window_uninit();
        }
        break;
    default:
        // not support
        entry_log( "not support", "orientation" ) ;
        pebble_set_error_code(ERROR_NOT_SUPPORT_ATTRIBUTE);
        break;
    }
}

/*!
 @brief Key Event Profile window initialize.
 */
static void key_event_window_init() {
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, true /* Animated */);  
}

/*!
 @brief Key Event Profile window uninitialize.
 */
static void key_event_window_uninit() {
    window_stack_remove(window, true /* Animated */);
    window_destroy(window);
    window = NULL;
}

/*!
 @brief Create text layer fot Key Event Profile window.

 @param[in] window Window pointer.

 */
static void window_load(Window *me) {
  Layer *layer = window_get_root_layer(me);
  const int16_t width = layer_get_frame(layer).size.w;

  title_text_layer = text_layer_create(GRect(4, 0, width, 60));
  text_layer_set_font(title_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(title_text_layer, GColorClear);
  text_layer_set_text(title_text_layer, "Key Event Profile");
  layer_add_child(layer, text_layer_get_layer(title_text_layer));

  keyevent_text_layer = text_layer_create(GRect(4, 44, width, 60));
  text_layer_set_font(keyevent_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(keyevent_text_layer, GColorClear);
  layer_add_child(layer, text_layer_get_layer(keyevent_text_layer));

  keytype_text_layer = text_layer_create(GRect(4, 44 + 28, width, 60));
  text_layer_set_font(keytype_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(keytype_text_layer, GColorClear);
  layer_add_child(layer, text_layer_get_layer(keytype_text_layer));

  update_text();
}

/*!
 @brief Destroy text layer fot Key Event Profile window.

 @param[in] window Window pointer.

 */
static void window_unload(Window *window) {
  text_layer_destroy(title_text_layer);
  text_layer_destroy(keyevent_text_layer);
  text_layer_destroy(keytype_text_layer);
}

/*!
 @brief Update text for keytype_text_layer.
 */
static void update_text() {
  static char body_text[50];
  
  switch (KeyType) {
  case KEYTYPE_STDKEY:
      snprintf(body_text, sizeof(body_text), "KeyType : STD_KEY");
      break;
  case KEYTYPE_MEDIA:
      snprintf(body_text, sizeof(body_text), "KeyType : MEDIA");
      break;
  case KEYTYPE_DPAD_BUTTON:
      snprintf(body_text, sizeof(body_text), "KeyType : DPAD / BUTTON");
      break;
  case KEYTYPE_USER:
      snprintf(body_text, sizeof(body_text), "KeyType : USER");
      break;
  }
  text_layer_set_text(keytype_text_layer, body_text);
}

/*!
 @brief Click configuration provider fot Key Event Profile window.

 @param[in] context Pointer to application specific data.

 */
static void click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_UP, up_click_down_handler, up_click_up_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_click_down_handler, select_click_up_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_click_down_handler, down_click_up_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_BACK, back_click_down_handler, back_click_up_handler, NULL);
}

/*!
 @brief Click up button down handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void up_click_down_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Up - down");
  in_key_event_handler(KEY_EVENT_ACTION_DOWN, KEY_EVENT_KEY_ID_UP);
}

/*!
 @brief Click up button up handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void up_click_up_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Up - up");
  in_key_event_handler(KEY_EVENT_ACTION_UP, KEY_EVENT_KEY_ID_UP);
}

/*!
 @brief Click select button down handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void select_click_down_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Select - down");
  in_key_event_handler(KEY_EVENT_ACTION_DOWN, KEY_EVENT_KEY_ID_SELECT);
}

/*!
 @brief Click select button up handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void select_click_up_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Select - up");
  in_key_event_handler(KEY_EVENT_ACTION_UP, KEY_EVENT_KEY_ID_SELECT);

  // Update Key Type.
  KeyType++;
  if (KeyType > KEYTYPE_USER) {
      KeyType = KEYTYPE_STDKEY;
  }
  update_text();
}

/*!
 @brief Click down button down handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void down_click_down_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Down - down");
  in_key_event_handler(KEY_EVENT_ACTION_DOWN, KEY_EVENT_KEY_ID_DOWN);
}

/*!
 @brief Click down button up handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void down_click_up_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Down - up");
  in_key_event_handler(KEY_EVENT_ACTION_UP, KEY_EVENT_KEY_ID_DOWN);
}

/*!
 @brief Click back button down handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void back_click_down_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Back - down");
  in_key_event_handler(KEY_EVENT_ACTION_DOWN, KEY_EVENT_KEY_ID_BACK);
}

/*!
 @brief Click back button up handler fot Key Event Profile window.

 @param[in] recognizer uninitializeReference to opaque click recognizer When a ClickHandler callback.
 @param[in] context Pointer to application specific data.

 */
static void back_click_up_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(keyevent_text_layer, "Back - up");
  in_key_event_handler(KEY_EVENT_ACTION_UP, KEY_EVENT_KEY_ID_BACK);
}

/*!
 @brief key event processing.

 @param[in] action Key evnet action (down or up).
 @param[in] keyID Key ID.

 */
static void in_key_event_handler(int action, int keyID) {  

    // And if in orientation running, to stop temporarily orientation event transmission.
    orientation_service_pause();

    if (!mq_push()) {
		    entry_log( "error", "in_key_event_handler" ) ;
		    return;
    }

    // Setting parameter to message queue.
    mq_kv_set(KEY_ACTION, ACTION_EVENT);
    mq_kv_set(KEY_PROFILE, PROFILE_KEY_EVENT);
	  mq_kv_set(KEY_PARAM_KEY_EVENT_KEY_TYPE, KeyType);
	  mq_kv_set(KEY_PARAM_KEY_EVENT_ID, keyID);
    switch (action) {
    case KEY_EVENT_ACTION_DOWN:
        mq_kv_set(KEY_ATTRIBUTE, KEY_EVENT_ATTRIBUTE_ON_DOWN);
        break;
    case KEY_EVENT_ACTION_UP:
        mq_kv_set(KEY_ATTRIBUTE, KEY_EVENT_ATTRIBUTE_ON_UP);
        break;
    default:
        return;
    }

    // Output runnning log. 
    {
        char title[ 20 ] ;
        snprintf( title, sizeof( title), "keyevent " ) ;
        char buf[ 30 ] ;
        snprintf( buf, sizeof( buf), "keyID:%d", keyID) ;
        replace_last_log( title, buf) ;
    }

    // Send event data.
	  send_message();
  
    // Speed up to Bluetooth communication.
    pebble_sniff_interval_normal();
}

