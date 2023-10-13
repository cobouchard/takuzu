#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <err.h>

#define DEFAULT_VERBOSE 0
#define DEFAULT_SOLVER_MODE true
#define DEFAULT_ALL false
#define DEFAULT_N 8
#define DEFAULT_UNIQUE false

static struct Params {
    int verbose_flag;
    bool solver_mode; //The program is on solver_mode by default, set to false if the program is running on generation mode
    bool unique;
    bool all;
    int N;
    char *output;
    char *input;
};

void print_help();

bool checkArgGenerator(char *arg);

int main(int argc, char *argv[]) {
    struct Params parameters;

    //initialise to defaults values
    parameters.verbose_flag = DEFAULT_VERBOSE;
    parameters.solver_mode = DEFAULT_SOLVER_MODE;
    parameters.N = DEFAULT_N;
    parameters.all = DEFAULT_ALL;
    parameters.unique = DEFAULT_UNIQUE;

    static struct option long_options[] =
            {
                    {"all",      no_argument,       0, 'a'},
                    {"generate", required_argument, 0, 'g'},
                    {"output",   required_argument, 0, 'o'},
                    {"unique",   no_argument,       0, 'u'},
                    {"verbose",  no_argument,       0, 'v'},
                    {"help",     no_argument,       0, 'h'},
                    {0,          0,                 0, 0}
            };

    int c;
    int option_index = 0;
    do {
        c = getopt_long(argc, argv, "auvhg:o:", long_options, &option_index);
        switch (c) {
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            case 'v':
                parameters.verbose_flag = 1;
                break;
            case 'u':
                if (parameters.solver_mode)
                    errx(EXIT_FAILURE, "option \"unique\" incompatible with mode \"solver\" ");

                parameters.unique = true;
                break;
            case 'o':
                parameters.output = optarg;
                printf("output will be written in %s \n", optarg);
                break;
            case 'a':
                if (!parameters.solver_mode)
                    errx(EXIT_FAILURE, "option \"all\" incompatible with mode \"generator\" ");

                parameters.all = true;
                break;
            case 'g':
                if (parameters.all)
                    errx(EXIT_FAILURE, "mode \"generator\" incompatible with option \"all\" ");

                if (!checkArgGenerator(optarg))
                    errx(EXIT_FAILURE, "grids size must be 4, 8, 16, 32 or 64\n");

                parameters.solver_mode = false;
                break;
        }
    } while (c != -1);

    if (parameters.solver_mode) {
        char *input = argv[optind];
        if (input == NULL)
            errx(EXIT_FAILURE, "no input grid file is given");

        FILE *file;
        file = fopen(input, "r");
        if (file == NULL)
            errx(EXIT_FAILURE, "the file %s doesn't exist", input);
        fclose(file);
        printf("opening the grid from \"%s\" file\n", input);
        parameters.input = input;
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

bool checkArgGenerator(char *arg) {
    char *end;
    long temp_N = strtol(arg, &end, 10);
    if (end == arg)
        return false;
    return (temp_N == 4 || temp_N == 8 || temp_N == 16 || temp_N == 32 || temp_N == 64);
}

//trop d'argument sur generator
//generate n optionnel
//warning sur conflit de parametres