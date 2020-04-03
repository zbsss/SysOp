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

int main(int argc, char** args){

    int i=0;

    for(i=1; i<argc -1; i++){
        int pd[2];
        pipe(pd);

        if(fork()==0){
            dup2(pd[1],STDOUT_FILENO); //To co mialo isc na output dziecka idzie do pipe
            execlp(args[i],args[i],NULL);
            perror("exec error1");
            abort();
        }

        dup2(pd[0], STDIN_FILENO); // To co jest w pipe idzie na input rodzica
        close(pd[1]);
    }

    execlp(args[i],args[i],NULL);
    perror(args[i]);
    abort();
}