#include <stdbool.h>
#include "../include/grid.h"

/**
 * when 2 consecutive characters are found (1 or 0) the next character is set to the opposite (if it was not filled)
 * @param g
 * @return return true if the grid has been changed, false otherwise
 */
bool heur_consecutive(t_grid *g){
    char current_char='_';
    bool change = false; //#TODO currently check only from left to right, meaning that if the size-2 character is empty and the next two are the same, it won't be updated
    for(int i=0; i!=g->size-2; i++){
        for(int j=0; j!=g->size-2; j++){
            if(g->grid[i][j]==current_char){
                //we update the cells on the left and right, (one on the right and two on the left)

            }

            current_char=g->grid[i][j];
            //the current implementation is probably not that smart
            //on the column (for a given line), if there are two consecutive 0 (resp 1), we might update next character to 1 (resp 0)

            //from left to right (check 2 cells and update the one on the right)
            if(g->grid[i][j]=='0' && g->grid[i][j+1]=='0'){
                if(g->grid[i][j+2]=='_'){
                    g->grid[i][j+2]='1';
                    change=true;
                }
            }
            if(g->grid[i][j]=='1' && g->grid[i][j+1]=='1'){
                if(g->grid[i][j+2]=='_'){
                    g->grid[i][j+2]='0';
                    change=true;
                }
            }

            //We do the same for lines
            if(g->grid[i][j]=='0' && g->grid[i+1][j]=='0'){
                if(g->grid[i+2][j]=='_'){
                    g->grid[i+2][j]='1';
                    change=true;
                }
            }
            if(g->grid[i][j]=='1' && g->grid[i+1][j]=='1'){
                if(g->grid[i+2][j]=='_'){
                    g->grid[i+2][j]='0';
                    change=true;
                }
            }

            //from right to left (check 2 cells and update the one on the left)
            //for columns
            if(g->grid[g->size-i-1][g->size-j-1]=='0' && g->grid[g->size-i-1][g->size-j-2]=='0'){
                if(g->grid[g->size-i-1][g->size-j-3]=='_'){
                    g->grid[g->size-i-1][g->size-j-3]='1';
                    change=true;
                }
            }
            if(g->grid[g->size-i-1][g->size-j-1]=='1' && g->grid[g->size-i-1][g->size-j-2]=='1'){
                if(g->grid[g->size-i-1][g->size-j-3]=='_'){
                    g->grid[g->size-i-1][g->size-j-3]='0';
                    change=true;
                }
            }

            //for lines
            printf("%c is here (%d,%d) and %c %c \n", g->grid[g->size-i-1][g->size-j-1],g->size-i-1,g->size-j-1,g->grid[g->size-i-2][g->size-j-1],g->grid[g->size-i-3][g->size-j-1]);
            if(g->grid[g->size-i-1][g->size-j-1]=='0' && g->grid[g->size-i-2][g->size-j-1]=='0'){
                if(g->grid[g->size-i-3][g->size-j-1]=='_'){
                    g->grid[g->size-i-3][g->size-j-1]='1';
                    change=true;
                }
            }
            if(g->grid[g->size-i-1][g->size-j-1]=='1' && g->grid[g->size-i-2][g->size-j-1]=='1'){
                if(g->grid[g->size-i-3][g->size-j-1]=='_'){
                    g->grid[g->size-i-3][g->size-j-1]='0';
                    change=true;
                }
            }
        }

    }


    return change;
}

bool heur_fill(t_grid *g){
    return false;
}