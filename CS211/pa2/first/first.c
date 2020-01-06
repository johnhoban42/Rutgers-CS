#include "first.h"


int main(int argc, char** argv){

    int** h = init_hexadoku(argv[1]);

    // Check that the starting grid is valid
    bool* valid = malloc(16*sizeof(bool));
    for(int i = 0; i < 16; i++){
        if(check_row(h, i, 0, valid) == NULL
        || check_column(h, i, 0, valid) == NULL
        || check_box(h, i, 0, valid) == NULL){
            printf("no-solution");
            free(valid);
            free_hexadoku(h);
            return 0;
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
                        printf("no-solution");
                        free(present);
                        free_hexadoku(h);
                        return 0;
                    }

                    present = check_column(h, j, i, present);
                    if(present == NULL){
                        printf("no-solution");
                        free(present);
                        free_hexadoku(h);
                        return 0;
                    }

                    int box = 4*(i/4) + j/4;
                    int pos = (j%4) + 4*(i-4*(box/4));
                    present = check_box(h, box, pos, present);
                    if(present == NULL){
                        printf("no-solution");
                        free(present);
                        free_hexadoku(h);
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

    if(is_filled(h)){
        print_hexadoku(h);
    }else{
        printf("no-solution");
    }
    free_hexadoku(h);


    return 0;

}
