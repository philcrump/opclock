#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#include "main.h"
#include "clock.h"
#include "screen.h"
#include "graphics.h"
#include "font/font.h"

/** DATE **/

#define DATE_WIDTH      350
#define DATE_HEIGHT     75
static screen_pixel_t date_buffer[DATE_HEIGHT][DATE_WIDTH] __attribute__ ((aligned (NEON_ALIGNMENT))) = { 0 };

#define DATE_POS_X    15
#define DATE_POS_Y    15

/** TIME **/

#define TIME_WIDTH      350
#define TIME_HEIGHT     75
static screen_pixel_t time_buffer[TIME_HEIGHT][TIME_WIDTH] __attribute__ ((aligned (NEON_ALIGNMENT))) = { 0 };

#define TIME_POS_X    (SCREEN_WIDTH - 15 - TIME_WIDTH)
#define TIME_POS_Y    15

/** EVENTS **/

#define EVENT_LINE_WIDTH      750
#define EVENT_LINE_HEIGHT     300
#define EVENT_EACH_LINE_HEIGHT  50
static screen_pixel_t event_line_buffer[EVENT_LINE_HEIGHT][EVENT_LINE_WIDTH] __attribute__ ((aligned (NEON_ALIGNMENT))) = { 0 };

#define EVENT_LINE_POS_X    25
#define EVENT_LINE_POS_Y    110

/** EVENT SOURCE **/

#define EVENTSOURCE_WIDTH      120
#define EVENTSOURCE_HEIGHT     25
static screen_pixel_t eventsource_buffer[EVENTSOURCE_HEIGHT][EVENTSOURCE_WIDTH] __attribute__ ((aligned (NEON_ALIGNMENT))) = { 0 };

#define EVENTSOURCE_POS_X    10
#define EVENTSOURCE_POS_Y    (SCREEN_HEIGHT - 10 - EVENTSOURCE_HEIGHT)

/** EVENT STATUS **/

#define EVENTSTATUS_WIDTH      120
#define EVENTSTATUS_HEIGHT     25
static screen_pixel_t eventstatus_buffer[EVENTSTATUS_HEIGHT][EVENTSTATUS_WIDTH] __attribute__ ((aligned (NEON_ALIGNMENT))) = { 0 };

#define EVENTSTATUS_POS_X    (EVENTSOURCE_POS_X + EVENTSOURCE_WIDTH + 15)
#define EVENTSTATUS_POS_Y    (SCREEN_HEIGHT - 10 - EVENTSTATUS_HEIGHT)

const screen_pixel_t graphics_white_pixel =
{
  .Alpha = 0x80,
  .Red = 0xFF,
  .Green = 0xFF,
  .Blue = 0xFF
};

const screen_pixel_t graphics_black_pixel =
{
  .Alpha = 0x80,
  .Red = 0x00,
  .Green = 0x00,
  .Blue = 0x00
};

const screen_pixel_t graphics_subtlegrey_pixel =
{
  .Alpha = 0x80,
  .Red = 0x30,
  .Green = 0x30,
  .Blue = 0x30
};

const screen_pixel_t graphics_darkgrey_pixel =
{
  .Alpha = 0x80,
  .Red = 0x70,
  .Green = 0x70,
  .Blue = 0x70
};

const screen_pixel_t graphics_grey_pixel =
{
  .Alpha = 0x80,
  .Red = 0xA0,
  .Green = 0xA0,
  .Blue = 0xA0
};

const screen_pixel_t graphics_red_pixel =
{
  .Alpha = 0x80,
  .Red = 0xFF,
  .Green = 0x00,
  .Blue = 0x00
};

const screen_pixel_t graphics_yellow_pixel =
{
  .Alpha = 0x80,
  .Red = 0xFF,
  .Green = 0xFF,
  .Blue = 0x00
};

const screen_pixel_t graphics_blue_pixel =
{
  .Alpha = 0x80,
  .Red = 0x00,
  .Green = 0x00,
  .Blue = 0xFF
};

const screen_pixel_t graphics_amber_pixel =
{
  .Alpha = 0x80,
  .Red = 0xFF,
  .Green = 0xBF,
  .Blue = 0x00
};

static inline void date_render_font_cb(int x, int y, screen_pixel_t *pixel_ptr)
{
  memcpy(&(date_buffer[y][x]), pixel_ptr, sizeof(screen_pixel_t));
}

void date_render(char *date_string)
{
  /* Clear buffer */
  for(uint32_t i = 0; i < DATE_HEIGHT; i++)
  {
    for(uint32_t j = 0; j <DATE_WIDTH; j++)
    {
      memcpy(&(date_buffer[i][j]), &graphics_black_pixel, sizeof(screen_pixel_t));
    }
  }

  font_render_string_with_callback(
    0, // Align left
    (DATE_HEIGHT - font_dejavu_sans_72.height) / 2,
    &font_dejavu_sans_72,
    date_string,
    date_render_font_cb
  );

  for(uint32_t i = 0; i < DATE_HEIGHT; i++)
  {
    screen_setPixelLine(DATE_POS_X, DATE_POS_Y + i, DATE_WIDTH, date_buffer[i]);
  }
}

