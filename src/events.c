#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "events.h"

void events_append(event_t **events_ptr_ptr, time_t event_time, char* event_description, int event_type)
{
  event_t *new_event_ptr = (event_t *)malloc(sizeof(event_t));

  if(new_event_ptr == NULL)
  {
    fprintf(stderr, "Error creating new event.\n");
    return;
  }

  new_event_ptr->event_time = event_time;
  new_event_ptr->description = strdup(event_description);
  new_event_ptr->type = event_type;
  new_event_ptr->countdown_int = 0;
  new_event_ptr->countdown_string = NULL;
  new_event_ptr->next = NULL;
  if(*events_ptr_ptr == NULL)
  {
    /* First event */
    *events_ptr_ptr = new_event_ptr;
  }
  else
  {
    event_t *event_cursor;

    /* Find the end of the list */
    event_cursor = *events_ptr_ptr;
    while(event_cursor->next != NULL)
    {
      event_cursor = event_cursor->next;
    }

    /* Append the new event */
    event_cursor->next = new_event_ptr;
  }
}

void events_delete(event_t *events_ptr)
{
  if(events_ptr == NULL)
  {
    return;
  }

  event_t *event_cursor, *event_next;

  event_cursor = events_ptr;
  while(event_cursor != NULL)
  {
    event_next = event_cursor->next;

    free(event_cursor);

    event_cursor = event_next;
  }
}