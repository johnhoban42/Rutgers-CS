#include "third.h"

int main(int argc, char** argv){

    // Scan matrix X and vector y from file
    FILE* f = fopen(argv[1], "r");
    int k; int n;
    fscanf(f, "%d\n%d\n", &k, &n);

    double** x = malloc(n*sizeof(double*));
    double** y = malloc(n*sizeof(double*));

    for(int i = 0; i < n; i++){
        x[i] = malloc((k+1)*sizeof(double));
        x[i][0] = 1;
        for(int j = 1; j < k+1; j++){
            fscanf(f, "%lf,", &x[i][j]);
        }
        y[i] = malloc(1*sizeof(double));
        fscanf(f, "%lf,", &y[i][0]);
    }
    fclose(f);

    // Find least-squares vector w
    double** xtx = multiply(transpose(x, n, k+1), x, k+1, n, k+1);
    double** xtxinv = inverse(xtx, k+1);
    double** xtxinv_xt = multiply(xtxinv, transpose(x, n, k+1), k+1, k+1, n);
    double** w = multiply(xtxinv_xt, y, k+1, n, 1);

    free_matrix(x, n);
    free_matrix(xtx, k+1);
    free_matrix(xtxinv, k+1);
    free_matrix(xtxinv_xt, k+1);
    //free_matrix(y, k+1);

    // Use w to find the test case values
    f = fopen(argv[2], "r");
    int m;
    fscanf(f, "%d\n", &m);

    for(int i = 0; i < m; i++){
        double price = w[0][0];
        for(int j = 1; j < k+1; j++){
            double attr;
            fscanf(f, "%lf,", &attr);
            price += w[j][0]*attr;
        }
        printf("%.f\n", price);
    }

    fclose(f);
    free_matrix(w, k+1);

    return 0;

}
