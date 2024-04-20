#pragma once
#include "colors/colors.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tile_to_char(enum Tile *t) {
  switch (*t) {

  case BLUE:
    return 'B';
  case CYAN:
    return 'C';
  case GREEN:
    return 'G';
  case ORANGE:
    return 'O';
  case WHITE:
    return 'W';
  case RED_TOPRIGHT:
    return ' ';
  case PURPLE_BOTTOMLEFT:
    return ' ';
  }
}

void print_tile_as_board(enum Tile *t) {
  switch (*t) {

  case BLUE:
    printf(BgBlue);
    break;
  case CYAN:
    printf(BgCyan);
    break;
  case GREEN:
    printf(BgGreen);
    break;
  case ORANGE:
    printf(BgOrange);
    break;
  case WHITE:
    // printf(BgWhite);
    break;
  case RED_TOPRIGHT:
    printf(BgRed);
    break;

  case PURPLE_BOTTOMLEFT:
    printf(BgPurple);
    break;
  }
  printf(" %c ", tile_to_char(t));
  printf(BACK);
}

enum Tile get_random_tile() { return (Tile)(random() % 5); }

// may return NULL
enum Tile *char_to_tile(char c) {
  enum Tile *t = (Tile *)malloc(sizeof(enum Tile));
  switch (c) {
    // python macro
    // l = [["BLUE", "b", "B", "1", "&"],["CYAN", "c", "C", "2"],["GREEN",
    // "g", "G", "3", "\""],["ORANGE", "o", "O", "4", "\\\'"],["WHITE", "w",
    // "W", "5", "("],] for x in l:
    //     head = x[0]
    //     for case in (x[1:]):
    //         print( end="\t"
    //             "case '%s' : {*t = %s; break;}" % (case, head),
    //             )
    //     print()
  case 'b': {
    *t = BLUE;
    break;
  }
  case 'B': {
    *t = BLUE;
    break;
  }
  case '1': {
    *t = BLUE;
    break;
  }
  case '&': {
    *t = BLUE;
    break;
  }
  case 'c': {
    *t = CYAN;
    break;
  }
  case 'C': {
    *t = CYAN;
    break;
  }
  case '2': {
    *t = CYAN;
    break;
  }

  case 'g': {
    *t = GREEN;
    break;
  }
  case 'G': {
    *t = GREEN;
    break;
  }
  case '3': {
    *t = GREEN;
    break;
  }
  case '"': {
    *t = GREEN;
    break;
  }
  case 'o': {
    *t = ORANGE;
    break;
  }
  case 'O': {
    *t = ORANGE;
    break;
  }
  case '4': {
    *t = ORANGE;
    break;
  }
  case '\'': {
    *t = ORANGE;
    break;
  }
  case 'w': {
    *t = WHITE;
    break;
  }
  case 'W': {
    *t = WHITE;
    break;
  }
  case '5': {
    *t = WHITE;
    break;
  }
  case '(': {
    *t = WHITE;
    break;
  }

  ////////////////////
  default: {
    if ((int)c == (int)*"é") {
      *t = CYAN;
      break;
    }
    return NULL;
  }
  }
  return t;
}