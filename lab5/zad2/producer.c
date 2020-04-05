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

    srand(time(NULL));

    FILE *file;
    int pipe;

    if ((file = fopen(file_path, "r")) == NULL)
    {
        perror("connot open file");
        exit(1);
    }

    if ((pipe = open(pipe_path, O_WRONLY))< 0)
    {
        perror("connot open pipe");
        exit(1);
    }

    char resource[N];

    while (fgets(resource, N, file) != NULL)
    {
        char message[N + 20];
        sprintf(message, "#%d#%s\n", getpid(), resource);
        write(pipe, message, strlen(message));
        sleep(rand() % 2 + 1);
    }

    close(pipe);
    fclose(file);
    return 0;
}