#include <stdio.h>
#include "lib.h"

void hello(){
    printf("Hello world!");
}

char* compareTwoFiles(char* file1, char*file2){
    char tempFile [strlen(file1) + strlen(file2) + 12];
    strcpy(tempFile,"diff ");
    strcat(tempFile,file1);
    strcat(tempFile," ");
    strcat(tempFile,file2);
    strcat(tempFile," >>tempFile");
    system(tempFile);
    return &tempFile;
}
