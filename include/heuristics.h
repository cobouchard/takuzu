#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <stdbool.h>
#include "grid.h"

#define check_coord(i, j, size) (!((i)<0 || (j)<0 || (i)>=size || (j)>=size))
#define check_empty(i, j, g) (g->grid[i][j]=='_')
#define other_char(c) (c=='0' ? '1' : '0')

#define heuristics(grid) (heur_consecutive(grid)||heur_fill(grid)||heur_between(grid))

/**
 * when 2 consecutive characters are found (1 or 0) the next character is set to the opposite (if it was not filled)
 * @param g
 * @return true if the grid has been changed, false otherwise
 */
bool heur_consecutive(t_grid *g);


/**
 * whenever a line/column already has half 1 or 0, fill the rest to the other char
 * @param g
 * @return true if the grid has been changed
 */
bool heur_fill(t_grid *g);

/**
 * fill spaces between two of the same char to the other char
 * @param g
 * @return true if the grid has been changed
 */
bool heur_between(t_grid *g);


#endif
