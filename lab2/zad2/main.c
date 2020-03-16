#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <memory.h>
#include <ctype.h>
#include <dirent.h>
#include <ftw.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <pwd.h>

char* file_type(int st_mode){
    if(S_ISDIR(st_mode) != 0)
        return "dir";
    if(S_ISCHR(st_mode) != 0)
        return "char dev";
    if(S_ISBLK(st_mode) != 0)
        return "block dev";
    if(S_ISFIFO(st_mode) != 0)
        return "fifo";
    if(S_ISLNK(st_mode) != 0)
        return "slink";
    if(S_ISREG(st_mode) != 0)
        return "file";
    return "error";
}

int compare_dates(struct stat* stats, char* option, char* arg){
    time_t file_time;
    time_t real_time = time(NULL);
   
    if(strcmp(option, "-mtime") == 0){
        file_time = stats->st_mtime;
    }

    else if(strcmp(option, "-atime") == 0){
        file_time = stats->st_atime;
    }

    else
        perror("Invalid argument");

    int diff =  (int)(difftime(real_time, file_time)/86400);
    int days = abs((int)strtol(arg,NULL,10));
    char sign =  arg[0];
    
    //printf("%d",days);
    // printf("%ld\n",real_time);
    // printf("%ld\n",file_time);
    // printf("%d\n",diff);

    if (sign == '+'){
        return diff > days;
    }
    else if (sign == '-'){
        return diff < days;
    }   
    else{
        return diff == days;
    }
}

void _stat(char* dir, char* option, char* argument, int depth)
{
    if(depth == 0)
        return;

    DIR* directory = opendir(dir);
    if(!directory)
        perror("Unable to open directory");

    struct dirent *file;
    struct tm* modTime;
    struct tm* accTime;
    char* absPath = (char*) calloc(1000,sizeof(char));
    char* nextPath = (char*) calloc(1000,sizeof(char));
    struct stat stats;


    while((file = readdir(directory))){
        strcpy(nextPath, dir);
        strcat(nextPath, "/");
        strcat(nextPath, file->d_name);
        realpath(nextPath, absPath);

        if(stat(nextPath, &stats) == -1){
            perror("Stat failed");
            continue;
        }

        if(strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0){
            
            if(compare_dates(&stats, option, argument)){
                printf("\nPath: %s\n"
                        "Type: %s\n"
                        "Size: %ld\n",
                        absPath,
                        file_type(stats.st_mode),
                        stats.st_size);
                printf("Last mod: %s",ctime(&stats.st_mtime));
                printf("Last acc: %s",ctime(&stats.st_atime));
            }

            if(file->d_type == 4)
                _stat(nextPath,option, argument, depth - 1);

        }
    }

    free(nextPath);
    free(absPath);
    free(file);
    closedir(directory);
}

int main(int argc, char* args[]){
    if(argc < 2){
        perror("Not enough arguments");
    }

    char* path = args[1];
    char* time_option;
    char* time_arg;
    int maxdepth;

    for(int i=2; i<argc; i++){
        if(strcmp(args[i],"-mtime") == 0 || strcmp(args[i],"-atime") == 0){
            if(i + 1 < argc){
                time_option = args[i];
                time_arg = args[i+1];
                i++;
            }
            else
                perror("Not enough arguments");
        }
       else if(strcmp(args[i],"-maxdepth") == 0){
                if(i + 1 < argc){
                    maxdepth = atoi(args[i+1]);
                    i++;
                }
            else
                perror("Not enough arguments");
       }
    }

    _stat(path, time_option, time_arg, maxdepth);


    return 0;
}