static inline void time_render_font_cb(int x, int y, screen_pixel_t *pixel_ptr)
{
  memcpy(&(time_buffer[y][x]), pixel_ptr, sizeof(screen_pixel_t));
}

void time_render(char *time_string)
{
  /* Clear buffer */
  for(uint32_t i = 0; i < TIME_HEIGHT; i++)
  {
    for(uint32_t j = 0; j <TIME_WIDTH; j++)
    {
      memcpy(&(time_buffer[i][j]), &graphics_black_pixel, sizeof(screen_pixel_t));
    }
  }

  font_render_string_with_callback(
    TIME_WIDTH - font_width_string(&font_dejavu_sans_72, time_string), // Align right
    (TIME_HEIGHT - font_dejavu_sans_72.height) / 2,
    &font_dejavu_sans_72,
    time_string,
    time_render_font_cb
  );

  for(uint32_t i = 0; i < TIME_HEIGHT; i++)
  {
    screen_setPixelLine(TIME_POS_X, TIME_POS_Y + i, TIME_WIDTH, time_buffer[i]);
  }
}


static inline void event_line_render_font_cb(int x, int y, screen_pixel_t *pixel_ptr)
{
  memcpy(&(event_line_buffer[y][x]), pixel_ptr, sizeof(screen_pixel_t));
}

static void event_line_graphic_transition(int x, int y, int height, int width, const screen_pixel_t *pixel_start_ptr, const screen_pixel_t *pixel_end_ptr)
{
  /* For each row */
  for(int i = 0; i < height; i++)
  {
    /* For each pixel in row */
    for(int j = 0; j < width; j++)
    {
      if(((float)(j)/width) < ((float)(height-i)/height))
      {
        memcpy(&(event_line_buffer[i+y][j+x]), pixel_start_ptr, sizeof(screen_pixel_t));
      }
      else
      {
        memcpy(&(event_line_buffer[i+y][j+x]), pixel_end_ptr, sizeof(screen_pixel_t));
      }
    }
  }
}

void event_line_render(event_t *events_ptr, bool show_previous_expired_event)
{
  /* Clear all lines buffer */
  for(uint32_t i = 0; i < EVENT_LINE_HEIGHT; i++)
  {
    for(uint32_t j = 0; j < EVENT_LINE_WIDTH; j++)
    {
      memcpy(&(event_line_buffer[i][j]), &graphics_black_pixel, sizeof(screen_pixel_t));
    }
  }

  event_t *event_cursor;
  event_t *event_previous_cursor;

  /* Find the next event */

  event_cursor = events_ptr;
  event_previous_cursor = NULL;

  while(event_cursor != NULL
    && event_cursor->next != NULL
    && event_cursor->countdown_int > 0)
  {
    event_previous_cursor = event_cursor;
    event_cursor = event_cursor->next;
  }

  if(event_cursor == NULL || event_cursor->countdown_int >= 60)
  {
    /* No events or newest event is more than 1 minute ago */

    /* Draw message, render early, and exit */
    font_render_string_with_callback(
      0, // Align left
      10, // Top
      &font_dejavu_sans_36,
      "No Events scheduled",
      event_line_render_font_cb
    );
    for(uint32_t i = 0; i < EVENT_LINE_HEIGHT; i++)
    {
      screen_setPixelLine(EVENT_LINE_POS_X, EVENT_LINE_POS_Y + i, EVENT_LINE_WIDTH, event_line_buffer[i]);
    }
    return;
  }
  else if(show_previous_expired_event
    && event_previous_cursor != NULL)
  {
    /* If a previous event exists, roll the cursor back one */
    event_cursor = event_previous_cursor;
  }

  /* Render event list */
  int events_displayed = 0;
  screen_pixel_t *text_colour_ptr;
  while(event_cursor != NULL && events_displayed <= 6)
  {
    /* This is an ugly thing to blank the top line when the next line is == 0, so just about to tick over */
    if(false == (
         show_previous_expired_event
      && events_displayed == 0
      && event_cursor->next != NULL
      && event_cursor->next->countdown_int == 0))
    {
      /* Get text colour from urgency */
      if(event_cursor->countdown_int > 0)
      {
        text_colour_ptr = (screen_pixel_t *)&graphics_darkgrey_pixel;
      }
      else if(event_cursor->countdown_int == 0)
      {
        text_colour_ptr = (screen_pixel_t *)&graphics_red_pixel;
      }
      else if(event_cursor->countdown_int > -60)
      {
        text_colour_ptr = (screen_pixel_t *)&graphics_amber_pixel;
      }
      else
      {
        text_colour_ptr = (screen_pixel_t *)&graphics_white_pixel;
      }

      /* Render description text */
      font_render_colour_string_with_callback(
        0, // Align left
        (events_displayed * EVENT_EACH_LINE_HEIGHT),
        &font_dejavu_sans_36, &graphics_black_pixel, text_colour_ptr,
        event_cursor->description, event_line_render_font_cb
      );

      /* Draw graphic for all events now or in the future */
      if(event_cursor->countdown_int <= 0)
      {
        if(event_cursor->type == 1)
        {
          // AoS
          event_line_graphic_transition(410, (events_displayed * EVENT_EACH_LINE_HEIGHT)+5, 30, 100, &graphics_black_pixel, &graphics_blue_pixel);
        }
        else if(event_cursor->type == 2)
        {
          // LoS
          event_line_graphic_transition(410, (events_displayed * EVENT_EACH_LINE_HEIGHT)+5, 30, 100, &graphics_blue_pixel, &graphics_black_pixel);
        }
        else if(event_cursor->type == 3)
        {
          // Uplink Start
          event_line_graphic_transition(410, (events_displayed * EVENT_EACH_LINE_HEIGHT)+5, 30, 100, &graphics_black_pixel, &graphics_yellow_pixel);
        }
        else if(event_cursor->type == 4)
        {
          // Uplink Stop
          event_line_graphic_transition(410, (events_displayed * EVENT_EACH_LINE_HEIGHT)+5, 30, 100, &graphics_yellow_pixel, &graphics_black_pixel);
        }
      }

      /* Draw countdown timer string */
      font_render_colour_string_with_callback(
        EVENT_LINE_WIDTH - font_width_string(&font_dejavu_sans_36, event_cursor->countdown_string), // Align right
        (events_displayed * EVENT_EACH_LINE_HEIGHT),
        &font_dejavu_sans_36, &graphics_black_pixel, text_colour_ptr,
        event_cursor->countdown_string, event_line_render_font_cb
      );
    }

    /* Draw separation line */
    if(events_displayed != 0)
    {
      for(int i = 2; i < (EVENT_LINE_WIDTH-2); i++)
      {
        memcpy(&event_line_buffer[(events_displayed * EVENT_EACH_LINE_HEIGHT)-3][i], &graphics_subtlegrey_pixel, sizeof(screen_pixel_t));
      }
    }

    event_cursor = event_cursor->next;
    events_displayed++;
  }

  for(uint32_t i = 0; i < EVENT_LINE_HEIGHT; i++)
  {
    screen_setPixelLine(EVENT_LINE_POS_X, EVENT_LINE_POS_Y + i, EVENT_LINE_WIDTH, event_line_buffer[i]);
  }
}


