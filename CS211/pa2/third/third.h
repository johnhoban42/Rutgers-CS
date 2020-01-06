#include <stdlib.h>
#include <stdio.h>

double** multiply(double** a, double** b, int a_rows, int a_cols, int b_cols);
double** transpose(double** x, int x_rows, int x_cols);
double** inverse(double** x, int x_rows);
void print_matrix(double** x, int x_rows, int x_cols);
void free_matrix(double** x, int x_rows);


double** multiply(double** a, double** b, int a_rows, int a_cols, int b_cols){

	double** c = malloc(a_rows*sizeof(double*));
	for(int i = 0; i < a_rows; i++){
		c[i] = malloc(b_cols*sizeof(double));
		for(int j = 0; j < b_cols; j++){
			for(int k = 0; k < a_cols; k++){
				c[i][j] += a[i][k]*b[k][j];
			}
		}
	}

    return c;

}


double** transpose(double** x, int x_rows, int x_cols){

    // Allocate memory for the transpose
    double** xt = malloc(x_cols*sizeof(double*));
    for(int i = 0; i < x_cols; i++){
        xt[i] = malloc(x_rows*sizeof(double));
    }

    // Perform transposition
    for(int i = 0; i < x_cols; i++){
        for(int j = 0; j < x_rows; j++){
            xt[i][j] = x[j][i];
        }
    }

    return xt;

}


double** inverse(double** x, int x_rows){

    // Build augmented matrix [A B]
    double** a = malloc(x_rows*sizeof(double*));
    for(int i = 0; i < x_rows; i++){
        a[i] = malloc(2*x_rows*sizeof(double));
        for(int j = 0; j < x_rows; j++){
            a[i][j] = x[i][j];
            if(j == i){
                a[i][x_rows + j] = 1;
            }else{
                a[i][x_rows + j] = 0;
            }
        }
    }

    // Find rref([A B])
    for(int i = 0; i < x_rows; i++){

        // Divide row i by the pivot entry in row i to set pivot[i] = 1
        double d = a[i][i];
		if(d == 0.0){
			printf("i = %d, Div by 0", i);
			//return NULL;
		}
        for(int j = 0; j < 2*x_rows; j++){
            a[i][j] /= d;
        }

        // Subtract multiples of row i from all other rows "j" such that
        // A[j][i] = 0
        for(int j = 0; j < x_rows; j++){
            if(j != i){
                double multiplier = a[j][i];
                for(int k = 0; k < 2*x_rows; k++){
                    a[j][k] -= (multiplier * a[i][k]);
                }
            }
        }

    }

    // Extract B = A^-1 from the augmented matrix
    double** b = malloc(x_rows*sizeof(double*));
    for(int i = 0; i < x_rows; i++){
        b[i] = malloc(x_rows*sizeof(double));
        for(int j = 0; j < x_rows; j++){
            b[i][j] = a[i][x_rows + j];
        }
    }

    free_matrix(a, x_rows);
    return b;

}


void print_matrix(double** x, int x_rows, int x_cols){
    for(int i = 0; i < x_rows; i++){
        for(int j = 0; j < x_cols; j++){
            printf("%f\t", x[i][j]);
        }
        printf("\n");
    }
}


void free_matrix(double** x, int x_rows){

	for(int i = 0; i < x_rows; i++){
		free(x[i]);
	}
	free(x);

}
