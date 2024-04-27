#include "main.h"
#include "colors/colors.h"
#include "player_utils.h"
#include "tile_utils.h"
#include "types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct World {
  Tile **board;
  size_t board_size;
  Tile *choice;
  Point *top_right_tiles;
  Point *bottom_left_tiles;
};

struct PlayerInfo {
  int *active_player;
  Point *owned_line;
  size_t *line_len;
  Point *converted;
  Tile color;
  int is_ai;
};

void print_board(enum Tile **board, size_t board_size) {
  for (int x = 0; x < board_size; x++) {
    for (int y = 0; y < board_size; y++) {
      // printf(" ");
      print_tile_as_board(&board[x][y]);
      // printf(" ");
    }
    // printf("\n");
    printf("\n");
  }
}

char get_input() {
  char *line = NULL;
  size_t lineSize = 0;
  size_t len = 1;
  lineSize = getline(&line, &len, stdin);
  if (lineSize == 0) {
    free(line);
    return '\n';
  }
  char c = line[0];
  free(line);
  return c;
}

enum Tile *get_tile_choice() {
  enum Tile *choice = NULL;

  while (choice == NULL) {
    printf("please input a character.\n");
    printf("%s%s", BLINK, BACK);
    char c = get_input();
    choice = char_to_tile(c);
    if (choice == NULL) {
      printf("incorrect char %c - %i\nPress enter to continue", c, c);
      char _ = get_input();
      continue;
    } else {
      printf("get tile choice: %c\n", tile_to_char(choice));
    }
  }
  return choice;
}

// put candidate in target if
// - is of right tile color and
// - is in the board and
// - a clone isn't already in the target
void filter_put(Tile **board, size_t board_size, Tile chosen_tile,
                Point candidate, Point *target, size_t *target_len,
                Point *gen_pool, size_t *gen_pool_len) {

  if (candidate.x < 0 || candidate.x >= board_size || candidate.y < 0 ||
      candidate.y >= board_size) {
    return;
  }

  if (chosen_tile != board[candidate.x][candidate.y]) {
    return;
  }

  int filter = 0;
  for (int i = 0; i < *target_len; i++) {
    Point p = target[i];
    if (p.x == candidate.x && p.y == candidate.y) {
      filter = 1;
      break;
    }
  }

  if (filter == 0) {
    target[*target_len] = candidate;
    (*target_len)++;
  }
}

Point *get_4points_around(Point p) {
  Point *res = malloc(4 * sizeof(Point));
  res[0] = (Point){p.x, p.y + 1};
  res[1] = (Point){p.x, p.y - 1};
  res[2] = (Point){p.x + 1, p.y};
  res[3] = (Point){p.x - 1, p.y};

  return res;
}

void play_move(Tile **board, size_t board_size, Tile *choice,
               Point *this_owned_line, size_t *this_len, Point *converted,
               size_t *converted_len, int *active_player, Tile player_color,
               int is_ai) {
  size_t save_len = *this_len;
  for (int i = 0; i < *this_len; i++) {
    Point p = this_owned_line[i];

    for (int d = 0; d < *this_len; d++) {
    }
    // 4 elements
    Point *neighbors = get_4points_around(p);

    for (int j = 0; j < 4; j++) {
      Point this_neighbor = neighbors[j];

      filter_put(board, board_size, *choice, this_neighbor, converted,
                 converted_len, this_owned_line, this_len);
    }
    free(neighbors);
    for (int j = 0; j < *converted_len; j++) {
      Point c = converted[j];
      int filter = 0;

      for (int k = 0; k < *this_len; k++) {
        Point p = this_owned_line[k];
        if (p.x == c.x && p.y == c.y) {
          filter = 1;
          break;
        }
      }
      if (filter == 0) {
        board[c.x][c.y] = player_color;
        this_owned_line[*this_len] = c;
        (*this_len)++;
      }
    }
  }

  if (save_len != *this_len) {
    if (*active_player == 0) {
      *active_player = 1;
    } else {
      *active_player = 0;
    }
  } else {
    if (!is_ai) {
      printf("EMPTY MOVE ARE INVALID, PRESS ENTER TO SELECT AGAIN\n");
      get_input();
    }
  }
}

