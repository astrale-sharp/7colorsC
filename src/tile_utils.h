#pragma once
#include "colors.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tile_to_char(enum Tile *t);

void print_tile_as_board(enum Tile *t);


enum Tile get_random_tile();

// may return NULL
enum Tile *char_to_tile(char c);