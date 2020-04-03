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

#define MAX_ARGUMENTS 2
#define MAX_PIPED 3

int main(int argc, char** args){

    if(argc != 2)
        perror("Wrong number of arguments");

    FILE* file = fopen(args[1],"r");
    if(file == NULL)
        perror("cannot open file");

    ssize_t readl;
    char* line;
    size_t len = 0;

    while((readl = getline(&line, &len, file) != -1)){
        char* token = strtok(line,"|");
        char* buff[MAX_PIPED];
        int j = 0;
        while(token != NULL){
            buff[j++] = token;
            token = strtok(NULL,"|");
        }

        //Przepisuje tablice zeby dac ja do exec*
        char* arguments[j+2];
        arguments[0] = "./test";
        arguments[j+1] = NULL;
        for(int k = 0; k < j ; k++){
            arguments[k + 1] = buff[k];
        }

        //test
        for(int k = 0; k < j+2 ; k++){
            printf(arguments[k]);
        }

        int pd[2];
        pipe(pd);

        if (fork() == 0){
            close(pd[0]);
            dup2(pd[1],STDOUT_FILENO); //To co mialo isc na output dziecka idzie do pipe
            execv("./test",arguments);
            perror("exec error3");
            abort();
        }

        close(pd[1]);
        char line[1000];
        int n = read( pd[0], line, 1000);
        write(STDOUT_FILENO, line, n);

    }
    return 0;
}