#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define set_bit(number, place) number = number | (1<<place)
#define mask_nbits(size) (1 << size) - 1
#define different(a, b) (a | b) != mask_nbits(g->size)

typedef struct {
    int size;
    char **grid;
} t_grid;

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

#endif 
