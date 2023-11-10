#include <stdbool.h>
#include "../include/grid.h"

/**
 * when 2 consecutive characters are found (1 or 0) the next character is set to the opposite (if it was not filled)
 * @param g
 * @return return true if the grid has been changed, false otherwise
 */
bool heur_consecutive(t_grid *g){
    bool change = false;

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

    // one full go through to make the lines/columnsCode array (O(n²))
    // then we go through the arrays (O(n))

    for(int i=0; i!=g->size; i++){
        for(int j=0; j!=g->size; j++){

            //(position1 & position1>>1) >> 1 should be the list of possibles positions on the right
            //(position1 & position1>>1) << 2 should be the list of possible positions on the left

            //try to get possible positions of new 1 and 0
            //use the method to check 2 consecutive char
            //test the different positions if there is an underscore
            //and with underscore positions and 1  (resp 0) positions


            //once we have the final binary result, we go through it by bit shifting to the right, we get the position by incrementing i every time
            //be careful to only bitshift for grid size times(or perhaps grid size -1), so we don't go above the array

            //since we do it for line and columns, 1 and 0, left and right, we end up having 8 integers each time
        }

    }


    return change;
}

bool heur_fill(t_grid *g){
    return false;
}