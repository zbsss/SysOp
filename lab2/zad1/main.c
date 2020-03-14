#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

double calculate_time(clock_t start, clock_t end){
    return (double) (end - start) / (double) sysconf(_SC_CLK_TCK);
}

void generate(char* name, char* records, char* bytes){
    int size = 100;
    char* command = calloc(size, sizeof(char));
    for(int i = 0; i < size; ++i){
        command[i] = 0;
    }
    strcpy(command, "head -c 1000000000 /dev/urandom | tr -dc 'a-z'");
    strcat(command, " | fold -w ");
    strcat(command, bytes);
    strcat(command, " | head -n ");
    strcat(command, records);
    strcat(command, " > ");
    strcat(command, name);
    
    system(command);
    free(command);
}

int main(int argc, char* args[]){
    
    for(int i = 1; i < argc; i++){
        
        char*  command = args[i];

        if(strcmp(command, "generate") == 0){
            if( i + 3 < argc){
                generate(args[i+1], args[i+2], args[i+3]);
                i += 3;
            }
            else
                printf("Not enough arguments for generating a file");
        }
    }
    return 0;
}
