#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>

int is_prime(int x);

int main(int argc, char** argv){

	// Open and read from file
	FILE* f = fopen(argv[1], "r");
	int n;
	fscanf(f, "%d\n", &n);

	// Read each number. Check if "k" is prime, then check if (k%10) is prime
	// until k = 0
	for(int i = 0; i < n; i++){
		int k;
		fscanf(f, "%d\n", &k);
		while(k > 0){
			if(is_prime(k) == 0){
				printf("no\n");
				break;
			}
			k = k / 10;
		}
		if(k == 0){
			printf("yes\n");
		}
	}

	fclose(f);
	return 0;

}


// Checks if a number x is prime by checking for divisors from 2 to x
int is_prime(int x){

	if(x == 1){
		return 0;
	}
	for(int i = 2; i < x; i++){
		if(x % i == 0){
			return 0;
		}
	}
	return 1;

}
