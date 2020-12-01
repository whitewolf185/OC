#include "time.h"
#include "stdio.h"
#include "vector.h"
#include "matfunc.h"
#include "stdlib.h"
#include <pthread.h>
#include "merge.h"

struct Matrix in;
struct Matrix out;

typedef struct tr_data {
    int x,y;
    int winX, winY;

}tr_data;

#define RAND_MAX 10
//#define DEBUG
//#define MERGE

int k,n,m;

int max(int lhs, int rhs){
    if(lhs > rhs){
        return lhs;
    }
    return rhs;
}

int min(int lhs, int rhs){
    if(lhs < rhs){
        return lhs;
    }
    return rhs;
}

void* threadFunc(void* Tdata){
    //printf("im alive\n");
    fflush(stdout);
    struct tr_data* id = (struct tr_data*) Tdata;
    vector pixels;
    create(&pixels,0);

    int cirI, curJ;
    for (int i = max(0, id->x - id->winX); i < min(n, id->x + id->winX); ++i) {
        for (int j = max(0, id->y - id->winY); j < min(m, id->y + id->winY); ++j) {
            push_back(&pixels,in.matr[i][j]);
            merge(&pixels,0,pixels.size-1);
        }
    }

    if(m>1 && n > 1){
        out.matr[id->x][id->y] = pixels.data[pixels.size/2];
        destroy(&pixels);
        return NULL;
    }
    else {
        out.matr[id->x][id->y] = in.matr[id->x][id->y];
        return NULL;
    }

}



int main(int argc, char const *argv[]){
    unsigned int max_threads = 1;
    if(argc > 1 && atoi(argv[1]) > 0){
        max_threads = atoi(argv[1]);
    }

    int winX, winY;

    printf("Threads %d\nEnter size of matrix. Enter N and M\n> ",max_threads);
    fflush(stdout);
    scanf("%d %d", &n, &m);

    if(n == 0 || m == 0){
        printf("I cannot create matrix with this n and m\n");
        return 0;
    }

    scanf_With_Fill_Rand_Matrix(&in, n, m);
    //printMatrix(&in);

    printf("Enter your window\n> ");
    fflush(stdout);
    scanf("%d %d", &winX, &winY);

    printf("Enter your K\n> ");
    fflush(stdout);
    scanf("%d", &k);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t)*max_threads);

    double start,end;

    struct tr_data* args = (struct tr_data*) malloc(sizeof(struct tr_data)*n*m);
    start = clock();
    args->winX = winX;
    args->winY = winY;
    for (int h = 0; h < k; ++h) {
        scanf_Matrix_with_Fill_0(&out,n,m);
#ifdef DEBUG
        printf("Im here with K=%d\n",k);
        fflush(stdout);
#endif //DEBUG
        int count = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                args[count].x = i;
                args[count].y = j;
                ++count;
            }
        }

        for (unsigned int i = 0; i < n * m; i+=max_threads) {
            for (unsigned int j = 0; j < max_threads; ++j) {
                //чтобы не запускался под конец новый процесс
                if(i+j >= n*m){
                    break;
                }
                if(pthread_create(&threads[j],NULL, threadFunc,(void*) &args[i+j]) != 0){
                    perror("Cannot create thread\n");
                    return 1;
                }
            }

            //ждем завершения потоков
            for (int j = 0; j < max_threads; ++j) {
                if(i+j >= n*m){
                    break;
                }
                if(pthread_join(threads[j], NULL)){
                    perror("Thread didn't finished\n");
                    return 1;
                }
            }

        }
#ifdef DEBUG
        printf("am I done?\n");
        fflush(stdout);
#endif //DEBUG
        swap(&in, &out);
    }
    end = clock();

    free(threads);
    free(args);

    printf("Result matrix:\n");
    //printMatrix(&in);

    FILE *file = fopen("log.txt", "a");
    fprintf(file, "threads = %d\ntime = %f s\n\n", max_threads,(end-start)/1000);
    fclose(file);

    return 0;
}
