#include "colors.h"

// given the color index, will print the color beautifully
void print_color(int idx, int trailing_space) {
  const char *color;

  switch (idx) {
  case 0:
    color = FgRed;
    break;
  case 1:
    color = FgGreen;
    break;
  case 2:
    color = FgOrange;
    break;
  case 3:
    color = FgBlue;
    break;
  case 4:
    color = FgPurple;
    break;
  case 5:
    color = FgCyan;
    break;
  case 6:
    color = FgWhite;
    break;
  case 7:
    color = BgRed;
    break;
  case 8:
    color = BgBlue;
    break;
  default: {
    printf("impossible idx in print_color");
    exit(2);
  }
  };
  char text;
  if (idx <= 6) {
    text = COLORS[idx];
  } else {
    text = ' ';
  }

  printf("%s%c", color, text);
  if (trailing_space) {
    printf(" ");
  }
  printf("%s", BACK);
}


// const char * ansi_color_from_code(const int code) {
//     char *res = malloc(15);
//     sprintf(res, "\033[%im", code);
//     return res;
// }