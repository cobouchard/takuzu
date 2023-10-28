#ifndef GRID_H
#define GRID_H

#include <stdio.h>

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

#endif 
