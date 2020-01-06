#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int** init_hexadoku(char* fname);
bool* check_row(int** h, int row, int pos, bool* p);
bool* check_column(int** h, int col, int pos, bool* p);
bool* check_box(int** h, int box, int pos, bool* p);
void print_hexadoku(int** h);
void free_hexadoku(int** h);


// Reads a hexadoku grid from file
int** init_hexadoku(char* fname){
    int** h = malloc(16*sizeof(char*));
    FILE* f = fopen(fname, "r");
    for(int i = 0; i < 16; i++){
        h[i] = malloc(16*sizeof(int));
        for(int j = 0; j < 16; j++){
            fscanf(f, "%x\t", &h[i][j]);
        }
    }
    fclose(f);

    // Replace '-' with 16
    f = fopen(fname, "r");
    char c;
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            fscanf(f, "%c\t", &c);
            if(c == '-'){
                h[i][j] = 0x10;
            }
        }
    }
    fclose(f);

    return h;
}


// Check every entry in the row except for the box being checked
// Returns an array of which numbers are present in the row, along with
// any numbers from other col/box checks on that specific entry
// Returns NULL if there is a repeat within the row
bool* check_row(int** h, int row, int pos, bool* present){

    bool in_row[16];
    for(int i = 0; i < 16; i++){
        in_row[i] = false;
    }
    for(int i = 0; i < 16; i++){
        if(i != pos){
            int k = h[row][i];
            if(k == 0x10){continue;}
            if(in_row[k]){
                return NULL;
            }
            present[k] = true;
            in_row[k] = true;
        }
    }
    return present;

}


// Check every entry in the row except for the box being checked
// Returns an array of which numbers are present in the row, along with
// any numbers from other row/box checks on that specific entry
// Returns NULL if there is a repeat within the column
bool* check_column(int** h, int col, int pos, bool* present){

    bool in_col[16];
    for(int i = 0; i < 16; i++){
        in_col[i] = false;
    }
    for(int i = 0; i < 16; i++){
        if(i != pos){
            int k = h[i][col];
            if(k == 0x10){continue;}
            if(in_col[k]){
                return NULL;
            }
            present[k] = true;
            in_col[k] = true;
        }
    }
    return present;

}


// Check every entry in the row except for the box being checked
// Returns an array of which numbers are present in the row, along with
// any numbers from other row/col checks on that specific entry
// Returns NULL if there is a repeat within the box
bool* check_box(int** h, int box, int pos, bool* present){

    bool in_box[16];
    for(int i = 0; i < 16; i++){
        in_box[i] = false;
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if((j%4) + 4*i != pos){
                int k = h[4*(box/4)+i][4*(box%4)+j];
                if(k == 0x10){continue;}
                if(in_box[k]){
                    return NULL;
                }
                present[k] = true;
                in_box[k] = true;
            }
        }
    }
    return present;

}


// Checks if a hexadoku is filled
bool is_filled(int** h){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(h[i][j] == 0x10){
                return false;
            }
        }
    }
    return true;
}


// Prints the hexadoku grid
void print_hexadoku(int** h){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            int entry = h[i][j];
            if(entry < 0xA || entry > 0xF){
                printf("%x\t", entry);
            }else if(entry == 0xA){
                printf("A\t");
            }else if(entry == 0xB){
                printf("B\t");
            }else if(entry == 0xC){
                printf("C\t");
            }else if(entry == 0xD){
                printf("D\t");
            }else if(entry == 0xE){
                printf("E\t");
            }else{
                printf("F\t");
            }
        }
        printf("\n");
    }
}


// Prints the hexadoku grid
void free_hexadoku(int** h){
    for(int i = 0; i < 16; i++){
        free(h[i]);
    }
    free(h);
}
