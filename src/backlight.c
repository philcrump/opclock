#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

#define BACKLIGHT_LEVEL_SCRIPTPATH  "./backlight_level.sh"
#define BACKLIGHT_LEVEL_MAX 180
#define BACKLIGHT_LEVEL_MIN 30

#define BACKLIGHT_POWER_SCRIPTPATH  "./backlight_power.sh"

void backlight_level(int32_t level_value)
{
  if(level_value > BACKLIGHT_LEVEL_MAX) level_value = BACKLIGHT_LEVEL_MAX;
  if(level_value < BACKLIGHT_LEVEL_MIN) level_value = BACKLIGHT_LEVEL_MIN;

  char *level_command_string;

  asprintf(&level_command_string, "%s %d >/dev/null", BACKLIGHT_LEVEL_SCRIPTPATH, level_value);

  system(level_command_string);

  free(level_command_string);
}

void backlight_power(bool on)
{
  char *power_command_string;

  if(on)
  {
    asprintf(&power_command_string, "%s 0  >/dev/null", BACKLIGHT_POWER_SCRIPTPATH);
  }
  else
  {
    asprintf(&power_command_string, "%s 1 >/dev/null", BACKLIGHT_POWER_SCRIPTPATH);
  }

  system(power_command_string);

  free(power_command_string);
}