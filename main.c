#include "main.h"
#include "array/array.c"
#include "colors/colors.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Point {
  int x;
  int y;
} Point;

typedef enum Tile {
  BLUE,
  CYAN,
  GREEN,
  ORANGE,
  WHITE,
  RED_TOPRIGHT,
  PURPLE_BOTTOMLEFT,
} Tile;

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

enum Tile get_random_tile() { return random() % 5; }

void print_board(enum Tile **board) {}

char *get_player_name(int active_player) {

  char *color;
  char *name;
  if (active_player == 0) {
    color = BgRed;
    name = "top right";
  } else {
    color = BgPurple;
    name = "bottom left";
  }
  char *dest = malloc(sizeof(char) * 30);
  strcat(dest, color);
  strcat(dest, name);
  strcat(dest, BACK);
  return dest;
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

// may return NULL
enum Tile *char_to_tile(char c) {
  enum Tile *t = malloc(sizeof(enum Tile));
  switch (c) {
  // python macro
  // l = [["BLUE", "b", "B", "1"],["CYAN", "c", "C", "2"],["GREEN", "g", "G",
  // "3"],["ORANGE", "o", "O", "4"],["WHITE", "w", "W", "5"],]
  //
  // for x in l:
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

  ////////////////////
  default: {
    return NULL;
  }
  }
  return t;
}

int main() {
  // use the time to set the seed
  srand(time(NULL));

  // o is top right, 1 is bottomleft
  int active_player = 0;

  size_t BOARD_SIZE = 25;

  enum Tile **board = malloc(BOARD_SIZE * sizeof(enum Tile *));
  for (int x = 0; x < BOARD_SIZE; x++) {
    enum Tile *line = malloc(BOARD_SIZE * sizeof(enum Tile));
    for (int y = 0; y < BOARD_SIZE; y++) {
      line[y] = get_random_tile();
    }
    board[x] = line;
  }

  board[BOARD_SIZE - 1][0] = PURPLE_BOTTOMLEFT;
  board[0][BOARD_SIZE - 1] = RED_TOPRIGHT;

  Point *top_right_tiles = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(Point));
  Point *bottom_left_tiles = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(Point));

  top_right_tiles[0] = (Point){.x = BOARD_SIZE - 1, .y = 0};
  size_t top_right_tiles_len = 1;
  //
  bottom_left_tiles[0] = (Point){.x = 0, .y = BOARD_SIZE - 1};
  size_t bottom_left_tiles_len = 1;

  while (1) {
    printf("%s", CLEAR);

    // print_board
    for (int x = 0; x < BOARD_SIZE; x++) {
      for (int y = 0; y < BOARD_SIZE; y++) {
        // printf(" ");
        print_tile_as_board(&board[x][y]);
        // printf(" ");
      }
      // printf("\n");
      printf("\n");
    }

    printf("Now playing : %s\n", get_player_name(active_player));
    printf("1 - BLUE\n2 - CYAN\n3 - GREEN\n4 - ORANGE\n5 - WHITE\n");
    enum Tile *choice = NULL;

    while (choice == NULL) {
      printf("please input a character.\n");
      printf("%s%s", BLINK, BACK);
      char c = get_input();
      choice = char_to_tile(c);
      if (choice == NULL) {
        printf("incorrect char %c\n", c);
        char _ = get_input();
        continue;
      } else {
        printf("got tile: %c", tile_to_char(choice));
      }

      // get current line of player
      Point *neigbhors = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(Point));
      size_t neighbors_len = 0;
      size_t *this_len;
      Point *this_owned_line;

      if (active_player == 0) {
        this_len = &top_right_tiles_len;
        this_owned_line = top_right_tiles;
      } else {
        this_len = &bottom_left_tiles_len;
        this_owned_line = bottom_left_tiles;
      }

      for (int i = 0; i < *this_len; i++) {
        Point p = this_owned_line[i];
        Point n1 = (Point){p.x, p.y + 1};
        Point n2 = (Point){p.x, p.y - 1};
        Point n3 = (Point){p.x + 1, p.y};
        Point n4 = (Point){p.x - 1, p.y};
        Point X;
        /// if not out of bounds and not owned, add to owned.
        /// python generate
// template = """{
//           int filter = 0;
//           for (int i = 0; i < *this_len; i++) {
//             Point p = this_owned_line[i];
//             if (p.x == $0$.x && p.y == $0$.y) {
//               filter = 1;
//               break;
//             }
//           }
//           if (filter == 0) {
//             for (int i = 0; i < neighbors_len; i++) {
//               Point p = neigbhors[i];
//               if (p.x == $0$.x && p.y == $0$.y) {
//                 filter = 1;
//                 break;
//               }
//             }
//           }
// 
//           if (filter == 0) {
//             neigbhors[neighbors_len] = $0$;
//             neighbors_len++;
//           }
//         }"""
// 
// for k in ["n1","n2","n3", "n4"]:
//         print(template.replace("$0$", k))
        {
          int filter = 0;
          for (int i = 0; i < *this_len; i++) {
            Point p = this_owned_line[i];
            if (p.x == n1.x && p.y == n1.y) {
              filter = 1;
              break;
            }
          }
          if (filter == 0) {
            for (int i = 0; i < neighbors_len; i++) {
              Point p = neigbhors[i];
              if (p.x == n1.x && p.y == n1.y) {
                filter = 1;
                break;
              }
            }
          }

          if (filter == 0) {
            neigbhors[neighbors_len] = n1;
            neighbors_len++;
          }
        }
        {
          int filter = 0;
          for (int i = 0; i < *this_len; i++) {
            Point p = this_owned_line[i];
            if (p.x == n2.x && p.y == n2.y) {
              filter = 1;
              break;
            }
          }
          if (filter == 0) {
            for (int i = 0; i < neighbors_len; i++) {
              Point p = neigbhors[i];
              if (p.x == n2.x && p.y == n2.y) {
                filter = 1;
                break;
              }
            }
          }

          if (filter == 0) {
            neigbhors[neighbors_len] = n2;
            neighbors_len++;
          }
        }
        {
          int filter = 0;
          for (int i = 0; i < *this_len; i++) {
            Point p = this_owned_line[i];
            if (p.x == n3.x && p.y == n3.y) {
              filter = 1;
              break;
            }
          }
          if (filter == 0) {
            for (int i = 0; i < neighbors_len; i++) {
              Point p = neigbhors[i];
              if (p.x == n3.x && p.y == n3.y) {
                filter = 1;
                break;
              }
            }
          }

          if (filter == 0) {
            neigbhors[neighbors_len] = n3;
            neighbors_len++;
          }
        }
        {
          int filter = 0;
          for (int i = 0; i < *this_len; i++) {
            Point p = this_owned_line[i];
            if (p.x == n4.x && p.y == n4.y) {
              filter = 1;
              break;
            }
          }
          if (filter == 0) {
            for (int i = 0; i < neighbors_len; i++) {
              Point p = neigbhors[i];
              if (p.x == n4.x && p.y == n4.y) {
                filter = 1;
                break;
              }
            }
          }

          if (filter == 0) {
            neigbhors[neighbors_len] = n4;
            neighbors_len++;
          }
        }

        // try convert neighbors in convert_neigbhors
        // no convert? -> break
        // convert? mut board && mut this owned_line && neighbors -> filter && restart this process with convert neighbors as neighbors
      }

    }
  }
}
