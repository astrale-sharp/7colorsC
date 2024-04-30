#include "main.h"

typedef struct World {
  Tile **board;
  size_t board_size;

  // o is top right, 1 is bottom left
  int active_player;

  Point *top_right_tiles;
  size_t top_right_tiles_len;

  Point *bottom_left_tiles;
  size_t bottom_left_tiles_len;
} World;

typedef struct PlayerInfo {
  Point *owned_line;
  size_t *line_len;

  // converted this turn
  Point *converted;
  size_t converted_len;

  Tile color;
  char *name;

  int is_ai;
} PlayerInfo;

World world__new(size_t board_size, int active_player) {
  srand(time(NULL));

  World w = {
      .active_player = active_player,

      .board = malloc(board_size * sizeof(enum Tile *)),
      .board_size = board_size,

      .top_right_tiles = malloc(board_size * board_size * sizeof(Point)),
      .bottom_left_tiles = malloc(board_size * board_size * sizeof(Point)),

      .top_right_tiles_len = 1,
      .bottom_left_tiles_len = 1,

  };

  for (int x = 0; x < board_size; x++) {
    enum Tile *line = malloc(board_size * sizeof(enum Tile));
    for (int y = 0; y < board_size; y++) {
      line[y] = get_random_tile();
    }
    w.board[x] = line;
  }

  w.board[board_size - 1][0] = PURPLE_BOTTOM_LEFT;
  w.board[0][board_size - 1] = RED_TOP_RIGHT;

  w.top_right_tiles[0] = (Point){.x = 0, .y = board_size - 1};
  w.bottom_left_tiles[0] = (Point){.x = board_size - 1, .y = 0};

  return w;
}

PlayerInfo player_info(World *w) {

  PlayerInfo pi = {
      .name = get_player_name(w->active_player),
      .converted = malloc(w->board_size * w->board_size * sizeof(Point)),
      .converted_len = 0,
      .is_ai = w->active_player == 1,
  };

  if (w->active_player == 0) {
    pi.line_len = &w->top_right_tiles_len;
    pi.owned_line = w->top_right_tiles;
    pi.color = RED_TOP_RIGHT;
  } else {
    pi.line_len = &w->bottom_left_tiles_len;
    pi.owned_line = w->bottom_left_tiles;
    pi.color = PURPLE_BOTTOM_LEFT;
  }
  return pi;
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

int can_play(World *w, PlayerInfo *info) {
  int pass = 0;
  for (int i = 0; i < *info->line_len; i++) {
    Point *neighbors = get_4points_around(info->owned_line[i]);
    for (int j = 0; j < 4; j++) {
      Point p = neighbors[j];
      if (!(p.x < 0 || p.x >= w->board_size || p.y < 0 ||
            p.y >= w->board_size)) {
        Tile t = w->board[p.x][p.y];
        if (t != PURPLE_BOTTOM_LEFT && t != RED_TOP_RIGHT) {
          pass = 1;
          break;
        }
      }
      if (pass) {
        break;
      }
    }
    free(neighbors);
  }
  return pass;
}

void play_move(World *w, PlayerInfo *info, Tile choice) {
  size_t save_len = *info->line_len;
  printf("save len 0: %zu\n", save_len);
  printf("board size : %zu\n", w->board_size);

  for (int i = 0; i < *info->line_len; i++) {
    Point p = info->owned_line[i];
    printf("i : %i\n", i);
    printf("p : %i, %i\n", p.x, p.y);

    // 4 elements
    Point *neighbors = get_4points_around(p);

    for (int j = 0; j < 4; j++) {
      Point this_neighbor = neighbors[j];
      printf("\tj : %i\n", j);
      printf("\tneigh : %i,%i\n", this_neighbor.x, this_neighbor.y);

      filter_put(w->board, w->board_size, choice, this_neighbor,
                 info->converted, &info->converted_len, info->owned_line,
                 info->line_len);
    }

    free(neighbors);

    for (int j = 0; j < info->converted_len; j++) {
      Point c = info->converted[j];
      int filter = 0;

      for (int k = 0; k < *info->line_len; k++) {
        Point p = info->owned_line[k];
        if (p.x == c.x && p.y == c.y) {
          filter = 1;
          break;
        }
      }
      printf("filter: %i\n", filter);

      if (filter == 0) {
        w->board[c.x][c.y] = info->color;
        info->owned_line[*info->line_len] = c;
        printf("line len pre ++: %zu\n", *info->line_len);
        (*info->line_len)++;
        printf("then: %zu\n", *info->line_len);

      } else {
        printf("unlicky?");
      }
    }
  }

  if (save_len != *info->line_len) {
    if (w->active_player == 0) {
      w->active_player = 1;
    } else {
      w->active_player = 0;
    }
  } else {
    printf("save len: %zu", save_len);
    printf("\n");
    printf("line len: %zu", *info->line_len);
    printf("\n");

    if (!info->is_ai) {
      printf("EMPTY MOVE ARE INVALID, PRESS ENTER TO SELECT AGAIN\n");
      get_input();
    }
  }
}

int play_against_ia(size_t board_size) {

  World w = world__new(board_size, 0);

  while (1) {

    PlayerInfo info = player_info(&w);

    // pass turn if no move to play
    if (!can_play(&w, &info)) {
      if (w.active_player == 0) {
        w.active_player = 1;
      } else {
        w.active_player = 0;
      }

      printf("skipping turn %s\n", info.name);
      free(info.converted);
      free(info.name);
      continue;
    }

    if (!info.is_ai) {
      printf("%s", CLEAR);

      print_board(w.board, board_size);

      printf("Now playing : %s\n", info.name);
      printf("1 - BLUE\n2 - CYAN\n3 - GREEN\n4 - ORANGE\n5 - WHITE\n");

      enum Tile *choice = get_tile_choice();
      switch (*choice) {
      case RED_TOP_RIGHT:
      case PURPLE_BOTTOM_LEFT:
        printf("ERR: chosen tile:");
        print_tile_as_board(choice);
        printf("is red or purple which is impossible");
        exit(1);

      default:
        break;
      }

      play_move(&w, &info, *choice);
      free(choice);
    } else {
      // compute the best choice
      play_move(&w, &info, get_random_tile());
    }

    free(info.converted);
    free(info.name);

    if (*info.line_len >= (board_size * board_size) / 2) {
      print_board(w.board, board_size);
      printf("WIN CONDITION REACHED\n");
      char *other_player_name = get_player_name(!w.active_player);
      printf("%s\n", other_player_name);
      free(other_player_name);
      break;
    }
  }

  for (int x = 0; x < board_size; x++) {
    free(w.board[x]);
  }
  free(w.board);
  free(w.top_right_tiles);
  free(w.bottom_left_tiles);

  return 0;
}

int main() {
  World w = world__new(10, 0);
  play_against_ia(10);
}
