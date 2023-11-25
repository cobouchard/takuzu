#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <stdbool.h>
#include "grid.h"

#define check_coord(i,j,size) (!((i)<0 || (j)<0 || (i)>=size || (j)>=size))
#define check_empty(i,j,g) (g->grid[i][j]=='_')
#define other_char(c) (c=='0' ? '1' : '0')

#define heuristics(grid) (heur_consecutive(grid)||heur_fill(grid)||heur_between(grid))


bool heur_consecutive(t_grid *g);

bool heur_fill(t_grid *g);

bool heur_between(t_grid *g);



#endif
