#ifndef BOARD_H
# define BOARD_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include "termdefines.h"

char	*get_color_pstr(int c);
int		**board_new(int size);
int		**board_cpy(int **src, int size);
void 	board_free(int **board, int size);
int		get_rand_tile();
void	board_random_populate(int **board, int size);
void	board_print_tile(int color);
void	board_print(int **board, int size);
void	board_double_print(int **b1, int **b2, int size);
void	board_get_opt(int **b, int size, int opti[5], bool p1_turn);
void	board_apply(int **b, int size, int color, bool p1_turn);
void	board_count_point(int **b, int size, int *p1, int *p2);

#endif
