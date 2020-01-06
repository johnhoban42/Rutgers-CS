#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct Variable{
	char* name;
	int value; // 0 or 1
	struct Variable* next; // for linked list storage
};
typedef struct Variable Variable;

Variable* tempvar = NULL; // head of the tempvar linked list
Variable* inputs; // array of inputs
Variable* outputs; // array of outputs

int power(int n);
int log_2(int x);
void free_temps();
void insert_temp(char* var_name, int value);
int get_temp(char* var_name);
int get_variable(char* var_name);
void set_variable(char* var_name, int value);


// power(n) = 2^n
int power(int n){
	int r = 1;
	while(n > 0){
		r *= 2;
		n--;
	}
	return r;
}

// log base 2 of x
int log_2(int x){
    int l = 0;
    while(x > 1){
        if(x % 2 == 1){return -1;} // check for bad values
        x = x >> 1;
        l++;
    }
    return l;
}


// Reset the linked list of temporary variables for the next graycode sequence
void free_temps(){
	while(tempvar != NULL){
		Variable* temp = tempvar;
		tempvar = tempvar->next;
		free(temp);
	}
}


// Add a variable to the linked list
void insert_temp(char* var_name, int value){
	Variable* v = malloc(sizeof(Variable));
	v->name = var_name;
	v->value = value;
	v->next = NULL;
	if(tempvar == NULL){
		tempvar = v;
	}else{
		Variable* ptr = tempvar;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = v;
	}
}


// Fetch a variable from the linked list
// If the value comes from an uninitialized temp variable, return -1
int get_temp(char* var_name){
	Variable* ptr = tempvar;
	while(ptr != NULL && strcmp(ptr->name, var_name) != 0){
		ptr = ptr->next;
	}
	if(ptr == NULL){return -1;}
    return ptr->value;
}


// Fetches a gate input variable's value given its name
// If the value comes from an uninitialized vairable, return -1
int get_variable(char* var_name){

	// Binary literals
	if(strcmp(var_name, "0") == 0){
		return 0;
	}
	if(strcmp(var_name, "1") == 0){
		return 1;
	}

	if(tolower(var_name[0]) == var_name[0]){ // lowercase, TEMP variable
		return get_temp(var_name);
	}
	// uppercase, INPUT variable
	// This function will never be called on a variable that hasn't already
	// been initialized, so this loop will always finish
	int i = 0;
	while(1){
		if(strcmp(inputs[i].name, var_name) == 0){
			return inputs[i].value;
		}
		i++;
	}
	return -1;
}


// Sets the value of a variable, creating a new one if necessary
void set_variable(char* var_name, int value){
	if(tolower(var_name[0]) == var_name[0]){
		insert_temp(var_name, value);
	}else{
		int i = 0;
		while(1){
			if(strcmp(outputs[i].name, var_name) == 0){
				outputs[i].value = value;
				return;
			}
			i++;
		}
	}
	return;
}
