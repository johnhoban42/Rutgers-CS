#include <stdlib.h>
#include <stdio.h>

int** scan_matrix(char** file);
void free_matrix(int** a, int rows);


int main(int argc, char** argv){

    // Read file and fill in magic square
    FILE* f = fopen(argv[1], "r");
    int n;
    fscanf(f, "%d\n", &n);
    int** m = malloc(n*sizeof(int*));
    for(int i = 0; i < n; i++){
        m[i] = malloc(n*sizeof(int));
        for(int j = 0; j < n; j++){
            fscanf(f, "%d", &m[i][j]);
        }
    }
    fclose(f);

    // Check for duplicate entries in the matrix
    int* entries = malloc(n*n*sizeof(int));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n*i + j; k++){
                if(m[i][j] == entries[k]){
                    printf("not-magic");
                    free(entries);
                    free_matrix(m, n);
                    exit(0);
                }
            }
            entries[n*i + j] = m[i][j];
        }
    }
    free(entries);

    // Check sum of first row. Then compare the sum to all other rows,
    // columns, and diagonals.
    int sum = 0;
    for(int i = 0; i < n; i++){
        sum += m[0][i];
    }

    for(int i = 1; i < n; i++){
        int temp = 0;
        for(int j = 0; j < n; j++){
            temp += m[i][j];
        }
        if(temp != sum){
            printf("not-magic");
            free_matrix(m, n);
            exit(0);
        }
    }

    for(int i = 0; i < n; i++){
        int temp = 0;
        for(int j = 0; j < n; j++){
            temp += m[j][i];
        }
        if(temp != sum){
            printf("not-magic");
            free_matrix(m, n);
            exit(0);
        }
    }

    int temp = 0;
    for(int i = 0; i < n; i++){
        temp += m[i][i];
    }
    if(temp != sum){
        printf("not-magic");
        free_matrix(m, n);
        exit(0);
    }

    temp = 0;
    for(int i = 0; i < n; i++){
        temp += m[i][n-i-1];
    }
    if(temp != sum){
        printf("not-magic");
        free_matrix(m, n);
        exit(0);
    }

    printf("magic");
    free_matrix(m, n);
    return 0;

}


// Free memory of a matrix by passing a pointer to the matrix
void free_matrix(int** a, int rows){
	for(int i = 0; i < rows; i++){
		free(a[i]);
	}
	free(a);
}
