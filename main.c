#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include "lib.h"

double calculateTime(clock_t start, clock_t end){
    return (double)(end - start)/(double)(sysconf(_SC_CLK_TCK));
}

int main(int argNum, char* args[]){
    struct Blocks** mainArray;
    int size = 0;

    struct tms** programTime = calloc(2,sizeof(struct tms*));
    clock_t realTimeProgram[2];
    for(int j=0; j<2; j++)
        programTime[j] = (struct tms*) calloc(1,sizeof(struct tms*));
    realTimeProgram[0] = times(programTime[0]);
    
    struct tms** tmsTime = calloc(2,sizeof(struct tms*));
    clock_t realTime[2];
    for(int j=0; j<2; j++)
        tmsTime[j] = (struct tms*) calloc(1,sizeof(struct tms*));

    for(int i=1; i < argNum; i++){
        
        char* argument = args[i];
        printf("\n");
        printf(argument);

        realTime[0] = times(tmsTime[0]);

        if(strcmp(argument,"compare_pairs") == 0){
            
            char**  files = (char*) calloc(1000,sizeof(char*));
            while(i + 2 < argNum &&
                  strstr(args[i + 1],".txt") != NULL &&
                  strstr(args[i + 2],".txt") != NULL){
                files[size] = args[i + 1];
                files[size+1] = args[i + 2];
                size += 2;
                i += 2;
            }
            mainArray = compareFiles(files,size);
            printf("\n%d\n",size);
        }
        else if(strcmp(argument,"remove_block") == 0){
            int blockIndex = atoi(args[i+1]);
            deleteBlock(blockIndex,mainArray,size);
            i++;
        }
        else if(strcmp(argument,"remove_operation") == 0){
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

        realTime[1] = times(tmsTime[1]);
        printf("[REAL TIME] Executing action %s took %fs\n",argument,calculateTime(realTime[0],realTime[1]));
        printf("[USER TIME] Executing action %s took %fs\n",argument,calculateTime(tmsTime[0]->tms_utime,tmsTime[1]->tms_utime));
        printf("[SYSTEM TIME] Executing action %s took %fs\n",argument,calculateTime(tmsTime[0]->tms_stime,tmsTime[1]->tms_stime));
    }

    realTime[1] = times(programTime[1]);
    printf("[REAL TIME] Executing main.c took %fs\n",calculateTime(realTimeProgram[0],realTime[1]));
    printf("[USER TIME] Executing main.c took %fs\n",calculateTime(programTime[0]->tms_utime,programTime[1]->tms_utime));
    printf("[SYSTEM TIME] Executing main.c took %fs\n",calculateTime(programTime[0]->tms_stime,programTime[1]->tms_stime));

    return 0;
}