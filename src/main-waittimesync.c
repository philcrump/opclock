#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "main.h"
#include "backlight.h"
#include "screen.h"
#include "graphics.h"

#define INTERFACE_NAME  "eth0"

int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;

  char *splash_comment_string;
  int r;

  /* Open a socket to get the network address */
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(fd >= 0)
  {
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, INTERFACE_NAME, IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    r = asprintf(&splash_comment_string, "%s - Waiting for time sync..", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
  }
  else
  {
    fprintf(stderr, "Error getting network address!\n");
    r = asprintf(&splash_comment_string, "Waiting for time sync..");
  }

  if(r < 0)
  {
    fprintf(stderr, "[main] Failed to allocate splash comment string, exiting..\n");
    return 1;
  }

  /* Switch on screen */
  backlight_power(true);

  /* Initialise screen and splash */
  if(!screen_init(true, splash_comment_string))
  {
    fprintf(stderr, "Error initialising screen!\n");
    return 1;
  }

  screen_deinit();

  return 0;
}
