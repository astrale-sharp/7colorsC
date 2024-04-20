#pragma once
#include "colors/colors.h"
#include <stdlib.h>
#include <string.h>

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
  char *dest = (char*)malloc(sizeof(char) * 30);
  strcat(dest, color);
  strcat(dest, name);
  strcat(dest, BACK);
  return dest;
}