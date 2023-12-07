#include <stdbool.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/takuzu.h"


bool check_char(const char c) {
    return c == '0' || c == '1' || c == '_';
}

bool check_separator(const char c) {
    return c == '\t' || c == ' ';
}

void file_parser(t_grid *grid, FILE *file, int *parameters_size) {
    char buffer[MAX_BUFFER];

    int current_line = 0;
    int temp_size = 0;
    char *first_line = (char *) calloc(MAX_BUFFER, sizeof(char));

    //let's read the first line, find the size of the grid and allocate the grid to store the data parsed

    do {
        if (fgets(buffer, MAX_BUFFER, file) != NULL) {
            readLine(buffer, MAX_GRID_SIZE, current_line, first_line, &temp_size);
        } else {
            free(first_line);
            free(grid);
            fclose(file);
            errx(EXIT_FAILURE, "File is empty !");
        }
    } while (temp_size == 0);

    temp_size = checkArgGeneratorInt(temp_size);
    if (temp_size == -1) {
        free(first_line);
        free(grid);
        fclose(file);
        errx(EXIT_FAILURE, "Incorrect number of significant characters, grid size must be 4, 8, 16, 32 or 64.\n");
    }

    *parameters_size = temp_size;
    grid_allocate(grid, *parameters_size);

    for (int i = 0; i != *parameters_size; i++) {
        grid->grid[0][i] = first_line[i];
    }
    current_line++;
    free(first_line);

    //we will now check that the next lines are the same size as the first one and finish the parsing of the file
    char *line = (char *) calloc(grid->size, sizeof(char));
    if (line == NULL) {
        free(line);
        grid_free(grid);
        free(grid);
        fclose(file);
        errx(EXIT_FAILURE, "Failed to allocated line in parser\n");
    }
    while (fgets(buffer, MAX_BUFFER, file) != NULL) {
        int current_size = 0;
        readLine(buffer, *parameters_size, current_line, line, &current_size);
        //there are no significant characters, empty line or commented line
        if (current_size == 0) {
            //we go to the next line
            continue;
        }

        if (current_size != *parameters_size) {
            free(line);
            grid_free(grid);
            free(grid);
            fclose(file);
            errx(EXIT_FAILURE,
                 "Incorrect number of significant characters '%d' in line '%d'; %d was expected as in the first uncommented line.\n",
                 current_size, current_line + 1, *parameters_size);
        }

        for (int i = 0; i != *parameters_size; i++) {
            grid->grid[current_line][i] = line[i];
        }
        current_line++;
    }

    if (current_line != *parameters_size) {
        free(line);
        grid_free(grid);
        free(grid);
        fclose(file);
        errx(EXIT_FAILURE, "Incorrect number of lines, %d lines found.", current_line);
    }

    free(line);
}


/**
 *
 * @param line
 * @param size size of the grid if known, if not known (first time going in this function, should be size 64 and the size will be determined thanks to the first line
 * @return the corresponding line of the grid (only significant characters)
 */
void readLine(char *line, int size, int current_line, char *line_to_return, int *count_char) {
    char current_char = '\0';
    int current_index = 0;
    memset(line_to_return, 0, size);

    if (line_to_return == NULL) {
        warnx("Fail in the allocation of line_of_grid in readLine function");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; current_char != '\n'; i++) {
        current_char = line[i];
        if (current_char != '\n') {
            if (current_char == '#') {
                return;
            } else if (check_separator(current_char)) {
                continue;
            } else if (check_char(current_char)) {
                if (current_index > size) {
                    errx(EXIT_FAILURE, "Exceeding 64 significant characters, grid cannot be bigger, exiting.");
                }

                line_to_return[current_index] = current_char;
                current_index++;
                *count_char += 1;
            } else {
                errx(EXIT_FAILURE,
                     "Unexpected character '%c' at line %d, column %d, a significant character was expected.\n",
                     current_char, ++current_line, ++current_index);
            }
        }
    }
}