static inline void eventsource_render_font_cb(int x, int y, screen_pixel_t *pixel_ptr)
{
  memcpy(&(eventsource_buffer[y][x]), pixel_ptr, sizeof(screen_pixel_t));
}

void eventsource_render(char *text_string)
{
  /* Clear buffer */
  for(uint32_t i = 0; i < EVENTSOURCE_HEIGHT; i++)
  {
    for(uint32_t j = 0; j < EVENTSOURCE_WIDTH; j++)
    {
      memcpy(&(eventsource_buffer[i][j]), &graphics_black_pixel, sizeof(screen_pixel_t));
    }
  }

  font_render_colour_string_with_callback(
    0, // Align left
    (EVENTSOURCE_HEIGHT - font_dejavu_sans_16.height) / 2,
    &font_dejavu_sans_16,
    &graphics_black_pixel, &graphics_darkgrey_pixel,
    text_string,
    eventsource_render_font_cb
  );

  for(uint32_t i = 0; i < EVENTSOURCE_HEIGHT; i++)
  {
    screen_setPixelLine(EVENTSOURCE_POS_X, EVENTSOURCE_POS_Y + i, EVENTSOURCE_WIDTH, eventsource_buffer[i]);
  }
}

static inline void eventstatus_render_font_cb(int x, int y, screen_pixel_t *pixel_ptr)
{
  memcpy(&(eventstatus_buffer[y][x]), pixel_ptr, sizeof(screen_pixel_t));
}

void eventstatus_render(char *text_string, bool status_ok)
{
  /* Clear buffer */
  for(uint32_t i = 0; i < EVENTSTATUS_HEIGHT; i++)
  {
    for(uint32_t j = 0; j < EVENTSTATUS_WIDTH; j++)
    {
      memcpy(&(eventstatus_buffer[i][j]), &graphics_black_pixel, sizeof(screen_pixel_t));
    }
  }

  screen_pixel_t *text_colour_ptr;

  if(status_ok)
  {
    text_colour_ptr = (screen_pixel_t *)&graphics_darkgrey_pixel;
  }
  else
  {
    text_colour_ptr = (screen_pixel_t *)&graphics_red_pixel;
  }

  font_render_colour_string_with_callback(
    0, // Align left
    (EVENTSTATUS_HEIGHT - font_dejavu_sans_16.height) / 2,
    &font_dejavu_sans_16,
    &graphics_black_pixel, text_colour_ptr,
    text_string,
    eventstatus_render_font_cb
  );

  for(uint32_t i = 0; i < EVENTSTATUS_HEIGHT; i++)
  {
    screen_setPixelLine(EVENTSTATUS_POS_X, EVENTSTATUS_POS_Y + i, EVENTSTATUS_WIDTH, eventstatus_buffer[i]);
  }
}