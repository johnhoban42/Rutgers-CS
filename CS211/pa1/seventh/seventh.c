#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char last_char(char* str);


int main(int argc, char** argv){

    for(int i = 1; i < argc; i++){
        printf("%c", last_char(argv[i]));
    }

}


char last_char(char* str){
    return str[strlen(str)-1];
}
