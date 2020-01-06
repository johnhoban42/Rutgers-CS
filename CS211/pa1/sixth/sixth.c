#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* pig_latin(char* str);


int main(int argc, char** argv){
    for(int i = 1; i < argc; i++){
        char* pl = pig_latin(argv[i]);
        printf("%s ", pl);
        free(pl);
    }
    return 0;
}


char* pig_latin(char* str){
    int l = strlen(str);
    char* new_str;
    int i = 0; // Index of first vowel
    while(i < l && !(tolower(str[i]) == 'a' || tolower(str[i]) == 'e' ||
    tolower(str[i]) == 'i' || tolower(str[i]) == 'o' || tolower(str[i]) == 'u')){
        i++;
    }
    char* stub = malloc(l*sizeof(char));
    strncpy(stub, str, i);

    // First letter is a vowel
    if(i == 0){
        new_str = malloc((l + 4)*sizeof(char));
        strcpy(new_str, str);
        strcat(new_str, "yay");

    // No vowels
    }else if(i == l){
        new_str = malloc((l + 4)*sizeof(char));
        strcpy(new_str, str);
        strcat(new_str, "ay");

    // First vowel is somewhere else in the word
    }else{
        new_str = malloc((l + 3)*sizeof(char));
        for(int j = i; j < l; j++){
            new_str[j-i] = str[j];
        }
        strcat(new_str, stub);
        strcat(new_str, "ay");
    }
    free(stub);

    return new_str;
}
