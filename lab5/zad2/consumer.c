#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *args[])
{
    if (argc != 4)
    {
        perror("wrong number of arguments");
        exit(1);
    }
    char *pipe_path = args[1];
    char *file_path = args[2];
    int N = atoi(args[3]);

    FILE *file, *pipe; ;

    if ((file = fopen(file_path, "w")) == NULL)
    {
        perror("connot open file");
        exit(1);
    }

    if ((pipe = fopen(pipe_path, "r")) == NULL)
    {
        perror("connot open pipe");
        exit(1);
    }
    
    char buff[N];

    while (fgets(buff, N, pipe) != NULL)
    {
        fprintf(file, buff, strlen(buff));
    }

    fclose(pipe);
    fclose(file);
    return 0;
}