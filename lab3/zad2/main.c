#define _OPEN_SOURCE 500
#define MAX_COLS 1000
#define MAX_LINE_WIDTH 5000
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <sys/file.h>

struct matrix{
    int rows;
    int cols;
    int** val;
};

int read_from_file(FILE* file, char** path){
    size_t size = 0;
    size_t line = getline(path, &size, file);
    if(line == 0){
        printf("Can't read line\n");
        return 1;
    }
    int len = strlen(*path);
    (*path)[len-1] = '\0';
    return 0;
}

int read_paths(char* lists, char** A, char** B, char** C){
    FILE* file = fopen(lists, "r");
    if(file == NULL)
        return 1;

    if(read_from_file(file, A) != 0)
        return 1;

    if(read_from_file(file, B) != 0)
        return 1;

    if(read_from_file(file, C) != 0)
        return 1;

    if(access(*A, F_OK | R_OK) == -1 || access(*B, F_OK | R_OK) == -1)
        return 1;
    return 0;
}

int get_dims(char* path, int* row, int* col){
    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("Cannot open file\n");
        return 1;
    }

    char* buff = NULL;
    size_t size = 0;

    while(getline(&buff, &size, file) != -1){
        (*row)++;
        if((*row) == 1){
            char t[20];
            int offset = 0;
            while(sscanf(buff+offset, "%s", t) != EOF){
                offset += (strlen(t) + 1) * sizeof(char);
                (*col)++;
            }
        }
    }

    return 0;
}

struct matrix get_matrix(char* path){
    FILE* file = fopen(path,"r");
    
    int rows, cols;
    get_dims(path, &rows, &cols);

    int **values = calloc(rows, sizeof(int*));
    for(int i=0; i<rows; i++){
        values[i] = calloc(cols, sizeof(int));
    }

    int x=0;
    int y=0;
    char line[MAX_LINE_WIDTH];
    while(fgets(line,MAX_LINE_WIDTH, file) != NULL){
        x = 0;
        char* number = strtok(line, " ");
        while(number!=NULL){
            values[y][x++] = atoi(number);
            number = strtok(NULL, " ");
        }
        y++;
    }

    fclose(file);
    struct matrix m;
    m.val = values;
    m.rows = rows;
    m.cols = cols;
    return m;
}

void free_matrix(struct matrix *m){
    for(int i=0; i<m->rows; i++){
        free(m->val[i]);
    }
    free(m->val);
}

int get_task(int task_count){
    FILE *tasks_file = fopen(".tmp/tasks", "r+");
    int fd = fileno(tasks_file);
    flock(fd, LOCK_EX);

    char *tasks = calloc(task_count + 1, sizeof(char));
    fseek(tasks_file, 0, SEEK_SET);
    fread(tasks, 1, task_count, tasks_file);

    char *task_ptr_offset = strchr(tasks, '0');
    int task_index = task_ptr_offset != NULL ? task_ptr_offset - tasks : -1;

    if (task_index >= 0)
    {
        tasks[task_index] = '1';
        fseek(tasks_file, 0, SEEK_SET);
        fwrite(tasks, 1, task_count, tasks_file);
        fflush(tasks_file);
    }

    flock(fd, LOCK_UN);
    fclose(tasks_file);

  return task_index;
}


void dot_column(struct matrix *A, struct matrix *B, int col){
    char* name = calloc(20, sizeof(char));
    sprintf(name, ".tmp/part%04d", col);
    FILE* file = fopen(name, "w+");
    
    for(int i=0; i<A->rows; i++){
        int dot = 0;
        for(int j=0; j<A->cols; j++){
            dot += A->val[i][j] * B->val[j][col];
        }
        fprintf(file, "%d \n", dot);
    }
    fclose(file);
}

int worker_callback(struct matrix *A, struct matrix *B){
    int counter = 0;
    int col_indx;
    while(1){
        col_indx = get_task(B->cols);
        if(col_indx == -1)
            break;

    dot_column(A, B, col_indx);
    counter++;
    }
    return counter;
}

int main(int argc, char **args) {

    if(argc != 5){
        printf("Wrong number of arguments\n");
        exit(1);
    }

    int worker_count = atoi(args[2]);
    pid_t* workers = calloc(worker_count, sizeof(int));

    char* A_path = NULL;
    char* B_path = NULL;
    char* C_path = NULL;

    if(read_paths(args[1], &A_path, &B_path, &C_path) != 0){
        exit(1);
    }

    struct matrix A = get_matrix(A_path);
    struct matrix B = get_matrix(B_path);

    char* mode = args[4];
    int useOneFile = (strcmp(mode, "1") == 0);

    FILE* task_file;

    system("rm -r -f .tmp");
    system("mkdir -p .tmp");


    task_file = fopen(".tmp/tasks", "w+");
    char* tasks = calloc(B.cols + 1, sizeof(char));
    sprintf(tasks, "%0*d", B.cols, 0);
    fwrite(tasks, 1, B.cols, task_file);
    fflush(task_file);
    free(tasks);
    fclose(task_file);
    if(useOneFile){
        useOneFile++;
    }

    for(int i=0; i<worker_count; i++){
        pid_t worker = fork();
        if(worker == 0){
            return worker_callback(&A, &B);
        }
        else{
            workers[i] = worker;
        }
    }

    int status = 0;
    for(int i = 0;i<worker_count;i++){
        waitpid(workers[i], &status, 0);
        printf("Process with PID %d performed %d multiplications\n", workers[i], B.cols / worker_count);
    }

    printf("Main Process ends\n");

    free(workers);
    free_matrix(&A);
    free_matrix(&B);
    
    fclose(task_file);
    char buff[256];
    sprintf(buff, "paste .tmp/part* > %s", C_path);
    system(buff);

    return 0;
}