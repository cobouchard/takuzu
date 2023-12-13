#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define set_bit(number, place) (number = number | (UINT64_C(1)<<place))
#define mask_nbits(size) ( (size==64) ? UINT64_MAX : UINT64_MAX >> (64-size))
#define different(a, b) ( (uint64_t )(a | b) != (uint64_t )mask_nbits(g->size))


#define percentage 20
#define MAX_SOLVABLE_SIZE 16

typedef struct {
    int size;
    char **grid;
} t_grid;

typedef struct {
    int row;
    int column;
    char choice;
} t_choice;

typedef enum {
    MODE_FIRST, MODE_ALL
} t_mode;

void grid_allocate(t_grid *g, int size);

void grid_free(t_grid *g);

/**
 * print the current grid in an output file, because this function is given a FILE*, it's not its role to close and free it
 * @param g grid to be printed
 * @param fd file descriptor
 */
void grid_print(const t_grid *g, FILE *fd);

void grid_copy(t_grid *grid_to_copy, t_grid *grid_result);

void set_cell(int i, int j, t_grid *g, char c);

char get_cell(int i, int j, t_grid *g);

/**
 * @param g
 * @param line
 * @param c
 * @return generate a 64-bits long integer, two identical lines generate the same number, return 0 if there is an '_' meaning that this line will be different than any other line
 */
int64_t lineToInt(t_grid *g, int line, char c);

/**
 * same function as lineToInt but go through a column
 * @param g
 * @param column
 * @param c
 * @return
 */
int64_t columnToInt(t_grid *g, int column, char c);


/**
 * @param g
 * @return true if there are no consecutive 0 or 1 AND if no two lines or two columns are equals
 */
bool is_consistent(t_grid *g);

int countBits(int64_t n);

bool hasConsecutiveCharacters(int64_t number);

bool is_valid(t_grid *g);

bool is_full(t_grid *g);

/**
 * fill every empty cell in given column with given char
 * @param column
 * @param g
 * @param c
 * @param count
 * @return true if a character have been changed
 */
bool fill_column(int column, t_grid *g, char c, int count);

bool fill_line(int line, t_grid *g, char c, int count);

void generate_solvable_grid(int size, t_grid *g);

void generate_rand_grid(int size, t_grid *g);

bool apply_heuristics(t_grid *g);

void grid_choice_apply(t_grid *g, const t_choice choice);

void grid_choice_print(const t_choice choice, FILE *fd);

/**
 * finds the first empty cell and update i and j to its coordinates
 * @param i
 * @param j
 * @param g
 */
void find_first_empty(int *i, int *j, t_grid *g);

t_choice grid_choice(t_grid *g);

t_grid *grid_solver(t_grid *g, const t_mode mode);

void *grid_solver_all(t_grid *g);

t_grid *grid_solver_first(t_grid *g);

/**
 * this should take a valid grid, it prints the grid and a text with the number of solutions
 * @param g
 * @param final is this is true, doesn't print any grid but print the number of solutions found instead
 */
void print_solution(t_grid *g, bool final);

#endif 
