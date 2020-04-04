#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#define MAX_ARGUMENTS 5
#define MAX_PIPED 5


int main(int argc, char** args){

    int i;

    int command_num = 0;

    FILE *fpin;

    char** commands = calloc(MAX_PIPED,sizeof(char*));

    for(int j=1; j<argc; j++){
        commands[command_num] = calloc(100,sizeof(char));
        strcpy(commands[command_num], args[j++]);
        while(j<argc && strcmp(args[j],"|") != 0){
            strcat(commands[command_num], " ");
            strcat(commands[command_num], args[j]);
            j++;
        }
        command_num++;
    }


    for(i=0; i<command_num; i++){
        
        fpin = popen(commands[i],"r");

       
        char output[1000];
        if(fgets(output, 1000, fpin) != NULL)
            fputs(output,stdin);

    }

    
    char output[1000];
        while(fgets(output, 1000, fpin) != NULL)
            if(fputs(output,stdout) == EOF)
                perror("fputs puts error to piep3");
    return 0;
}