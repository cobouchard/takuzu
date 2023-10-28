#ifndef GRID_H
#define GRID_H

typedef struct {
    int size;
    char **grid;
} t_grid;

void grid_allocate(t_grid *g, int size);

void grid_free(const t_grid *g);

#endif 
