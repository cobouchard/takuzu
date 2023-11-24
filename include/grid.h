#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define set_bit(number, place) (number = number | (INT64_C(1)<<place))
#define mask_nbits(size) (INT64_MAX >> (64-size))
#define different(a, b) ((a | b) != mask_nbits(g->size))


#define percentage 20

typedef struct {
    int size;
    char **grid;
} t_grid;

typedef struct {
    int row;
    int column;
    char choice;
} t_choice;

typedef enum{MODE_FIRST,MODE_ALL} t_mode;

void grid_allocate(t_grid *g, int size);

void grid_free(const t_grid *g);

void grid_print(const t_grid *g, FILE *fd);

void grid_copy(t_grid *grid_to_copy, t_grid *grid_result);

void set_cell(int i, int j, t_grid *g, char c);

char get_cell(int i, int j, t_grid *g);

int64_t lineToInt(t_grid *g, int line, char c);

int64_t columnToInt(t_grid *g, int column, char c);

bool isConsistent(t_grid *g);

int countBits(int64_t n);

bool hasConsecutiveCharacters(int64_t number);

bool is_valid(t_grid *g);

bool fill_column(int column, t_grid *g, char c, int count);

bool fill_line(int line, t_grid *g, char c, int count);

void generate_grid(int size, t_grid *g);

bool solve(t_grid *g);

void grid_choice_apply(t_grid *g,const t_choice choice);

void grid_choice_print(const t_choice choice, FILE *fd);

t_choice grid_choice(t_grid *g);

t_grid *grid_solver(t_grid *g, const t_mode);

#endif 