int play(size_t board_size) {
  // use the time to set the seed
  srand(time(NULL));

  // o is top right, 1 is bottomleft
  int active_player = 0;

  enum Tile **board = malloc(board_size * sizeof(enum Tile *));
  // fill the board with random tiles
  for (int x = 0; x < board_size; x++) {
    enum Tile *line = malloc(board_size * sizeof(enum Tile));
    for (int y = 0; y < board_size; y++) {
      line[y] = get_random_tile();
    }
    board[x] = line;
  }
  // set top right and bottom left
  board[board_size - 1][0] = PURPLE_BOTTOMLEFT;
  board[0][board_size - 1] = RED_TOPRIGHT;

  Point *top_right_tiles = malloc(board_size * board_size * sizeof(Point));
  Point *bottom_left_tiles = malloc(board_size * board_size * sizeof(Point));

  top_right_tiles[0] = (Point){.x = 0, .y = board_size - 1};
  size_t top_right_tiles_len = 1;

  bottom_left_tiles[0] = (Point){.x = board_size - 1, .y = 0};
  size_t bottom_left_tiles_len = 1;

  while (1) {
    size_t *this_len;
    Point *this_owned_line;
    Tile player_color;

    if (active_player == 0) {
      this_len = &top_right_tiles_len;
      this_owned_line = top_right_tiles;
      player_color = RED_TOPRIGHT;
    } else {
      this_len = &bottom_left_tiles_len;
      this_owned_line = bottom_left_tiles;
      player_color = PURPLE_BOTTOMLEFT;
    }

    // pass turn if no move to play
    int pass = 1;
    for (int i = 0; i < *this_len; i++) {
      Point *neighbors = get_4points_around(this_owned_line[i]);
      for (int j = 0; j < 4; j++) {
        Point p = neighbors[j];
        if (!(p.x < 0 || p.x >= board_size || p.y < 0 || p.y >= board_size)) {
          Tile t = board[p.x][p.y];
          if (t != PURPLE_BOTTOMLEFT && t != RED_TOPRIGHT) {
            pass = 0;
            break;
          }
        }
        if (pass == 0) {
          break;
        }
      }
      free(neighbors);
    }

    if (pass == 1) {
      if (active_player == 0) {
        active_player = 1;
      } else {
        active_player = 0;
      }
      continue;
    }

    char *player_name = get_player_name(active_player);
    Point *converted = malloc(board_size * board_size * sizeof(Point));
    size_t converted_len = 0;

    if (active_player == 0) {
      printf("%s", CLEAR);

      print_board(board, board_size);

      printf("Now playing : %s\n", player_name);
      printf("1 - BLUE\n2 - CYAN\n3 - GREEN\n4 - ORANGE\n5 - WHITE\n");

      enum Tile *choice = get_tile_choice();
      switch (*choice) {
      case RED_TOPRIGHT:
      case PURPLE_BOTTOMLEFT:
        printf("ERR: chosen tile:");
        print_tile_as_board(choice);
        printf("is red or purple which is impossible");
        exit(1);

      default:
        break;
      }

      play_move(board, board_size, choice, this_owned_line, this_len, converted,
                &converted_len, &active_player, player_color, 0);
      free(choice);

    } else {
      // compute the best choice
      Tile choice = get_random_tile();

      play_move(board, board_size, &choice, this_owned_line, this_len,
                converted, &converted_len, &active_player, player_color, 1);
    }
    free(converted);
    free(player_name);

    if (*this_len >= (board_size * board_size) / 2) {
      print_board(board, board_size);
      printf("WIN CONDITION REACHED\n");
      char *other_player_name = get_player_name(!active_player);
      printf("%s\n", other_player_name);
      free(other_player_name);
      break;
    }
  }

  for (int x = 0; x < board_size; x++) {
    free(board[x]);
  }
  free(board);
  free(top_right_tiles);
  free(bottom_left_tiles);

  return 0;
}

int main() { play(10); }
