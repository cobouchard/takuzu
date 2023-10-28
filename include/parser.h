#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "grid.h"

bool check_char(const char c);

bool check_separator(const char c);

void file_parser(t_grid *grid, char *filename, int* parameters_size);

char *readLine(char *line, int size, int current_line);

int countCharInString(char* string);


#endif 
