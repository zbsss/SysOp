#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

char* file_name(char* path, int i){
    char* newpath = calloc(30,sizeof(char));
    strcpy(newpath, path);
    
    char letter = '`' + i;
    char ending[2] = {letter,'\0'};
    strcat(newpath,ending);

    return newpath;
}

int main(int argc, char *argv[])
{
    char* pipe_path = "fifo";
    
    char* producer = "./producer";
    char* pro_path = "./resource_mine/";

    char* consumer = "./consumer";
    char* cons_path = "./consumed_resources/output";

    char* N_prod = "3";
    char* N_cons = "10"; 

    mkfifo("fifo", S_IRUSR | S_IWUSR);

    pid_t pids[6];
    if ((pids[0] = fork()) == 0)
        execlp(consumer, consumer, pipe_path, cons_path, N_cons, NULL);

    for(int i=1; i<6; i++){
        if ((pids[1] = fork()) == 0){
            char* path = file_name(pro_path, i);
            execlp(producer, producer, pipe_path, path, N_prod, NULL);
        }
    }

    for (int i = 0; i < 6; i++)
        waitpid(pids[i], NULL, 0);

    return 0;
}