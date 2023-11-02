#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "grid.h"

bool check_char(const char c);

bool check_separator(const char c);

void file_parser(t_grid *grid, char *filename, int* parameters_size);

void readLine(char *line, int size, int current_line, char *line_to_return);

int countCharInString(char* string);


#endif 
