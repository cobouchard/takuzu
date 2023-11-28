#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <err.h>
#include <time.h>

#include "../include/takuzu.h"
#include "../include/parser.h"


static struct Params parameters;

int main2(char *argv[]){
    t_grid *grid = (t_grid *) malloc(sizeof(t_grid));
    FILE *input_file;
    input_file = fopen(argv[1],"r");
    file_parser(grid,input_file,&parameters.N);
    solve(grid);
    if(!is_consistent(grid)){
        grid_print(grid, stdout);
        errx(EXIT_FAILURE,"the grid is not consistent just after heuristics\n");
    }
    if(is_full(grid)){
        grid_print(grid, stdout);
        errx(EXIT_SUCCESS,"the grid is solved just after heuristics\n");
    }

    t_grid *result=(t_grid *)malloc(sizeof (t_grid));
    result= grid_solver2(grid,MODE_FIRST);
    grid_print(result, stdout);

    grid_free(result);
    free(result);
    fclose(input_file);
    errx(EXIT_SUCCESS,"end of main2\n");
}

int main(int argc, char *argv[]) {


    //initialise to defaults values
    parameters.verbose_flag = DEFAULT_VERBOSE;
    parameters.solver_mode = DEFAULT_SOLVER_MODE;
    parameters.N = DEFAULT_N;
    parameters.all = DEFAULT_ALL;
    parameters.unique = DEFAULT_UNIQUE;
    int rand = time(NULL);
    srand(rand);
    printf("%d\n",rand);
    srand(1701109528);

    main2(argv);

    static struct option long_options[] =
            {
                    {"all",      no_argument,       0, 'a'},
                    {"generate", optional_argument, 0, 'g'},
                    {"output",   required_argument, 0, 'o'},
                    {"unique",   no_argument,       0, 'u'},
                    {"verbose",  no_argument,       0, 'v'},
                    {"help",     no_argument,       0, 'h'},
                    {0, 0,                          0, 0}
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
                parameters.all = MODE_ALL;
                break;
            case 'g':
                if (parameters.all==MODE_ALL) {
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


        FILE *input_file;
        FILE *output_file;

        input_file = fopen(input, "r");
        if (input_file == NULL) {
            errx(EXIT_FAILURE, "the file %s doesn't exist", input);
        }

        printf("opening the grid from \"%s\" file\n", input);
        t_grid *grid = (t_grid *) malloc(sizeof(t_grid));
        file_parser(grid,input_file,&parameters.N);

        if (parameters.output == NULL) {
            output_file=stdout;
        } else {
            output_file = fopen(parameters.output, "w+");
            if(output_file==NULL){
                errx(EXIT_FAILURE,"Couldn't open output file %s\n",parameters.output);
            }
        }

        if(!is_consistent(grid)){
            //if the grid is not consistent, we wont try to solve it and exit with an error
            errx(EXIT_FAILURE, "The grid given in input is not consistent.\n");
        }

        if(!solve(grid)){
            //the grid is not consistent after only application of heuristics, it's an impossible grid
            grid_print(grid,stdout);
            errx(EXIT_FAILURE,"grid cannot be solved, not consistent after heuristics");
        }



        if(!is_full(grid)){
            //the grid haven't been solved only by the heuristics, we apply backtracking
            grid= grid_solver2(grid,MODE_FIRST);
        } else{
            //the grid have been solved by the heuristics only
            //result=grid;
        }

        if(is_valid(grid)){
            printf("The grid has been solved !\n");
        }
        else{
            printf("The grid hasn't been solved entirely or cannot be solved.\n");
        }
        grid_print(grid,output_file);
        grid_free(grid);
        free(grid);
        fclose(input_file);
        if(output_file!=stdout){
            fclose(output_file);
        }

    } else if (argv[optind] != NULL) {
        errx(EXIT_FAILURE, "too many arguments for generator mode, no file needed");
    }
    else{ //generator mode
        t_grid *generated = (t_grid *) malloc(sizeof(t_grid));
        generate_grid(parameters.N,generated);
        if(!is_consistent(generated)){
            grid_free(generated);
            free(generated);
            errx(EXIT_FAILURE,"The generated grid is not consistent\n");
        }
        if(parameters.output==NULL){
            grid_print(generated,stdout);
        }
        else{
            FILE *file;
            file = fopen(parameters.output, "w+");
            if(file==NULL){
                grid_free(generated);
                free(generated);
                errx(EXIT_FAILURE, "Couldn't open file %s to write the output of generator\n",parameters.output);
            }
            grid_print(generated,file);
        }

        grid_free(generated);
        free(generated);
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




