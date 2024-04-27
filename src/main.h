#include "board_utils.h"
#include "colors.h"
#include "player_utils.h"
#include "tile_utils.h"
#include "types.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct World;
struct PlayerInfo;

struct World world__new(size_t board_size, int active_player);

struct PlayerInfo player_info(struct World *w);

char get_input();
enum Tile *get_tile_choice();
int can_play(struct World *w, struct PlayerInfo *info);

void play_move(struct World *w, struct PlayerInfo *info, Tile choice);
int play_against_ia(size_t board_size);