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

typedef struct command{
    char** args; //array for a command and it's arguments
    int size;
}command;

int main(int argc, char** args){

    int i;

    command* commands = calloc(MAX_PIPED,sizeof(command));
    int command_num = 0;

    for(int j=1; j<argc; j++){
        commands[command_num].args = calloc(MAX_ARGUMENTS+1,sizeof(char*));
        commands[command_num].size = 0;
        while(j<argc && strcmp(args[j],"|") != 0){
            commands[command_num].args[commands[command_num].size++] = args[j];
            j++;
        }
        commands[command_num].args[commands[command_num].size++] = NULL;
        command_num++;
    }

    for(i=0; i<command_num -1; i++){
        int pd[2];
        pipe(pd);


        if(fork()==0){
            dup2(pd[1],STDOUT_FILENO); //To co mialo isc na output dziecka idzie do pipe
            execvp(commands[i].args[0],commands[i].args);
            perror("exec error1");
            abort();
        }

        dup2(pd[0], STDIN_FILENO); // To co jest w pipe idzie na input rodzica
        close(pd[1]);
    }

    execvp(commands[i].args[0],commands[i].args);
    perror("exec errpr2");
    abort();
}