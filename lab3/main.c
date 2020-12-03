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
//#define ENTER

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
#ifdef DEBUG
    printf("Border: %d %d\n",( (id->winX - 1)/2 ), ( (id->winY - 1) / 2 ));
#endif //DEBUG
    for (int i = max(0, id->x - ((id->winX - 1)/2)); i <= min(n-1, id->x + ((id->winX - 1) / 2)); ++i) {
        for (int j = max(0, id->y - ((id->winY - 1) / 2)); j <= min(m-1, id->y + ((id->winY - 1) / 2)); ++j) {
#ifdef DEBUG
            printf("IN FOR %di %dj\nmatrix %d\n",i,j,in.matr[i][j]);
            fflush(stdout);
#endif //DEBUG
            push_back(&pixels,in.matr[i][j]);
        }
    }

#ifdef DEBUG
    printf("_____\n");
    printf("BEFORE MERGE  id is %dx %dy\n",id->x,id->y);
    print_all(&pixels);
    printf("\n");
    fflush(stdout);
#endif //DEBUG

    merge(&pixels,0,pixels.size-1);

#ifdef DEBUG
    printf("AFTER MERGE  id is %dx %dy\n",id->x,id->y);
    print_all(&pixels);
    printf("\n");
    fflush(stdout);
#endif //DEBUG


    if(m > 1 && n > 1){
        out.matr[id->x][id->y] = pixels.data[pixels.size/2];
        destroy(&pixels);
        return NULL;
    }
    else {
        out.matr[id->x][id->y] = in.matr[id->x][id->y];
        destroy(&pixels);
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
#ifdef ENTER
    printMatrix(&in);
#endif
    printf("Enter your window\n> ");
    fflush(stdout);
    scanf("%d %d", &winX, &winY);

    //условия задания окна
    if( winX != winY ||
        winX == 0 || winY == 0 ||
        winX > n || winY > m ||
        winX % 2 == 0)
    {
        printf("I cannot work with this window\n");
        return 0;
    }

    printf("Enter your K\n> ");
    fflush(stdout);
    scanf("%d", &k);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t)*max_threads);

    double start,end;

    struct tr_data* args = (struct tr_data*) malloc(sizeof(struct tr_data)*n*m);
    start = clock();

    for (int h = 0; h < k; ++h) {
        scanf_Matrix_with_Fill_0(&out,n,m);
#ifdef DEBUG
        printf("Im here with K=%d\n\n",k);
        fflush(stdout);
#endif //DEBUG
        int count = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                args[count].x = i;
                args[count].y = j;
                args[count].winX = winX;
                args[count].winY = winY;
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
#ifdef ENTER
    printMatrix(&in);
#endif

    FILE *file = fopen("log.txt", "a");
    FILE* thread = fopen("D:\\Documents\\Projects\\c++\\OC\\venv\\treads.txt","a");
    FILE* time = fopen("D:\\Documents\\Projects\\c++\\OC\\venv\\time.txt","a");

    fprintf(file, "threads = %d\ntime = %fms\n\n", max_threads,(end-start)/1000);
    fprintf(thread, "%d\n",max_threads);
    fprintf(time,"%f\n",(end-start)/1000);

    fclose(file);
    fclose(thread);
    fclose(time);

    return 0;
}
