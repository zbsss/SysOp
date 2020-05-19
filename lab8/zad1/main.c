#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <math.h>


int W, H, threads;
int **image;
int **histogram; 

void error(char* msg){
    fprintf(stderr, msg);
    exit(1);
}

void load_image(char* filename){
    FILE* file = fopen(filename, "r");
    
    if(file == NULL){
        error("Cannot open input image");
    }

    int shades;
    int read = fscanf(file, "P2\n%d %d\n%d\n", &W, &H, &shades);
    if(shades > 255){
        error("Wrong type of image, to many shades");
    }

    if(read != 3){
        error("Error while reading image header");
    }


    image = calloc(H, sizeof(int*));
    for(int i=0; i<H; i++){
        image[i] = calloc(W, sizeof(int));
    }

    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            fscanf(file, "%d", &image[i][j]);
        }
    }

    fclose(file);
}


void* sign(int indx){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    int first_color = ceil(256/threads) * indx; //Przedzial domkniety
    int last_color = (indx+1) * (256/threads); //Przedzial otwarty

    for(int i=0; i<W; i++){
        for(int j=0; j<H; j++){
            if(first_color <= image[j][i] && image[j][i] < last_color)
                histogram[indx][image[j][i]]++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    int time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    return (void *) time;
}

void* block(int indx){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    int first = (indx) * ceil(W/threads); //Przedzial domkniety
    int last = (indx+1) * (W/threads); //Przedzial otwarty

    for(int i=first; i<last; i++){
        for(int j=0; j<H; j++){
            histogram[indx][image[j][i]]++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    int time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    return (void *) time;
}

void* interleaved(int indx){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i=indx; i<W; i+=threads){
        for(int j=0; j<H; j++){
            histogram[indx][image[j][i]]++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    int time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    return (void *) time;
}

void save_histogram(char* out){
    
    FILE* file = fopen(out, "w");
    
    if(!file){
        error("Cannot open output file");
    }

    int sum[256];

    for(int i=0; i<256; i++){
        sum[i] = 0;
        for(int j=0; j<threads; j++){
            sum[i] += histogram[j][i];
        }
    }

    for(int i=0; i<256; i++){
        fprintf(file, "%d: %d\n", i, sum[i]);
    }
    
    fclose(file);
}

int main(int argc, char** argv){
    if(argc != 5){
        error("Error, wrong number of arguments");
    }

    threads = atoi(argv[1]);
    char* mode = argv[2];
    char* input_file = argv[3];
    char* output_file = argv[4];

    if(threads < 1)
        error("Number of threads must be at least 1");

    load_image(input_file);

    FILE* Times = fopen("Times.txt", "a");
    if( Times == NULL){
        error("Cannot open Times.txt");
    }

    /*
        histogram must be a 2D array because every thread mist have its own histogram.
        This is because they are asynchonous and, if they shared one histogram they might interrupt each other 
        which could result in a wrong result.
    */
    histogram = calloc(threads, sizeof(int*));
    for(int i=0; i<threads; i++){
        histogram[i] = calloc(256, sizeof(int));
        for(int j=0; j<256; j++){
            histogram[i][j] = 0;
        }   
    }


    pthread_t *thread_ids = calloc(threads, sizeof(pthread_t));

    printf("\nMode: %s    Threads: %d\n", mode, threads);
    fprintf(Times, "\nMode: %s    Threads: %d\n", mode, threads);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    if(strcmp(mode, "sign") == 0){
        if(256 % threads != 0)
            error("Number of threads is not a divisor of 256");

        for(int i=0; i<threads; i++){
            pthread_create(&thread_ids[i], NULL, sign, i);
            int time;
            pthread_join(thread_ids[i],(void*) &time);
            printf("Thread %ld:    %d ms\n",thread_ids[i], time);
            fprintf(Times, "Thread %ld:    %d ms\n",thread_ids[i], time);
        }
    }
    else if(strcmp(mode, "block") == 0){
        for(int i=0; i<threads; i++){
            pthread_create(&thread_ids[i], NULL, block, i);
            int time;
            pthread_join(thread_ids[i], (void*)&time);
            printf("Thread %ld:    %d ms\n",thread_ids[i], time);
            fprintf(Times, "Thread %ld:    %d ms\n",thread_ids[i], time);
        }
    }
    else if(strcmp(mode, "interleaved") == 0){
        for(int i=0; i<threads; i++){
            pthread_create(&thread_ids[i], NULL, interleaved, i);
            int time;
            pthread_join(thread_ids[i], (void*)&time);
            printf("Thread %ld:    %d ms\n",thread_ids[i], time);
            fprintf(Times, "Thread %ld:    %d ms\n",thread_ids[i], time);
        }
    }
    else{
        error("No such type. Types: sign, block, interleaved");
    }

    clock_gettime(CLOCK_REALTIME, &end);
    int time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("Total time: %d ms\n", time);
    fprintf(Times, "Total time: %d ms\n", time);


    save_histogram(output_file);


    fclose(Times);

    for(int i=0; i<threads; i++){
        free(histogram[i]);
    }
    free(histogram);

    for(int i=0; i<H; i++){
        free(image[i]);
    }
    free(image);

    return 0;
}