#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define check(VALUE, MSG, BADVAL) if (VALUE == BADVAL) { perror(MSG); exit(1); }

int main(){
    char* libs[] = {"./liblib1.so", "./liblib2.so"};
    int lib = 0;
    int (*PrimeCount)(int, int) = NULL;
    void (*Sort)(int [SIZE]) = NULL;

    void* handle = dlopen(libs[lib], RTLD_NOW);
    check(handle,dlerror(),NULL);

    int command;
    int a,b;
    int arr[SIZE];
    while((printf("> ") && fflush(stdout)) || scanf("%d", &command) > 0){
        if(command == 0){
            if (dlclose(handle) != 0) {
                perror(dlerror());
                exit(1);
            }
            lib = (lib + 1) % 2;
            if(lib == 0){
                printf("Now im using lib1\n");
                fflush(stdout);
            }
            else{
                printf("Now im using lib2\n");
                fflush(stdout);
            }
            handle = dlopen(libs[lib], RTLD_NOW);
            check(handle, dlerror(), NULL);
        }

        else if(command == 1){
            printf("Enter your a and b\n>> ");
            fflush(stdout);
            if(scanf("%d %d", &a, &b) != 2){
                perror("invalid arguments");
                exit(1);
            }

            PrimeCount = dlsym(handle, "PrimeCount");
            check(PrimeCount,dlerror(),NULL);
            printf("Prime nums: %d\n", (*PrimeCount)(a,b));
            fflush(stdout);
        }
        else if(command == 2){
            printf("Enter your array. SIZE = %d\n>> ", SIZE);
            fflush(stdout);
            //array initialize
            for (int i = 0; i < SIZE; ++i) {
                scanf("%d", &arr[i]);
            }

            Sort = dlsym(handle, "Sort");
            check(Sort, dlerror(), NULL);
            (*Sort)(arr);

            printf("array is:\n");
            fflush(stdout);
            for (int i = 0; i < SIZE; ++i) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            fflush(stdout);
        }
    }

    if (dlclose(handle) != 0) {
        perror(dlerror());
        exit(1);
    }

    return 0;
}