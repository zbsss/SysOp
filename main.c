#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib.h"

int main(){
    
    char* a = "a.txt";
    char* b = "b.txt";
    
    char** files = (char* ) calloc(2,sizeof(char*));
    
    files[0] = a;
    files[1] = b;

    compareFiles(files);

    return 0;
}