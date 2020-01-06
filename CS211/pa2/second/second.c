#include "second.h"
int** h;


// Recursive solving algorithm
// Returns 1 if grid is filled successfully, 0 if no solution or the grid
// requires backtracking
bool solve(){

    // temp array in case we need to backtrack
    int** htemp = malloc(16*sizeof(int*));
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            htemp[i] = malloc(16*sizeof(int));
            htemp[i][j] = h[i][j];
        }
    }

    // Check that the starting grid is valid
    bool* valid = malloc(16*sizeof(bool));
    for(int i = 0; i < 16; i++){
        if(check_row(h, i, 0, valid) == NULL
        || check_column(h, i, 0, valid) == NULL
        || check_box(h, i, 0, valid) == NULL){
            free(valid);
            return false;
        }
    }
    free(valid);

    // Iterate through the grid looking for fillable boxes
    // If all boxes are filled, print the solution
    // A grid has no solution if some boxes can't be filled or if a group
    // has duplicates
    // Stop iterating when no boxes are filled on a complete pass of the grid
    bool done;
    do{
        done = true;
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                if(h[i][j] == 0x10){

                    bool* present = malloc(16*sizeof(bool));
                    present = check_row(h, i, j, present);
                    if(present == NULL){
                        free(present);
                        return 0;
                    }

                    present = check_column(h, j, i, present);
                    if(present == NULL){
                        free(present);
                        return 0;
                    }

                    int box = 4*(i/4) + j/4;
                    int pos = (j%4) + 4*(i-4*(box/4));
                    present = check_box(h, box, pos, present);
                    if(present == NULL){
                        free(present);
                        return 0;
                    }

                    // Fill in the empty box if the other 15 numbers were found
                    int new_entry = 120;
                    int count = 0;
                    for(int k = 0; k < 16; k++){
                        if(present[k] == true){
                            new_entry -= k;
                            count++;
                        }
                    }
                    if(count == 15){
                        h[i][j] = new_entry;
                        done = false;
                    }

                    free(present);

                } // end if
            } // end j
        } // end i
    }while(!done);

    // If the grid is filled, we are done
    if(is_filled(h)){
        return true;

    // If the grid is unfilled, enter a valid guess in the first empty cell
    // and proceed
    }else{
        // Find valid entries by eliminating numbers in the same row/col/box
        int* coords = first_empty(h);
        int ci = coords[0]; int cj = coords[1];
        bool* present = malloc(16*sizeof(bool));
        present = check_row(h, ci, cj, present);
        present = check_column(h, cj, ci, present);
        int box = 4*(ci/4) + cj/4;
        int pos = (cj%4) + 4*(ci-4*(box/4));
        present = check_box(h, box, pos, present);

        // Iterate through possible entries
        for(int k = 0; k < 16; k++){
            if(!present[k]){
                h[ci][cj] = k;
                int solved = solve();
                if(solved){
                    return true;
                }
                // Revert grid after a failed attempt
                h = htemp;
                free(htemp);
            }
        }

        // If no number can satisfy this box, the hexadoku is unsolvable
        return false;
    }

}


int main(int argc, char** argv){

    h = init_hexadoku(argv[1]);
    bool solved = solve();
    if(solved){
        print_hexadoku(h);
    }else{
        printf("no-solution");
    }
    free_hexadoku(h);
    return 0;

}
