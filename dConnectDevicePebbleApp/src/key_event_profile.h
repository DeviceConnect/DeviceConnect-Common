#ifndef key_event_profile_h
#define key_event_profile_h

/*!
 @brief Message processing for key event profile.

 @param[in] received Received message.

 @retval RETURN_SYNC Synchronous.
 @retval RETURN_ASYNC Asynchronous.
 */
int in_received_key_event_handler(DictionaryIterator *received);

#endif	/* key_event_profile_h */
