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

int error(char* msg){
    perror(msg);
    return 1;
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

void qsort_sys(int fd, int records, int bytes, int low, int high){
    if(low < high){
        int q = partition_sys(fd, records, bytes, low, high);
        qsort_sys(fd, records, bytes, low, q - 1);
        qsort_sys(fd, records, bytes, q + 1, high);
    }
}

void sort_sys(char* file, int records, int bytes){
    int fd = open(file,O_RDWR);
    if(fd < 0)
        error("Unable to open file");

    qsort_sys(fd, records, bytes, 0, records - 1);
    
    close(fd);
}

int partition_lib(FILE * file, int records, int bytes, int low, int high){
    char* x = read_lib(file, high, bytes);
    int i = low - 1;

    for(int j=low; j<high; j++){
        char* y = read_lib(file, j, bytes);

        if(y[0] <= x[0]){
            i++;
            write_lib(file, read_lib(file, i, bytes), j, bytes);
            write_lib(file, y, i, bytes);
        }
    }
    write_lib(file, read_lib(file, i + 1, bytes), high, bytes);
    write_lib(file, x, i + 1, bytes);
    return i + 1;
}

void qsort_lib(FILE* file, int records, int bytes, int low, int high){
    if(low < high){
        int q = partition_lib(file, records, bytes, low, high);
        qsort_lib(file, records, bytes, low, q - 1);
        qsort_lib(file, records, bytes, q + 1, high);
    }
}

void sort_lib(char* fileName, int records, int bytes){
    FILE* file = fopen(fileName,"r+");
    if(!file)
        error("Unable to open file");

    qsort_lib(file, records, bytes, 0, records - 1);

    fclose(file);
}

void copy_sys(char* file1Name, char* file2Name, int records, int bytes){
    int fd1 = open(file1Name,O_RDONLY);
    if(fd1 < 0)
        error("Unable to open file1 while copying (sys)");
    
    int fd2 = open(file2Name,O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR);
    if(fd2 < 0)
        error("Unable to open file2 while copying (sys)");

    char *buffer = calloc(bytes+1,sizeof(char));
    for(int i=0; i<records; i++){
        read(fd1, buffer, bytes + 1);
        write(fd2, buffer, bytes + 1);
    }

    close(fd1);
    close(fd2);
}

void copy_lib(char* file1Name, char* file2Name, int records, int bytes){
    FILE* file1 = fopen(file1Name,"r");
    if(!file1)
        error("Unable to open file1 while copying (lib)");
    
    FILE* file2 = fopen(file2Name,"w");
    if(!file2)
        error("Unable to open file2 while copying (lib)");

    char *buffer = calloc(bytes+1, sizeof(char));
    for(int i=0; i<records; i++){
        fread(buffer,sizeof(char), bytes+1, file1);
        fwrite(buffer, sizeof(char), bytes+1, file2);
    }

    fclose(file1);
    fclose(file2);
}

int main(int argc, char* args[]){
    
    struct tms** tmsTime = calloc(2,sizeof(struct tms*));
    clock_t realTime[2];
    
    for(int j=0; j<2; j++)
        tmsTime[j] = (struct tms*) calloc(1,sizeof(struct tms*));


    for(int i = 1; i < argc; i++){
        
        realTime[0] = times(tmsTime[0]);

        char*  command = args[i];
        char* type = "_";

        if(strcmp(command, "generate") == 0){
            if( i + 3 < argc){
                generate(args[i+1], args[i+2], args[i+3]);
                i += 3;
            }
            else
                error("Not enough arguments for generating a file");
        }
        else if(strcmp(command, "sort") == 0){
            if(i + 4 < argc){
                char* arg = args[i+1];

                if(strcmp(arg, "sys")==0){
                    // fileName = args[i+2], records = args[i+3], bytes =args[i+4]
                    sort_sys(args[i+2],atoi(args[i+3]),atoi(args[i+4]));
                    type = "sys";
                }
                else if(strcmp(arg, "lib")==0){
                    // fileName = args[i+2], records = args[i+3], bytes =args[i+4]
                    sort_sys(args[i+2],atoi(args[i+3]),atoi(args[i+4]));
                    type = "lib";
                }
                i += 4;
            }
            else{
                error("Not enough arguments to sort");
            }
        }
        else if(strcmp(command, "copy") == 0){
            if(i + 5 < argc){
                char * argument = args[i+1];
                if(strcmp(argument, "sys")==0){
                    copy_sys(args[i+2],args[i+3],atoi(args[i+4]),atoi(args[i+5]));
                    type = "sys";
                }
                else if( strcmp(argument, "lib")==0){
                    copy_lib(args[i+2],args[i+3],atoi(args[i+4]),atoi(args[i+5]));
                    type = "lib";
                }

                i += 5;
            }
            else
                error("Not enough arguments to copy");
        }

        realTime[1] = times(tmsTime[1]);
        printf("[REAL TIME] Executing action %s %s took %fs\n",command, type, calculate_time(realTime[0],realTime[1]));
        printf("[USER TIME] Executing action %s  %s took %fs\n",command, type, calculate_time(tmsTime[0]->tms_utime,tmsTime[1]->tms_utime));
        printf("[SYSTEM TIME] Executing action %s %s took %fs\n\n",command, type, calculate_time(tmsTime[0]->tms_stime,tmsTime[1]->tms_stime));
    }

    return 0;
}
