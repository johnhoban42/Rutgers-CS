#include <stdlib.h>
#include <stdio.h>


void free_matrix(int** a, int rows);


int main(int argc, char** argv){

	FILE* f = fopen(argv[1], "r");

	// Read first array
	int r1;
	int c1;
	fscanf(f, "%d %d\n", &r1, &c1);
	int** a = malloc(r1*sizeof(int*));
	for(int i = 0; i < r1; i++){
		a[i] = malloc(c1*sizeof(int));
		for(int j = 0; j < c1; j++){
			fscanf(f, "%d", &a[i][j]);
		}
	}

	// Read second array
	// If c1 != r2, then the multiplication is invalid and the program
	// terminates
	int r2;
	int c2;
	fscanf(f, "%d %d\n", &r2, &c2);
	if(c1 != r2){
		printf("bad-matrices");
		free_matrix(a, r1);
		exit(0);
	}
	int** b = malloc(r2*sizeof(int*));
	for(int i = 0; i < r2; i++){
		b[i] = malloc(c2*sizeof(int));
		for(int j = 0; j < c2; j++){
			fscanf(f, "%d", &b[i][j]);
		}
	}

	// Multiply
	int** c = malloc(r1*sizeof(int*));
	for(int i = 0; i < r1; i++){
		c[i] = malloc(c2*sizeof(int));
		for(int j = 0; j < c2; j++){
			for(int k = 0; k < c1; k++){
				c[i][j] += a[i][k]*b[k][j];
			}
		}
	}

	// Print matrix
	for(int i = 0; i < r1; i++){
		for(int j = 0; j < c2; j++){
			printf("%d	", c[i][j]);
		}
		printf("\n");
	}

	free_matrix(a, r1);
	free_matrix(b, r2);
	free_matrix(c, r1);

	fclose(f);

	return 0;

}

// Free memory of a matrix by passing a pointer to the matrix
void free_matrix(int** a, int rows){
	for(int i = 0; i < rows; i++){
		free(a[i]);
	}
	free(a);
}
