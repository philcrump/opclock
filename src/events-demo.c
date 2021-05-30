#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "main.h"
#include "events.h"
#include "util/timing.h"

void *events_demo_thread(void *arg)
{
  app_state_t *app_state_ptr = (app_state_t *)arg;

  time_t event_time;
  time(&event_time);

  pthread_mutex_lock(&app_state_ptr->events_mutex);

  events_append(&app_state_ptr->events, event_time+3, "AoS ISS", 1);

  events_append(&app_state_ptr->events, event_time+12, "Uplink Start", 3);

  events_append(&app_state_ptr->events, event_time+65, "Second questions", 0);

  events_append(&app_state_ptr->events, event_time+75, "Uplink Stop", 4);

  events_append(&app_state_ptr->events, event_time+139, "LoS ISS", 2);

  pthread_mutex_unlock(&app_state_ptr->events_mutex);

  app_state_ptr->events_source_ok = true;

  while(!app_state_ptr->app_exit)
  {
    sleep_ms(100);
  }

  return NULL;
}