#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

void hello(){
    printf("Hello world!");
}

struct Block** compareFiles(char* files[],int size){
    if(size % 2 != 0){
        printf("Even number of files needed");
        return NULL;
    }

    struct Block** mainArray = (struct Block**) calloc(size/2 , sizeof(struct Block *));
    int index = 0;

    for(int i=0; i< size -1 ; i+=2){
        compareTwoFiles(files[i],files[i+1]);
        mainArray[index++] = createBlock("tempFile");
    }

    return mainArray;
}

void compareTwoFiles(char* file1, char* file2){
    char tempFile [strlen(file1) + strlen(file2) + 50];
    strcpy(tempFile,"diff ");
    strcat(tempFile,file1);
    strcat(tempFile," ");
    strcat(tempFile,file2);
    strcat(tempFile," >>tempFile");
    system(tempFile);
}

struct Block* createBlock(char operationsFile[]){
    char** operationsArray = (char**) calloc(1000,sizeof(char *));
    
    FILE * file;
    char* line = NULL;
    size_t len = 0;
    int i = 0;

    file = fopen(operationsFile,"r");
    if(file == NULL){
        printf("Failed to open file");
        exit(EXIT_FAILURE);
    }
    operationsArray[i] = (char*) calloc(1000000,sizeof(char));
    strcpy(operationsArray[i],"");

    while(getline(&line,&len,file) != -1){
        if(strcmp(line,"---\n") == 0){
            i++;
            operationsArray[i] = (char*) calloc(100000,sizeof(char));
            strcpy(operationsArray[i],"");
        }
        else{
            strcat(operationsArray[i],line);
        }
    }

    fclose(file);
    free(line);

    struct Block* block = (struct Block*) calloc(1,sizeof(struct Block *));
    block -> operationNumber = i + 1;
    block -> operations = operationsArray;

    system("rm tempFile");

    return block;
}

int getNumberOfOperations(int index, struct Block** blocks){
    return blocks[index] -> operationNumber;
}

void deleteBlock(int index, struct Block** blocks, int size){
    if(index >= size)
        printf("Cannot delete, index out of range!");
    else
    {
        free(blocks[index]);
        blocks[index] = NULL;
    }
}

void deleteOperation(int index, struct Block* block){
    if(index >= block->operationNumber)
        printf("Cannot delete, index out of range!");
    else
    {
        free(block->operations[index]);
        block->operations[index] = NULL;
        block->operationNumber--;
    }
}