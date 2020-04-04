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
#include <unistd.h>

#define MAX_ARGUMENTS 5
#define MAX_PIPED 5

int main(int argc, char** args){

    if(argc != 2)
        perror("Wrong number of arguments");

    FILE* file = fopen(args[1],"r");
    if(file == NULL)
        perror("cannot open file");

    ssize_t readl;
    char* line;
    size_t len = 0;

    int i = 1;

    while((readl = getline(&line, &len, file) != -1)){
        
        if(line[strlen(line)-1] == '\n'){
            //remove newline
            line[strlen(line)-1] = '\0';
        }

        char* token = strtok(line," ");
        char* buff[MAX_PIPED * (MAX_ARGUMENTS+1)];
        int j = 0;
        while(token != NULL){

            buff[j++] = token;

            token = strtok(NULL," ");
        }


        char* arguments = calloc(1000,sizeof(char));
        strcpy(arguments, "./child");
        for(int k = 0; k < j ; k++){
            strcat(arguments, " ");
            strcat(arguments, buff[k]);
        }


        FILE *fpout;
        fpout = popen(arguments,"r");

        printf("============== executing line: %d ==============\n",i);

        char output[1000];
        while(fgets(output, 1000, fpout) != NULL)
            if(fputs(output,stdout) == EOF)
                perror("fputs puts error to piep1");
        
        i++;
        printf("\n");
    }
    return 0;
}