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

}tr_data;

#define RAND_MAX 10
//#define DEBUG
//#define MERGE

int k,n,m;

void* threadFunc(void* Tdata){
    //printf("im alive\n");
    fflush(stdout);
    struct tr_data* id = (struct tr_data*) Tdata;
    vector pixels;
    create(&pixels,0);

#ifdef DEBUG
    printf("Im in threadFunc with Id = %d %d\n", id->x, id->y);
    fflush(stdout);
#endif //DEBUG

    //угловые пиксели
    if(id->x == 0 && id->y == 0
       || id->x == n-1 && id->y == m-1
       || id->x == 0 && id->y == m-1
       || id->x == n-1 && id->y == 0)
    {

#ifdef DEBUG
        printf("Im in firs if with Id = %d %d; m = %d; n = %d\n", id->x, id->y,m,n);
        fflush(stdout);
#endif //DEBUG

        int flagX = (id->x == 0)? 1 : -1;
        int flagY = (id->y == 0)? 1 : -1;

        if(m > 1 && n > 1){
            push_back(&pixels,in.matr[id->x][id->y]);
            push_back(&pixels,in.matr[id->x+flagX][id->y]);
            push_back(&pixels,in.matr[id->x][id->y+flagY]);
            push_back(&pixels,in.matr[id->x+flagX][id->y+flagY]);


#ifdef DEBUG
            printf("This is pixels before MERGE\n");
            print_all(&pixels);
            fflush(stdout);
#endif //DEBUG

            merge(&pixels,0,pixels.size-1);

#ifdef DEBUG
            printf("This is pixels after MERGE\n");
            print_all(&pixels);
            fflush(stdout);
#endif //DEBUG

#ifdef DEBUG
            printf("\nShow me Matrix OUT\n");
            printMatrix(&out);
            fflush(stdout);
#endif //DEBUG

            out.matr[id->x][id->y] = (pixels.data[1] + pixels.data[2]) / 2;

#ifdef DEBUG
            printf("\nShow me Matrix OUT again\n");
            printMatrix(&out);
            fflush(stdout);
#endif //DEBUG

            destroy(&pixels);
            return NULL;
        }
        else{
            out.matr[id->x][id->y] = in.matr[id->x][id->y];
            return NULL;
        }
    }


        //края без угловых элементов
    else if(id->x == 0 ||  id->y == 0 ||
            id->x == n-1 || id->y == m-1)
    {

#ifdef DEBUG
        printf("Im in second if with Id = %d %d; m = %d; n = %d\n", id->x, id->y,m,n);
        fflush(stdout);
#endif //DEBUG

        if(id->x == 0){
            if(m > 1 && n > 1){
                push_back(&pixels,in.matr[id->x][id->y]);

                push_back(&pixels,in.matr[id->x+1][id->y]);
                push_back(&pixels,in.matr[id->x][id->y+1]);
                push_back(&pixels,in.matr[id->x][id->y-1]);

                push_back(&pixels,in.matr[id->x+1][id->y+1]);
                push_back(&pixels,in.matr[id->x+1][id->y-1]);



                merge(&pixels,0,pixels.size-1);

                out.matr[id->x][id->y] = pixels.data[pixels.size/2];
                destroy(&pixels);
                return NULL;
            }
            else{
                out.matr[id->x][id->y] = in.matr[id->x][id->y];
                return NULL;
            }
        }

        if(id->y == 0){
            if(m > 1 && n > 1){
                push_back(&pixels,in.matr[id->x][id->y]);

                push_back(&pixels,in.matr[id->x][id->y+1]);
                push_back(&pixels,in.matr[id->x+1][id->y]);
                push_back(&pixels,in.matr[id->x-1][id->y]);

                push_back(&pixels,in.matr[id->x+1][id->y+1]);
                push_back(&pixels,in.matr[id->x-1][id->y+1]);


                merge(&pixels,0,pixels.size-1);

                out.matr[id->x][id->y] = pixels.data[pixels.size/2];
                destroy(&pixels);
                return NULL;
            }
            else{
                out.matr[id->x][id->y] = in.matr[id->x][id->y];
                return NULL;
            }
        }



        if(id->x == n-1){
            if(m > 1 && n > 1){
                push_back(&pixels,in.matr[id->x][id->y]);

                push_back(&pixels,in.matr[id->x-1][id->y]);
                push_back(&pixels,in.matr[id->x][id->y+1]);
                push_back(&pixels,in.matr[id->x][id->y-1]);

                push_back(&pixels,in.matr[id->x-1][id->y+1]);
                push_back(&pixels,in.matr[id->x-1][id->y-1]);



                merge(&pixels,0,pixels.size-1);

                out.matr[id->x][id->y] = pixels.data[pixels.size/2];
                destroy(&pixels);
                return NULL;
            }
            else{
                out.matr[id->x][id->y] = in.matr[id->x][id->y];
                return NULL;
            }
        }

        if(id->y == m-1){
            if(m > 1 && n > 1){
                push_back(&pixels,in.matr[id->x][id->y]);

                push_back(&pixels,in.matr[id->x][id->y-1]);
                push_back(&pixels,in.matr[id->x+1][id->y]);
                push_back(&pixels,in.matr[id->x-1][id->y]);

                push_back(&pixels,in.matr[id->x+1][id->y-1]);
                push_back(&pixels,in.matr[id->x-1][id->y-1]);


                merge(&pixels,0,pixels.size-1);

                out.matr[id->x][id->y] = pixels.data[pixels.size/2];
                destroy(&pixels);
                return NULL;
            }
            else{
                out.matr[id->x][id->y] = in.matr[id->x][id->y];
                return NULL;
            }
        }

    }


        //все остальные случаи
    else{
        if(m > 1 && n > 1){
            push_back(&pixels,in.matr[id->x][id->y]);

            push_back(&pixels,in.matr[id->x+1][id->y]);
            push_back(&pixels,in.matr[id->x][id->y+1]);
            push_back(&pixels,in.matr[id->x+1][id->y+1]);

            push_back(&pixels,in.matr[id->x-1][id->y]);
            push_back(&pixels,in.matr[id->x][id->y-1]);
            push_back(&pixels,in.matr[id->x-1][id->y-1]);

            push_back(&pixels,in.matr[id->x-1][id->y+1]);

            push_back(&pixels,in.matr[id->x+1][id->y-1]);

            merge(&pixels,0,pixels.size-1);

            out.matr[id->x][id->y] = pixels.data[pixels.size/2];
            destroy(&pixels);
            return NULL;
        }
        else{
            out.matr[id->x][id->y] = in.matr[id->x][id->y];
            return NULL;
        }
    }
}


