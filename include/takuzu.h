#ifndef TAKUZU_H
#define TAKUZU_H

#include "grid.h"
#include <stdbool.h>

struct Params {
    int verbose_flag;
    bool solver_mode; //The program is on solver_mode by default, set to false if the program is running on generation mode
    bool unique;
    bool all;
    int N;
    char *output;
    char *input;
};

void print_help();

int checkArgGenerator(char *arg);

int checkArgGeneratorInt(int N);

bool check_char(const char c);

bool check_separator(const char c);

void file_parser(t_grid *grid, char *filename);

char *readLine(char *line, int size, int current_line);

int countCharInString(char* string);

void end_of_main(char *output);

#endif /* TAKUZU_H */
