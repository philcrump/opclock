#ifndef __EVENTS_H__
#define __EVENTS_H__

void events_append(event_t **events_ptr_ptr, time_t event_time, char* event_description, int event_type);
void events_delete(event_t *events_ptr);

void *events_demo_thread(void *arg);
void *events_http_thread(void *arg);

#endif /* __EVENTS_H__ */