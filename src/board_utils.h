#include "tile_utils.h"
#include "types.h"

void print_board(enum Tile **board, size_t board_size);

// put candidate in target if
// - is of right tile color and
// - is in the board and
// - a clone isn't already in the target
void filter_put(Tile **board, size_t board_size, Tile chosen_tile,
                Point candidate, Point *target, size_t *target_len,
                Point *gen_pool, size_t *gen_pool_len);

Point *get_4points_around(Point p);