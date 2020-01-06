#include <stdlib.h>
#include <stdio.h>

struct Node{
	int data;
	struct Node* next;
};
typedef struct Node Node;

void insert(int data);
void delete(int target);
int list_length();
void print_list();
void free_list();
Node* head = NULL;


int main(int argc, char** argv){

	/* Open file */
	FILE* f = fopen(argv[1], "r");
	if(f == NULL){
		printf("error");
		exit(0);
	}

	char cmd;
	int data;
	while(fscanf(f, "%c	%d\n", &cmd, &data) != EOF){
		if(cmd == 'i'){
			insert(data);
		}else if(cmd == 'd'){
			delete(data);
		}
		//print_list();
	}
	int l = list_length();
	printf("%d\n", l);
	print_list();
	free_list();
	fclose(f);
	return 0;

}


void insert(int data){
	if(head == NULL){
		head = malloc(sizeof(Node));
		head->data = data;
		return;
	}

	Node* temp = malloc(sizeof(Node));
	temp->data = data;

	Node* ptr = head;
	Node* prev = NULL;
	while(ptr != NULL){
		if(ptr->data == data){
			return;
		}else if(ptr->data > data){
			if(ptr == head){
				temp->next = head;
				head = temp;
			}else{
				temp->next = ptr;
				prev->next = temp;
			}
			return;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	prev->next = temp;
}


void delete(int target){

	if(head == NULL){
		return;
	}

	Node* ptr = head;
	Node* prev = NULL;
	while(ptr != NULL && ptr->data <= target){
		if(ptr->data == target){
			if(ptr == head){
				head = ptr->next;
			}else{
				prev->next = ptr->next;
			}
			free(ptr);
			return;
		}
		prev = ptr;
		ptr = ptr->next;
	}

}


int list_length(){

	int length = 0;
	for(Node* ptr = head; ptr != NULL; ptr = ptr->next){
		length++;
	}
	return length;

}


void print_list(){
	Node* ptr = head;
	while(ptr != NULL){
		if(ptr->next == NULL){
			printf("%d", ptr->data);
		}else{
			printf("%d	", ptr->data);
		}
		ptr = ptr->next;
	}
	printf("\n");
}


void free_list(){

	Node* ptr = head;
	while(ptr != NULL){
		Node* prev = ptr;
		ptr = ptr->next;
		free(prev);
	}

}
