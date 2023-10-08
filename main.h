#include <stddef.h>
#include "array/array.h"
typedef struct Board {
  // contains size squared element.
  size_t size;
  // 0 to 6 means the color
  // 7 mean Team top right conquered this position
  // 8 mean Team bottom left conquered this position
  int **data;
} Board;

// contains length element
// each element contains at 0: x, at 1: y

int populate_random();

Board board_new(size_t size);
void board_print(Board *board);

int *get_valid_moves(Board *board, int player_code /*7 or 8: see main.h*/);
int is_out_of_bound(int x, int y, size_t size);

int **get_neighbors(size_t x, size_t y);
void free_neighbors(int **n);
void print_neighbors(int **n);

Vec get_case_with_idx(Board *board, int idx);
