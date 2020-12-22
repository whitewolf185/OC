#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    int command;
    int a,b;
    int arr[SIZE];
    while(scanf("%d", &command) > 0){
        if(command == 1){
            if(scanf("%d %d", &a, &b) != 2){
                perror("invalid input");
                exit(1);
            }
            printf("Prime numbers: %d\n", PrimeCount(a,b));
            fflush(stdout);
        }

        else if(command == 2){
            printf("Enter your array. SIZE = %d\n>> ", SIZE);
            fflush(stdout);
            //array initialize
            for (int i = 0; i < SIZE; ++i) {
                scanf("%d", &arr[i]);
            }

            Sort(arr);
            for (int i = 0; i < SIZE; ++i) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            fflush(stdout);
        }
    }

    return 0;
}