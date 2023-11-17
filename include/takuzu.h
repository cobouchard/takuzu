#ifndef TAKUZU_H
#define TAKUZU_H

#include "grid.h"
#include <stdbool.h>

#define DEFAULT_VERBOSE 0
#define DEFAULT_SOLVER_MODE true
#define DEFAULT_ALL false
#define DEFAULT_N 8
#define DEFAULT_UNIQUE false

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

void testing_grids();

void end_of_main(char *output);

#endif /* TAKUZU_H */
