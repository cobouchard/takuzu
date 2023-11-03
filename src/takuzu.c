#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <err.h>

#include "../include/takuzu.h"
#include "../include/parser.h"

#define DEFAULT_VERBOSE 0
#define DEFAULT_SOLVER_MODE true
#define DEFAULT_ALL false
#define DEFAULT_N 8
#define DEFAULT_UNIQUE false


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

        if (parameters.output == NULL) {
            end_of_main("grid_sortie.txt");
        } else {
            end_of_main(parameters.output);
        }
    } else if (argv[optind] != NULL) {
        errx(EXIT_FAILURE, "too many arguments for generator mode, no file needed");
    }


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

    return checkArgGeneratorInt(temp_N);
}

int checkArgGeneratorInt(int N) {
    if (N == 4 || N == 8 || N == 16 || N == 32 || N == 64) {
        return N;
    }

    return -1;
}


void end_of_main(char *output) {
    FILE *file;
    file = fopen(output, "w+");

    t_grid *myGrid = (t_grid *)malloc(sizeof (t_grid));

    file_parser(myGrid, parameters.input, &(parameters.N));
    grid_print(myGrid, file);


    if (!isConsistent(myGrid))
        printf("the grid is not consistent\n");
    else
        printf("the grid is consistent\n");


    fclose(file);
    grid_free(myGrid);
    free(myGrid);
}