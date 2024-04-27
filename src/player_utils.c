#include "player_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

char *get_player_name(int active_player) {

  const char *color;
  const char *name;
  if (active_player == 0) {
    color = BgRed;
    name = "top right";
  } else {
    color = BgPurple;
    name = "bottom left";
  }
  char* dest = (char*)(malloc(40*sizeof(char)));
  sprintf(dest, "%s%s%s", color, name, BACK);

  return dest;
}