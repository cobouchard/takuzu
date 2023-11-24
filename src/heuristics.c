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
    for(int i=0; i!=g->size; i++){
        for(int j=0; j!=g->size; j++){
            current_char=g->grid[i][j];
            if(current_char=='_'){
                continue;
            }

            //check for consecutive char in columns
            if(check_coord(i,j+1,g->size)){
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
            }


            //check for consecutives char in lines

            if(check_coord(i+1,j,g->size)){
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

bool heur_between(t_grid *g){
    bool change=false;
    for(int i=0; i!=g->size; i++){
        for(int j=0; j!=g->size; j++){
            char current_char=g->grid[i][j];
            if(current_char!='_'){
                continue;
            }

            //we check that left and right are equals
            if(check_coord(i-1,j,g->size) && check_coord(i+1,j,g->size)){
                if(g->grid[i-1][j]==g->grid[i+1][j] && g->grid[i-1][j]!='_'){
                    change=true;
                    set_cell(i,j,g,other_char(g->grid[i-1][j]));
                }
            }

            //we do the same for top and bottom
            if(check_coord(i,j-1,g->size) && check_coord(i,j+1,g->size)){
                if(g->grid[i][j-1]==g->grid[i][j+1] && g->grid[i][j-1]!='_'){
                    change=true;
                    set_cell(i,j,g,other_char(g->grid[i][j-1]));
                }
            }
        }
    }
    return change;
}