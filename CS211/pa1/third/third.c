#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// STRUCTS
struct Node{
	int key;
	struct Node* next;
};
typedef struct Node Node;

// FUNCTION PROTOTYPES
void insert(int key);
void search(int key);
void print_list(Node* head);
void print_hashtable();

// GLOBALS
Node** hashtable;


int main(int argc, char** argv){

	// Open the file
	FILE* f = fopen(argv[1], "r");

	// Initialize the hash table
	hashtable = malloc(1000*sizeof(Node*));

	// Read input
	char cmd;
	int key;
	while(fscanf(f, "%c %d\n", &cmd, &key) != EOF){
		if(cmd == 'i'){
			insert(key);
		}else if(cmd == 's'){
			search(key);
		}
	}

	// print_hashtable();

	// Free memory
	fclose(f);
	for(int i = 0; i < 1000; i++){
		Node* ptr = hashtable[i];
		while(ptr != NULL){
			Node* temp = ptr;
			ptr = ptr->next;
			free(temp);
		}
	}
	free(hashtable);
	return 0;

}


void insert(int key){

	// Hash
	int i = abs(key % 1000);

	// Insert
	Node* temp = malloc(sizeof(Node));
	temp->key = key;

	// If the bucket is empty, initialize the first node
	if(hashtable[i] == NULL){
		hashtable[i] = temp;
		printf("inserted\n");
		return;
	}

	// Otherwise, insert nodes in ascending order
	Node* ptr = hashtable[i];
	Node* prev = NULL;
	while(ptr != NULL){
		if(ptr->key == key){
			printf("duplicate\n");
			return;
		}
		else if(ptr->key > key){
			if(ptr == hashtable[i]){
				temp->next = hashtable[i];
				hashtable[i] = temp;
			}else{
				temp->next = ptr;
				prev->next = temp;
			}
			printf("inserted\n");
			return;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	prev->next = temp;
	printf("inserted\n");

}


void search(int key){

	// Hash the key and move to that bucket in the hash table
	int i = abs(key % 1000);
	Node* ptr = hashtable[i];

	// Search the chain for the key
	if(ptr == NULL){
		printf("absent\n");
		return;
	}

	while(ptr != NULL && ptr->key <= key){
		if(ptr->key == key){
			printf("present\n");
			return;
		}
		ptr = ptr->next;
	}
	printf("absent\n");

}


// Assumes the list is not NULL
void print_list(Node* head){

	for(Node* ptr = head; ptr != NULL; ptr = ptr->next){
		printf("%d -> ", ptr->key);
	}
	printf("NULL\n");

}


// Prints the non-NULL buckets in the hash table
void print_hashtable(){

	for(int i = 0; i < 1000; i++){
		if(hashtable[i] != NULL){
			printf("[%d]: ", i);
			print_list(hashtable[i]);
		}
	}

}
