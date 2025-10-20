#ifndef GAMELOGIC_H
# define GAMELOGIC_H

# include <stdbool.h>
# include "termdefines.h"
# include "board.h"

int **init_board(int size);
void make_optstr(int opti[5], char *optstr);
void player_turn(int **b, int size, bool p1_turn);
void	ai_turn(int **b, int size, bool p1_turn);
void play(int size);


#endif