#include <stdlib.h>
#include <stdio.h>

struct Node{
    int data;
    struct Node* left;
    struct Node* right;
};
typedef struct Node Node;

void search(int target, Node* root, int height);
Node* insert(int data, Node* root, int height);
void free_tree(Node* root);


int main(int argc, char** argv){

    FILE* f = fopen(argv[1], "r");
	if(f == NULL){
		printf("error");
		exit(0);
	}

    Node* root = NULL;

    char cmd;
	int data;
	while(fscanf(f, "%c	%d\n", &cmd, &data) != EOF){
        if(cmd == 's'){
            search(data, root, 1);
        }else if(cmd == 'i'){
            root = insert(data, root, 1);
        }
    }

    fclose(f);
    free_tree(root);

    return 0;
}


void search(int target, Node* root, int height){
    if(root == NULL){
        printf("absent\n");
        return;
    }else if(root->data == target){
        printf("present %d\n", height);
    }else if(root->data > target){
        search(target, root->left, height+1);
    }else{
        search(target, root->right, height+1);
    }
}


Node* insert(int data, Node* root, int height){
    if(root == NULL){
        root = malloc(sizeof(Node));
        root->data = data;
        printf("inserted %d\n", height);
        return root;
    }else if(root->data == data){
        printf("duplicate\n");
        return root;
    }else if(root->data > data){
        if(root->left == NULL){
            Node* temp = malloc(sizeof(Node));
            temp->data = data;
            root->left = temp;
            printf("inserted %d\n", height+1);
            return root;
        }
        root->left = insert(data, root->left, height+1);
    }else{
        if(root->right == NULL){
            Node* temp = malloc(sizeof(Node));
            temp->data = data;
            root->right = temp;
            printf("inserted %d\n", height+1);
            return root;
        }
        root->right = insert(data, root->right, height+1);
    }
    return root;
}


void free_tree(Node* root){
    if(root->left != NULL){
        free_tree(root->left);
    }
    if(root->right != NULL){
        free_tree(root->right);
    }
    free(root);
}
