#pragma once

__attribute__((unused)) static const char *COLORS = "ABCDEFG";
__attribute__((unused)) static const char *FgRed = "\033[31m";
__attribute__((unused)) static const char *BgRed = "\033[41m";
__attribute__((unused)) static const char *FgGreen = "\033[92m";
__attribute__((unused)) static const char *BgGreen = "\033[102m";
__attribute__((unused)) static const char *FgOrange = "\033[93m";
__attribute__((unused)) static const char *BgOrange = "\033[103m";
__attribute__((unused)) static const char *FgBlue = "\033[94m";
__attribute__((unused)) static const char *BgBlue = "\033[104m";
__attribute__((unused)) static const char *FgPurple = "\033[95m";
__attribute__((unused)) static const char *BgPurple = "\033[105m";
__attribute__((unused)) static const char *FgCyan = "\033[96m";
__attribute__((unused)) static const char *BgCyan = "\033[106m";
__attribute__((unused)) static const char *FgWhite = "\033[97m";
__attribute__((unused)) static const char *BgWhite = "\033[107m";
__attribute__((unused)) static const char *BACK = "\033[0m";
__attribute__((unused)) static const char *CLEAR = "\e[1;1H\e[2J";
void print_color(int idx, int trailing_space);