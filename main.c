#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib.h"

int main(){
    int size = 2;
    char* a = "a.txt";
    char* b = "b.txt";
    
    char** files = (char* ) calloc(size,sizeof(char*));
    
    files[0] = a;
    files[1] = b;

    struct Block** mainArray = compareFiles(files,2);
    printf("%d",getNumberOfOperations(0,mainArray));
    deleteOperation(0,mainArray[0]);
    printf("%d",getNumberOfOperations(0,mainArray));

    return 0;
}