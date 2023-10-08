#include "main.h"
#include "colors/colors.h"
#include "array/array.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// returns an int from 0 to 6 which is a `color`.
int populate_random() { return rand() % 7; }

Board board_new(size_t size) {
  int **data = malloc(size * sizeof(int *));
  for (size_t i = 0; i < size; i++) {
    data[i] = malloc(size * sizeof(int));
    for (size_t j = 0; j < size; j++) {
      data[i][j] = populate_random();
    }
  }
  data[size - 1][0] = 7;
  data[0][size - 1] = 8;
  return (Board){
      .size = size,
      .data = data,
  };
}

void board_print(Board *board) {
  for (size_t x = 0; x < board->size; x++) {
    for (size_t y = 0; y < board->size; y++) {
      int trailing_space = y != board->size - 1;
      print_color(board->data[x][y], trailing_space);
    }
    printf("\n");
  }
}

int *get_valid_moves(Board *board, int player_code /*7 or 8: see main.h*/) {
  int *moves;
  if (player_code != 7 && player_code != 8) {
    printf("invalid player code in get_valid_moves");
    exit(2);
  }

  return moves;
}

// given a board size and x,y position, return true if the position is illegal.
int is_out_of_bound(int x, int y, size_t size) {
  return x < 0 || y < 0 || x >= (int)size || y >= (int)size;
}

// gives the four neighbors of a position
int **get_neighbors(size_t x, size_t y) {
  int **moves = malloc(4 * sizeof(int *));
  moves[0] = malloc(2 * sizeof(int));
  moves[0][0] = x - 1;
  moves[0][1] = y;
  moves[1] = malloc(2 * sizeof(int));
  moves[1][0] = x + 1;
  moves[1][1] = y;
  moves[2] = malloc(2 * sizeof(int));
  moves[2][0] = x;
  moves[2][1] = y - 1;
  moves[3] = malloc(2 * sizeof(int));
  moves[3][0] = x;
  moves[3][1] = y + 1;
  return moves;
}

void free_neighbors(int **n) {
  for (int k = 0; k < 4; k++) {
    free(n[k]);
  }
  free(n);
}

void print_neighbors(int **n) {
  for (int k = 0; k < 4; k++) {
    printf("v: %i,%i\n", n[k][0], n[k][1]);
  }
}

_2DArray get_case_with_idx(Board *board, int idx) {
  int **data = malloc(board->size * board->size * sizeof(int *));
  int k = 0;
  for (size_t x; x < board->size; x++) {
    for (size_t y; y < board->size; y++) {
      if (board->data[x][y] == idx) {
        data[k] = malloc(2 * sizeof(int));
        data[k][0] = x;
        data[k][1] = y;
        k++;
      }
    }
  }
  return (_2DArray){.length = k, .data = data};
}

void free_2d_array(_2DArray a) {
  for (size_t k = 0; k < a.length; k++) {
    free(a.data[k]);
  }
  free(a.data);
}

int main() {
  // use the time to set the seed
  srand(time(NULL));
  Board board = board_new(20);
  int top_right_playing = 1;
  // see Board documentation
  int special_index = top_right_playing ? 7 : 8;

  while (1) {
    printf("%s", CLEAR);
    board_print(&board);
    // todo add a color here to show who is playing
    printf("Player turn, enter a letter from A to G or type H for "
           "help:%s%s",
           BLINK, BACK);


    // get valid moves
    _2DArray case_of_player = get_case_with_idx(&board, special_index);
    // for each case, add case to handled, neighbours to to_handle

    // until to_handle is empty, pop last elem
    // Special? neighbors to to_handle : add to valid moves

    // forEach owned case at the border, add neighbour to list if not present

    // which
    while (1) {
      char *line = NULL;
      size_t len = 0;
      ssize_t lineSize = 0;
      lineSize = getline(&line, &len, stdin);
      char c = line[0];
      printf("You selected %c\n", c);
      printf("Select again:%s%s", BLINK, BACK);
      // break;
      // recursively change the board
      free(line);
    }
    // break;
  }
  return 0;
}