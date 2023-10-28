#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <err.h>

#include "../include/takuzu.h"
#include "../include/grid.h"

#define DEFAULT_VERBOSE 0
#define DEFAULT_SOLVER_MODE true
#define DEFAULT_ALL false
#define DEFAULT_N 8
#define DEFAULT_UNIQUE false

#define MAX_BUFFER 256
#define MAX_GRID_SIZE 64

//this defines states for the parsing function, the current state indicating what kind of character is expected
#define COMMENT 0
#define SIGNIFICANT_CHARACTER 1
#define SEPARATOR 2





void print_help();

int checkArgGenerator(char *arg);

int checkArgGeneratorInt(int N);

void grid_print(const t_grid *g, FILE *fd);

bool check_char(const char c);

bool check_separator(const char c);

void file_parser(t_grid *grid, char *filename);

char *readLine(char *line, int size, int current_line);

int countCharInString(char* string);

void end_of_main(char *output);

static struct Params parameters;

int main(int argc, char *argv[]) {


    //initialise to defaults values
    parameters.verbose_flag = DEFAULT_VERBOSE;
    parameters.solver_mode = DEFAULT_SOLVER_MODE;
    parameters.N = DEFAULT_N;
    parameters.all = DEFAULT_ALL;
    parameters.unique = DEFAULT_UNIQUE;

    static struct option long_options[] =
            {
                    {"all",      no_argument,       0, 'a'},
                    {"generate", optional_argument, 0, 'g'},
                    {"output",   required_argument, 0, 'o'},
                    {"unique",   no_argument,       0, 'u'},
                    {"verbose",  no_argument,       0, 'v'},
                    {"help",     no_argument,       0, 'h'},
                    {0,          0,                 0, 0}
            };

    int c;
    int option_index = 0;
    do {
        c = getopt_long(argc, argv, "auvhg::o:", long_options, &option_index);
        switch (c) {
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            case 'v':
                parameters.verbose_flag = 1;
                printf("Verbose mode on\n");
                break;
            case 'u':
                if (parameters.solver_mode) {
                    warnx("option \"unique\" incompatible with mode \"solver\" ");
                    exit(EXIT_FAILURE);
                }

                printf("Generating a grid with unique solution\n");
                parameters.unique = true;
                break;
            case 'o':
                parameters.output = optarg;
                printf("output will be written in %s \n", optarg);
                break;
            case 'a':
                if (!parameters.solver_mode) {
                    warnx("option \"all\" incompatible with mode \"generator\" ");
                    exit(EXIT_FAILURE);
                }

                printf("Searching for all possibles solutions\n");
                parameters.all = true;
                break;
            case 'g':
                if (parameters.all) {
                    warnx("mode \"generator\" incompatible with option \"all\" ");
                    exit(EXIT_FAILURE);
                }
                int temp_N;
                if (argv[optind] != NULL) {
                    temp_N = checkArgGenerator(argv[optind]);
                    if (temp_N == -1) {
                        errx(EXIT_FAILURE, "grids size must be 4, 8, 16, 32 or 64");
                    }

                    parameters.N = temp_N;
                    optind++;
                }

                printf("Generating grids for N=%d\n", parameters.N);
                parameters.solver_mode = false;
                break;
        }
    } while (c != -1);

    if (parameters.solver_mode) {
        char *input = argv[optind];

        if (input == NULL) {
            errx(EXIT_FAILURE, "no input grid file is given for solver mode");
        }


        if (argv[optind + 1]) {
            errx(EXIT_FAILURE, "too many arguments for solver mode");
        }


        FILE *file;
        file = fopen(input, "r");
        if (file == NULL) {
            errx(EXIT_FAILURE, "the file %s doesn't exist", input);
        }

        fclose(file);
        printf("opening the grid from \"%s\" file\n", input);
        parameters.input = input;
    } else if (argv[optind] != NULL) {
        errx(EXIT_FAILURE, "too many arguments for generator mode, no file needed");
    }


    end_of_main("grid_sortie.txt");
}

void print_help() {
    printf("Usage: \ttakuzu[-a|-o FILE|-v|-h] FILE... \n\ttakuzu -g[SIZE] [-u|-o FILE|-v|-h] \nSolve or generate takuzu grids of size: 4, 8, 16, 32, 64\n \
    -a, --all \t \t \t \tsearch for all possible solutions\n \
    -g[N], --generate[N] \t \tgenerate a grid of size NxN (default:8)\n \
    -o FILE, --output FILE \t \twrite output to FILE\n \
    -u, --unique \t \t \tgenerate a grid with unique solution\n \
    -v, --verbose \t \t \tverbose output \n \
    -h, --help \t \t \tdisplay this help and exit \n");

}

