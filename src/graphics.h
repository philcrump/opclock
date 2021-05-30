#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

void date_render(char *date_string);

void time_render(char *time_string);

void event_line_render(event_t *events_ptr, bool show_previous_expired_event);

void eventsource_render(char *text_string);

void eventstatus_render(char *text_string, bool status_ok);

#endif /* __GRAPHICS_H__ */