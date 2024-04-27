#include "board_utils.h"
#include "tile_utils.h"
#include <stdio.h>
#include <stdlib.h>

void print_board(enum Tile **board, size_t board_size) {
  for (int x = 0; x < board_size; x++) {
    for (int y = 0; y < board_size; y++) {
      print_tile_as_board(&board[x][y]);
    }
    // printf("\n");
    printf("\n");
  }
}

void filter_put(Tile **board, size_t board_size, Tile chosen_tile,
                Point candidate, Point *target, size_t *target_len,
                Point *gen_pool, size_t *gen_pool_len) {

  if (candidate.x < 0 || candidate.x >= board_size || candidate.y < 0 ||
      candidate.y >= board_size) {
    printf("candidate out of bound\n");
    printf("board size : %zu\n", board_size);
    return;
  }

  if (chosen_tile != board[candidate.x][candidate.y]) {
    printf("candidate is wrong color\n");
    print_tile_as_board(&board[candidate.x][candidate.y]);
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
    printf("candidate added : %i, %i\n", candidate.x, candidate.y);

    target[*target_len] = candidate;
    (*target_len)++;

    printf("new len: %zu\n", *target_len);
  }
}

Point *get_4points_around(Point p) {
  Point *res = (Point *)malloc(4 * sizeof(Point));
  res[0] = (Point){p.x, p.y + 1};
  res[1] = (Point){p.x, p.y - 1};
  res[2] = (Point){p.x + 1, p.y};
  res[3] = (Point){p.x - 1, p.y};

  return res;
}