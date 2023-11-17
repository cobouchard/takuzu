#include <stdbool.h>
#include "../include/grid.h"
#include "../include/heuristics.h"

/**
 * when 2 consecutive characters are found (1 or 0) the next character is set to the opposite (if it was not filled)
 * @param g
 * @return return true if the grid has been changed, false otherwise
 */
bool heur_consecutive(t_grid *g){
    bool change = false;
    char current_char = '_';

    for(int i=0; i!=g->size-1; i++){
        for(int j=0; j!=g->size-1; j++){
            current_char=g->grid[i][j];
            if(current_char=='_'){
                continue;
            }
            if(g->grid[i][j+1]==current_char){
                if(check_coord(i,j-1,g->size)){
                    if(check_empty(i,j-1,g)){
                        change=true;
                        set_cell(i,j-1,g, other_char(current_char));
                    }
                }
                if(check_coord(i,j+2,g->size)){
                    if(check_empty(i,j+2,g)){
                        change=true;
                        set_cell(i,j+2,g,other_char(current_char));
                    }
                }
            }

            if(g->grid[i+1][j]==current_char){
                if(check_coord(i-1,j,g->size)){
                    if(check_empty(i-1,j,g)){
                        change=true;
                        set_cell(i-1,j,g,other_char(current_char));
                    }
                }
                if(check_coord(i+2,j,g->size)){
                    if(check_empty(i+2,j,g)){
                        change=true;
                        set_cell(i+2,j,g,other_char(current_char));
                    }
                }
            }
        }
    }


    return change;
}

bool heur_fill(t_grid *g){
    int count_1_line=0, count_0_line=0, count_1_column=0, count_0_column=0;
    bool change=false;
    for(int i=0; i!=g->size; i++){
        count_0_column=0; count_0_line=0; count_1_column=0; count_1_line=0;
        for(int j=0; j!=g->size; j++){
            count_1_line += (g->grid[i][j]=='1') ? 1 : 0;
            count_0_line += (g->grid[i][j]=='0') ? 1 : 0;
            count_1_column += (g->grid[j][i]=='1') ? 1 : 0;
            count_0_column += (g->grid[j][i]=='0') ? 1 : 0;
        }
        change |= fill_column(i,g,'1',count_0_column);
        change |= fill_column(i,g,'0',count_1_column);
        change |= fill_line(i,g,'1',count_0_line);
        change |= fill_line(i,g,'0',count_1_line);
    }

    return change;
}