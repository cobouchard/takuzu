#include <stdbool.h>
#include "../include/grid.h"

/**
 * when 2 consecutive characters are found (1 or 0) the next character is set to the opposite (if it was not filled)
 * @param g
 * @return return true if the grid has been changed, false otherwise
 */
bool heur_consecutive(t_grid *g){
    bool change = false; //#TODO currently check only from left to right, meaning that if the size-2 character is empty and the next two are the same, it won't be updated
    for(int i=0; i!=g->size-2; i++){
        for(int j=0; j!=g->size-2; j++){
            //on the line there are two consecutive 0 (resp 1), we might update next character to 1 (resp 0)
            if(g->grid[i][j]=='0' && g->grid[i][j+1]=='0'){
                if(g->grid[i][j+2]=='_'){
                    g->grid[i][j+2]='1';
                    change=true;
                }
            }
            //printf("current char is %c and next one is %c\n", g->grid[i][j],g->grid[i][j+1]);
            if(g->grid[i][j]=='1' && g->grid[i][j+1]=='1'){
                if(g->grid[i][j+2]=='_'){
                    g->grid[i][j+2]='0';
                    change=true;
                }
            }

            //We do the same for columns
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
        }

        //#TODO we must check the last case, if the two last characters of a line/column are equal, the one before can be set
    }


    return change;
}

bool heur_fill(t_grid *g){
    return false;
}