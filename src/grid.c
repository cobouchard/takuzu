#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/grid.h"
#include "../include/heuristics.h"


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


void grid_free(t_grid *g) {
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
    if(i>=g->size || j>=g->size || j<0 || i<0){
        return;
    }

    g->grid[i][j] = c;
}

bool check_coords(int i, int j, int size){
    return i<0 || j<0 || i>=size || j>=size;
}

char get_cell(int i, int j, t_grid *g) {
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
            set_bit(code, column);
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
            set_bit(code, line);
        }

    }

    return code;
}



/**
 *
 * @param g
 * @return true if there are no consecutive 0 or 1 AND if no two lines or two columns are equals
 */
bool is_consistent(t_grid *g) {
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

    bool check_column = true;
    bool check_line = true;

    for (int i = 0; i != g->size; i++) {
        //checking that there are no consecutive characters
        if (hasConsecutiveCharacters(linesCodeZeroes[i]) || hasConsecutiveCharacters(linesCodeOnes[i]) ||
            hasConsecutiveCharacters(columnsCodeZeroes[i]) || hasConsecutiveCharacters(columnsCodeOnes[i])) {
            return false;
        }

        //checking that lines and columns are different
        if (different(linesCodeZeroes[i], linesCodeOnes[i])) {
            //there is an underscore, lines are necessarily different
            check_line = false;
        }
        if (different(columnsCodeZeroes[i], columnsCodeOnes[i])) {
            //there is an underscore, columns are necessarily different
            check_column = false;
        }


        for (int j = i + 1; j != g->size; j++) {
            //if the position of zeroes and one is the same in both lines resp columns, the two are equals
            if (check_line) {
                if (linesCodeOnes[i] == linesCodeOnes[j] && linesCodeZeroes[i] == linesCodeZeroes[j]) {
                    return false;
                }
            }
            if (check_column) {
                if (columnsCodeOnes[i] == columnsCodeOnes[j] && columnsCodeZeroes[i] == columnsCodeZeroes[j]) {
                    return false;
                }
            }

        }
        //we check that there are not more than grid_size/2 of the same character in a line or column
        if (countBits(columnsCodeOnes[i]) > g->size / 2 ||
            countBits(columnsCodeOnes[i]) > g->size / 2 ||
            countBits(linesCodeZeroes[i]) > g->size / 2 ||
            countBits(linesCodeZeroes[i]) > g->size / 2) {
            return false;
        }
        check_column = true;
        check_line = true;
    }

    return true;
}


bool hasConsecutiveCharacters(int64_t number) {
    //here we cast the number to an unsigned int, because for size 64, problems occured because of the bit sign
    uint64_t u_number = (uint64_t)number;
    return u_number & (u_number >> 1) & (u_number >> 2);
}

int countBits(int64_t n) {
    int count = 0;
    while (n) {
        count++;
        n &= (n - 1);
    }
    return count;
}

bool is_full(t_grid *g){
    for (int i = 0; i != g->size; i++) {
        for (int j = 0; j != g->size; j++) {
            if (g->grid[i][j] == '_') {
                return false;
            }
        }
    }
    return true;
}

bool is_valid(t_grid *g) {
    return is_consistent(g) && is_full(g);
}

bool fill_column(int column, t_grid *g, char c, int count){
    bool change=false;

    if(count<g->size/2){
        return change;
    }

    for(int i=0; i!=g->size;i++){
        if(g->grid[i][column]=='_'){
            g->grid[i][column]=c;
            change=true;
        }
    }

    return change;
}

bool fill_line(int line, t_grid *g, char c, int count){
    bool change=false;

    if(count<g->size/2){
        return change;
    }
    for(int i=0; i!=g->size;i++){
        if(g->grid[line][i]=='_'){
            g->grid[line][i]=c;
            change=true;
        }
    }

    return change;
}

