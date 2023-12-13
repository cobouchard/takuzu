#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "grid.h"

#define MAX_BUFFER 256
#define MAX_GRID_SIZE 64

bool check_char(const char c);

bool check_separator(const char c);

void file_parser(t_grid *grid, FILE *filename, int *parameters_size);


/**
 * @param line
 * @param size size of the grid if known, if not known (first time going in this function, should be size 64 and the size will be determined thanks to the first line
 * @return the corresponding line of the grid (only significant characters)
 */
void readLine(char *line, int size, int current_line, char *line_to_return, int *count_char);


#endif
