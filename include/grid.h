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

#endif 
