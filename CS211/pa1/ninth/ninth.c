#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct Node{
    int data;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};
typedef struct Node Node;

// The "silent" parameters hide output for nested calls
Node* search(int target, Node* root, int height, bool silent);
Node* insert(int data, Node* root, int height);
Node* delete(int target, Node* root, bool silent);
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
            search(data, root, 1, false);
        }else if(cmd == 'i'){
            root = insert(data, root, 1);
        }else if(cmd == 'd'){
            root = delete(data, root, false);
        }
    }

    fclose(f);
    free_tree(root);

    return 0;
}


Node* search(int target, Node* root, int height, bool silent){
    if(root == NULL){
        if(!silent){printf("absent\n");}
        return NULL;
    }else if(root->data == target){
        if(!silent){printf("present %d\n", height);}
        return root;
    }else if(root->data > target){
        return search(target, root->left, height+1, silent);
    }else{
        return search(target, root->right, height+1, silent);
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
            temp->parent = root;
            root->left = temp;
            printf("inserted %d\n", height+1);
            return root;
        }
        root->left = insert(data, root->left, height+1);
    }else{
        if(root->right == NULL){
            Node* temp = malloc(sizeof(Node));
            temp->data = data;
            temp->parent = root;
            root->right = temp;
            printf("inserted %d\n", height+1);
            return root;
        }
        root->right = insert(data, root->right, height+1);
    }
    return root;
}


Node* delete(int target, Node* root, bool silent){
    Node* ptr = search(target, root, 1, true);
    if(ptr == NULL){
        if(!silent){printf("fail\n");}
        return root;
    }

    bool l = ptr->left != NULL;
    bool r = ptr->right != NULL;

    if(l && r){
        // FIND Q
        Node* q = ptr->right;
        while(q->left != NULL){
            q = q->left;
        }
        ptr->data = q->data;

        // DELETE Q
        if(q == q->parent->left){
            if(q->right == NULL){
                q->parent->left = NULL;
            }else{
                q->parent->left = q->right;
            }
        }else{
            if(q->right == NULL){
                q->parent->right = NULL;
            }else{
                q->parent->right = q->right;
            }
        }
        free(q);

    }else if(l && !r){
        if(ptr == root){
            root = ptr->left;
        }else{
            if(ptr == ptr->parent->left){
                ptr->parent->left = ptr->left;
            }else{
                ptr->parent->right = ptr->left;
            }
            ptr->left->parent = ptr->parent;
        }
        free(ptr);

    }else if(!l && r){
        if(ptr == root){
            root = ptr->right;
        }else{
            if(ptr == ptr->parent->left){
                ptr->parent->left = ptr->right;
            }else{
                ptr->parent->right = ptr->right;
            }
            ptr->right->parent = ptr->parent;
        }
        free(ptr);

    }else{
        if(ptr != root){
            if(ptr == ptr->parent->left){
                ptr->parent->left = NULL;
            }else{
                ptr->parent->right = NULL;
            }
            free(ptr);
        }
        else{
            free(root);
            printf("success\n");
            return NULL;
        }
    }

    if(!silent){printf("success\n");}
    return root;
}


void free_tree(Node* root){
    if(root == NULL){
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
