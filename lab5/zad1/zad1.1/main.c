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

    int i=1;

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

        //Przepisuje tablice zeby dac ja do exec*
        char* arguments[j+2];
        arguments[0] = "./child";
        arguments[j+1] = NULL;
        for(int k = 0; k < j ; k++){
            arguments[k + 1] = buff[k];
        }

        int pd[2];
        pipe(pd);

        if (fork() == 0){
            close(pd[0]);
            dup2(pd[1],STDOUT_FILENO); //To co mialo isc na output dziecka idzie do pipe
            execv("./child",arguments);
            perror("exec error3");
            abort();
        }


        //rodzic  wczytuje i wypisuje wynik programu ./child z pipe'a
        printf("============== executing line: %d ==============\n",i);

        close(pd[1]);
        char output[100];
        int n; 
        n = read(pd[0], output, 100);
        write(STDOUT_FILENO, output, n);

        i++;
        printf("\n");

    }
    return 0;
}