/**
 *
 * @param arg string entered to the terminal, should come directly from argv
 * @return return -1 if the number is not in the list (4,8,16,32,64)
 */
int checkArgGenerator(char *arg) {
    char *end;
    long temp_N = strtol(arg, &end, 10);
    if (end == arg) {
        return false;
    }

    if (temp_N == 4 || temp_N == 8 || temp_N == 16 || temp_N == 32 || temp_N == 64) {
        return temp_N;
    }

    return -1;
}

int checkArgGeneratorInt(int N){
    if (N == 4 || N == 8 || N == 16 || N == 32 || N == 64) {
        return N;
    }

    return -1;
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

bool check_char(const char c) {
    return c == '0' || c == '1' || c == '_';
}

bool check_separator(const char c) {
    return c == '\t' || c == ' ';
}

void file_parser(t_grid *grid, char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    if(file==NULL){
        errx(EXIT_FAILURE, "can't open file %s in function file_parser.", filename);
    }


    char buffer[MAX_BUFFER];
    char *line;
    int current_line = 0;
    int temp_size;


    //let's read the first line, find the size of the grid and allocate the grid to store the data parsed

    do{
        if (fgets(buffer, MAX_BUFFER, file) != NULL) {
            line = readLine(buffer, MAX_GRID_SIZE, current_line);
            temp_size = countCharInString(line);
        }
        else{
            errx(EXIT_FAILURE, "File %s is empty !", filename);
        }
    }while(temp_size==0);

    temp_size = checkArgGeneratorInt(temp_size);
    current_line++;

    if(temp_size==-1){
        errx(EXIT_FAILURE, "Incorrect number of significant characters, grid size must be 4, 8, 16, 32 or 64.\n");
    }
    parameters.N=temp_size;
    grid_allocate(grid,parameters.N);
    for(int i=0; i!=parameters.N; i++){
        grid->grid[0][i]=line[i];
    }

    //we will now check that the next lines are the same size as the first one and finish the parsing of the file

    while(fgets(buffer, MAX_BUFFER, file) != NULL){

        line = readLine(buffer, parameters.N, current_line);
        int current_size=countCharInString(line);


        //there are no significant characters, empty line or commented line
        if(current_size==0) {
            //we go to the next line
            continue;
        }


        if(current_size!=parameters.N){
            errx(EXIT_FAILURE,"Incorrect number of significant characters '%d' in line '%d'; %d was expected as in the first uncommented line.\n", current_size, current_line, parameters.N);
        }

        for(int i=0; i!=parameters.N; i++){
            grid->grid[current_line][i]=line[i];
        }
        current_line++;

    }

    if(current_line!=parameters.N){
        errx(EXIT_FAILURE, "Incorrect number of lines, %d lines found.", current_line);
    }

    fclose(file);
}

int countCharInString(char* string){
    int sum=0;

    char c = string[0];
    while(c!='\n' && c!='\0'){
        sum++;
        c=string[sum];
    }

    return sum;
}

/**
 *
 * @param line
 * @param size size of the grid if known, if not known (first time going in this function, should be size 64 and the size will be determined thanks to the first line
 * @return the corresponding line of the grid (only significant characters)
 */
char *readLine(char *line, int size, int current_line) {

    char current_char = '\0';
    int current_index = 0;
    char *line_of_grid = (char *) malloc(size * sizeof(char *));
    if (line_of_grid == NULL) {
        warnx("Fail in the allocation of line_of_grid in readLine function");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; current_char != '\n'; i++) {
        current_char = line[i];
        if (current_char != '\n') {
            if (current_char == '#') {
                current_char='\n';
            } else if(check_separator(current_char)){
                continue;
            } else if(check_char(current_char)){
                if(current_index>size){
                    errx(EXIT_FAILURE,"Exceeding 64 significant characters, grid cannot be bigger, exiting.");
                }

                line_of_grid[current_index] = current_char;
                current_index++;
            } else{
                errx(EXIT_FAILURE,
                     "Unexpected character '%c' at line %d, column %d, a significant character was expected.\n",
                     current_char, ++current_line, ++current_index);
            }

        }

    }
    line_of_grid[current_index]='\n';
    return line_of_grid;
}

void end_of_main(char *output) {
    FILE *file;
    file = fopen(output, "w+");

    t_grid *myGrid = (t_grid *) malloc(sizeof(t_grid));
    grid_allocate(myGrid, parameters.N);

    file_parser(myGrid, parameters.input);
    grid_print(myGrid, file);

    fclose(file);
    grid_free(myGrid);
}