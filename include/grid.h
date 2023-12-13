#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define set_bit(number, place) (number = number | (UINT64_C(1)<<place))
#define mask_nbits(size) ( (size==64) ? UINT64_MAX : UINT64_MAX >> (64-size))
#define different(a, b) ( (uint64_t )(a | b) != (uint64_t )mask_nbits(g->size))


#define percentage 20
#define MAX_SOLVABLE_SIZE 16

typedef struct {
    int size;
    char **grid;
} t_grid;

typedef struct {
    int row;
    int column;
    char choice;
} t_choice;

typedef enum {
    MODE_FIRST, MODE_ALL
} t_mode;

void grid_allocate(t_grid *g, int size);

void grid_free(t_grid *g);

void grid_print(const t_grid *g, FILE *fd);

void grid_copy(t_grid *grid_to_copy, t_grid *grid_result);

void set_cell(int i, int j, t_grid *g, char c);

char get_cell(int i, int j, t_grid *g);

int64_t lineToInt(t_grid *g, int line, char c);

int64_t columnToInt(t_grid *g, int column, char c);

bool is_consistent(t_grid *g);

int countBits(int64_t n);

bool hasConsecutiveCharacters(int64_t number);

bool is_valid(t_grid *g);

bool is_full(t_grid *g);

bool fill_column(int column, t_grid *g, char c, int count);

bool fill_line(int line, t_grid *g, char c, int count);

void generate_solvable_grid(int size, t_grid *g);

void generate_rand_grid(int size, t_grid *g);

bool apply_heuristics(t_grid *g);

void grid_choice_apply(t_grid *g, const t_choice choice);

void grid_choice_print(const t_choice choice, FILE *fd);

void find_first_empty(int *i, int *j, t_grid *g);

t_choice grid_choice(t_grid *g);

t_grid *grid_solver(t_grid *g, const t_mode mode);

void *grid_solver_all(t_grid *g);

t_grid *grid_solver_first(t_grid *g);

void print_solution(t_grid *g, bool final);

#endif 
