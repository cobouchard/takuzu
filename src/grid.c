#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/grid.h"


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
    if (g == NULL) {
        return;
    }
    for (int i = 0; i < g->size; ++i) {
        if (g->grid[i] != NULL) {
            free(g->grid[i]);
        }
    }

    free(g->grid);
}


/**
 * print the current grid in an output file, because this function is given a FILE*, it's not its role to close and free it
 * @param g
 * @param fd
 */
void grid_print(const t_grid *g, FILE *fd) {
    if (g == NULL) {
        warnx("grid given to print in file is NULL");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i != g->size; i++) {
        for (int j = 0; j != g->size; j++) {
            fprintf(fd, "%c ", g->grid[i][j]);
        }
        fprintf(fd, "%c", '\n');
    }
}

void grid_copy(t_grid *grid_to_copy, t_grid *grid_result) {
    if (grid_to_copy == NULL) {
        errx(EXIT_FAILURE, "trying to copy an unallocated grid");
    }
    grid_allocate(grid_result, grid_to_copy->size);

    for (int i = 0; i != grid_to_copy->size; i++) {
        for (int j = 0; j != grid_to_copy->size; j++) {
            grid_result->grid[i][j] = grid_to_copy->grid[i][j];
        }
    }
}

void set_cell(int i, int j, t_grid *g, char c) {
    if (g == NULL) {
        errx(EXIT_FAILURE, "trying to setting a cell on an unallocated grid");
    }

    g->grid[i][j] = c;
}

char get_cell(int i, int j, t_grid *g) {
    if (g == NULL) {
        errx(EXIT_FAILURE, "trying to get a cell from an unallocated grid");
    }

    return g->grid[i][j];
}

/**
 *
 * @param g
 * @param line
 * @return generate a 64-bits long integer, two identical lines generate the same number, return 0 if there is an '_' meaning that this line will be different than any other line
 */
int64_t lineToInt(t_grid *g, int line, char c) {
    int64_t code = INT64_C(0);

    for (int column = 0; column != g->size; column++) {
        char temp = g->grid[line][column];

        if (temp == c) {
            code = code | (1 << column);
        }
    }

    return code;
}

//same function but for columns
int64_t columnToInt(t_grid *g, int column, char c) {
    int64_t code = INT64_C(0);

    for (int line = 0; line != g->size; line++) {
        char temp = g->grid[line][column];
        if (temp == c) {
            code = code | (1 << line);
        }

    }

    return code;
}

/**
 *
 * @param g
 * @return return true if no lines nor columns are identical
 */
bool checkLinesColumnsDifferent(t_grid *g) {
    int64_t linesCodeOnes[g->size];
    int64_t columnsCodeOnes[g->size];

    int64_t linesCodeZeroes[g->size];
    int64_t columnsCodeZeroes[g->size];

    for (int i = 0; i != g->size; i++) {
        linesCodeOnes[i] = lineToInt(g, i, '1');
        columnsCodeOnes[i] = columnToInt(g, i, '1');
        linesCodeZeroes[i] = lineToInt(g, i, '0');
        columnsCodeZeroes[i] = columnToInt(g, i, '0');
    }

    for (int i = 0; i != g->size; i++) {
        if ((linesCodeOnes[i] | linesCodeZeroes[i]) != (1 << g->size) - 1) {
            //there is an underscore, lines are necessarily different
            continue;
        }
        for (int j = i + 1; j != g->size; j++) {
            if (i != j) {
                //if the position of zeroes and one is the same in both lines resp columns, the two are equals
                if (linesCodeOnes[i] == linesCodeOnes[j] && linesCodeZeroes[i] == linesCodeZeroes[j]) {
                    return false;
                }
            }
        }
    }

    return true;
}

/**
 *
 * @param g
 * @return return true is there are no consecutive 0 or consecutive 1
 */
bool checkConsecutiveCharacters(t_grid *g) {
    int64_t linesCodeOnes[g->size];
    int64_t columnsCodeOnes[g->size];

    int64_t linesCodeZeroes[g->size];
    int64_t columnsCodeZeroes[g->size];

    for (int i = 0; i != g->size; i++) {
        linesCodeOnes[i] = lineToInt(g, i, '1');
        columnsCodeOnes[i] = columnToInt(g, i, '1');
        linesCodeZeroes[i] = lineToInt(g, i, '0');
        columnsCodeZeroes[i] = columnToInt(g, i, '0');
    }


    for (int i = 0; i != g->size; i++) {
        //check that there are no 3 consecutive 0 or 1 in lines and columns
        if (hasConsecutiveCharacters(linesCodeZeroes[i]) || hasConsecutiveCharacters(linesCodeOnes[i]) ||
            hasConsecutiveCharacters(columnsCodeZeroes[i]) || hasConsecutiveCharacters(columnsCodeOnes[i])) {
            return false;
        }
    }

    return true;
}


bool hasConsecutiveCharacters(int64_t number) {
    return number & (number >> 1) & (number >> 2);
}