void generate_grid(int size, t_grid *g){
    if(g==NULL){
        errx(EXIT_FAILURE, "trying to generate a grid on an empty pointer\n");
    }

    grid_allocate(g,size);
    int set_number = percentage*size*size/100;
    int tries = set_number*100000;

    int i = rand()%size, j = rand()%size;

    while(set_number!=0 && tries!=0){
        while(g->grid[i][j]!='_'){
            i = rand()%size;
            j = rand()%size;
        }
        char rand_char = rand()%2==1 ? '0' : '1';
        g->grid[i][j] = rand_char;
        if(!is_consistent(g)){
            g->grid[i][j] = other_char(rand_char);
        }

        if(!is_consistent(g)){
            //if we can't set a 0 or a 1 to this empty space, the grid will never have a solution, we start again from the start
            grid_free(g);
            generate_grid(size,g);
        }
        set_number--;
        tries--;
    }

}

bool solve(t_grid *g){
    bool consistent=true;
    while(heuristics(g) && consistent){
        consistent= is_consistent(g);
    }

    return consistent;
}

void grid_choice_apply(t_grid *g,const t_choice choice){
    g->grid[choice.row][choice.column]=choice.choice;
}

void grid_choice_print(const t_choice choice, FILE *fd){
    fprintf(fd,"Chose %c at line=%d and column=%d\n",choice.choice,choice.row+1,choice.column+1);
}

t_choice grid_choice(t_grid *g){
    t_choice choice;

    int i = rand()%g->size, j = rand()%g->size;
    char rand_char = rand()%2==1 ? '0' : '1';

    while(g->grid[i][j]!='_'){
        i = rand()%g->size;
        j = rand()%g->size;
    }

    choice.row=i;
    choice.column=j;
    choice.choice=rand_char;
    return choice;
}

/**
 * we suppose that heuristics have already applied before first entering the function
 * @param g
 * @param mode
 * @return
 */
t_grid *grid_solver(t_grid *g, const t_mode mode){
    t_choice choice = grid_choice(g);
    t_grid *copy = (t_grid *) malloc(sizeof(t_grid));
    grid_copy(g,copy);
    grid_choice_apply(copy,choice);
    bool consistent=solve(copy);

    //if the grid is not consistent here, perhaps the choice was wrong
    if(!consistent){
        choice.choice=other_char(choice.choice);
        grid_copy(g,copy);
        grid_choice_apply(copy,choice);
        grid_free(g);
        free(g);
        return grid_solver(copy,mode);
    }

    grid_free(g);
    free(g);
    if(is_full(copy)){
        return copy;
    }
    //the grid is not complete
    return grid_solver(copy,mode);
}

t_grid *grid_solver2(t_grid *g, const t_mode mode){
    t_choice choice = grid_choice(g);
    t_grid *copy = (t_grid *) malloc(sizeof(t_grid));
    grid_copy(g,copy);
    grid_choice_apply(copy,choice);
    bool consistent=solve(copy);
    bool full= is_full(copy);

    //the grid is full and consistent :
    //we can return the current grid (copy)
    if(full && consistent){
        grid_free(g);
        free(g);
        return copy;
    }

    //the grid is not consistent :
    //the choice was wrong, we go explore the other choice
    if(!consistent){
        choice.choice=other_char(choice.choice);
        grid_copy(g,copy); //we remove changes that were made
        grid_choice_apply(copy,choice); //and apply the new choice
        grid_free(g);
        free(g);
        return grid_solver2(copy,mode);
    }


    t_grid *parcours = grid_solver2(copy,mode);
    if(is_consistent(parcours)){
        //the choice was correct, we return either the first solutions or all of them
        if(mode==MODE_FIRST){
            grid_free(g);
            free(g);
            return parcours;
        }
        else{
            printf("i'm here\n");
            return NULL; // #TODO return all solutions
        }
    }
    grid_free(parcours);
    free(parcours);

    //the grid is not consistent, the choice was wrong, we backtrack
    choice.choice=other_char(choice.choice);
    grid_copy(g,copy); //we remove changes that were made
    grid_choice_apply(copy,choice); //and apply the new choice
    return grid_solver2(copy,mode);
}