#include "../include/grid.h"
#include <stdlib.h>
#include <err.h>

void grid_allocate(t_grid *g, int size) {
    g->grid = (char **) malloc(size * sizeof(char *));

    g->size = size;
    if (g->grid == NULL) {
        warnx("Fail in the allocation of grid in t_grid");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; ++i) {
        g->grid[i] = (char *) malloc(size * sizeof(char));
        if (g->grid[i] == NULL) {
            grid_free(g);
            warnx("Fail in the allocation of each row of grid in t_grid");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < size; ++j) {
            g->grid[i][j] = '_';
        }
    }
}


void grid_free(const t_grid *g) {
    for (int i = 0; i < g->size; ++i) {
        if(g->grid[i]!=NULL)
            free(g->grid[i]);
    }

    free(g->grid);
}