int main(int argc, char const *argv[]){
    unsigned int max_threads = 1;
    if(argc > 1 && atoi(argv[1]) > 0){
        max_threads = atoi(argv[1]);
    }

#ifdef MERGE
    struct vector mer;
    create(&mer,0);
    srand(time(NULL));
    for (int i = 0; i < 4; ++i) {
        int a;
        scanf("%d",&a);
        push_back(&mer,a);
    }

    printf("-|");
    print_all(&mer);
    merge(&mer,0,mer.size-1);
    printf("-|");
    print_all(&mer);
// 8 9 7 8 5 1 5 9 5 6
    return 0;
#endif

    printf("Threads %d\nEnter size of matrix. Enter N and M\n> ",max_threads);
    fflush(stdout);
    scanf("%d %d", &n, &m);

    if(n == 0 || m == 0){
        printf("I cannot create matrix with this n and m\n");
        return 0;
    }

    scanf_With_Fill_Rand_Matrix(&in, n, m);
    //printMatrix(&in);

    printf("Enter your K\n> ");
    fflush(stdout);
    scanf("%d", &k);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t)*max_threads);

    time_t start,end;

    struct tr_data* args = (struct tr_data*) malloc(sizeof(struct tr_data)*n*m);
    start = time(NULL);
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
    end = time(NULL);

    free(threads);
    free(args);

    printf("Result matrix:\n");
    //printMatrix(&in);

    FILE *file = fopen("log.txt", "a");
    fprintf(file, "threads = %d\ntime = %ld s\n\n", max_threads,end-start);
    fclose(file);

    return 0;
}
