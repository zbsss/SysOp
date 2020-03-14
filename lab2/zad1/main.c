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

char * read_sys(int fd, int index, int bytes){
    char* record = calloc(bytes , sizeof(char));
    lseek(fd, (bytes + 1)*index, 0); //bytes + 1 cuz of \n
    read(fd, record, bytes);
    return record;
}

char * read_lib(FILE * file, int index, int bytes){
    char* record = calloc(bytes , sizeof(char));
    fseek(file, (bytes + 1)*index, 0); //bytes + 1 cuz of \n
    fread(record, sizeof(char), bytes, file);
    return record;
}

void write_sys(int fd, char *block, int index, int bytes){
    lseek(fd, (bytes+1)*index, 0);
    write(fd, block, bytes);
}

void write_lib(FILE * file, char *block, int index, int bytes){
    fseek(file, (bytes+1)*index, 0);
    fwrite(block,sizeof(char), bytes, file);
}

void sort_sys(char* file, int records, int bytes){
    int fd = open(file,O_RDWR);
    if(fd < 0)
        error("Unable to open file");

    qsort_sys(fd, records, bytes, 0, records - 1);
}

void qsort_sys(int fd, int records, int bytes, int low, int high){
    if(low < high){
        int q = partition_sys(fd, records, bytes, low, high);
        qsort_sys(fd, records, bytes, low, q - 1);
        qsort_sys(fd, records, bytes, q + 1, high);
    }
}

int partition_sys(int fd, int records, int bytes, int low, int high){
    char* x = read_sys(fd, high, bytes);
    int i = low - 1;

    for(int j=low; j<high; j++){
        char* y = read_sys(fd, j, bytes);

        if(y[0] <= x[0]){
            i++;
            write_sys(fd, read_sys(fd, i, bytes), j, bytes);
            write_sys(fd, y, i, bytes);
        }
    }
    write_sys(fd, read_sys(fd, i + 1, bytes), high, bytes);
    write_sys(fd, x, i + 1, bytes);
    return i + 1;
}

void sort_lib(char* fileName, int records, int bytes){
    FILE* file = fopen(fileName,'r+');
    if(!file)
        error("Unable to open file");

    qsort_lib(file, records, bytes, 0, records - 1);
}

void qsort_lib(FILE* file, int records, int bytes, int low, int high){
    if(low < high){
        int q = partition_lib(file, records, bytes, low, high);
        qsort_lib(file, records, bytes, low, q - 1);
        qsort_lib(file, records, bytes, q + 1, high);
    }
}

int partition_lib(FILE * file, int records, int bytes, int low, int high){
    char* x = read_lib(file, high, bytes);
    int i = low - 1;

    for(int j=low; j<high; j++){
        char* y = read_lib(file, j, bytes);

        if(y[0] <= x[0]){
            i++;
            write_lib(file, read_sys(file, i, bytes), j, bytes);
            write_lib(file, y, i, bytes);
        }
    }
    write_lib(file, read_sys(file, i + 1, bytes), high, bytes);
    write_lib(file, x, i + 1, bytes);
    return i + 1;
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
        else if(strcmp(command, "sort") == 0){
            char* arg = args[i+1];
            if(strcmp(arg, "sys")==0){
                // fileName = args[i+2], records = args[i+3], bytes =args[i+4]
                sort_sys(args[i+2],atoi(args[i+3]),atoi(args[i+4]));
            }
            else if(strcmp(arg, "lib")==0){
                // fileName = args[i+2], records = args[i+3], bytes =args[i+4]
                sort_sys(args[i+2],atoi(args[i+3]),atoi(args[i+4]));
            }
        }
    }

    return 0;
}
