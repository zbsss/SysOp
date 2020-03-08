#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "lib.h"

double calculateTime(clock_t start, clock_t end){
    return (double)(end - start)/(double)(sysconf(_SC_CLK_TCK));
}



int main(int argNum, char* args[]){
    struct Blocks** mainArray;
    int size = 0;

    for(int i=1; i < argNum; i++){
        printf(args[i]);
        if(strcmp(args[i],"compare_pairs") == 0){
            
            char**  files = (char*) calloc(100,sizeof(char*));
            while(i + 2 < argNum &&
                  strstr(args[i + 1],".txt") != NULL &&
                  strstr(args[i + 2],".txt") != NULL){
                files[size] = args[i + 1];
                files[size+1] = args[i + 2];
                size += 2;
                i += 2;
            }
            mainArray = compareFiles(files,size);
        }
        else if(strcmp(args[i],"remove_block") == 0){
            int blockIndex = atoi(args[i+1]);
            deleteBlock(blockIndex,mainArray,size);
            i++;
        }
        else if(strcmp(args[i],"remove_operation") == 0){
            int blockIndex = atoi(args[i+1]);
            int operationIndex = atoi(args[i+2]);

            printf("\n%d",getNumberOfOperations(blockIndex,mainArray));
            deleteOperation(operationIndex,mainArray[blockIndex]);
            printf("\n%d\n",getNumberOfOperations(blockIndex,mainArray));
            i+=2;
        }
        else
        {
            i++;
        }
    }

    return 0;
}