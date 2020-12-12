#include "functions.h"
#include "stdlib.h"
#include "stdio.h"

void swap(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void Qsort(int arr[SIZE], int lhs, int rhs){
    int l = lhs;
    int r = rhs;
    int rep = arr[(l + r)/2];

    while(l <= r){
        while(arr[l] < rep){
            ++l;
        }
        while(arr[r] > rep){
            --r;
        }
        if(l <= r){
            swap(&arr[l++], &arr[r--]);
        }
    }

    if(lhs < r){
        Qsort(arr, lhs, r);
    }
    if(l < rhs){
        Qsort(arr,l,rhs);
    }
}

void Sort(int arr[SIZE]){
    Qsort(arr,0,SIZE - 1);
}

int PrimeCount(int A, int B) {
    int n = 0;
    int *sieve = (int *)malloc(sizeof(int) * (B+1));
    if (sieve == NULL) {
        perror("malloc error");
        exit(1);
    }
    for (int i = 0; i <= B; ++i) sieve[i] = 0;
    for (int i = 2; i <= B; ++i) {
        if (sieve[i] != 0) {
            continue;
        }
        ++n;
        for (int j = i + i; j <= B; j += i) {
            sieve[j] = 1;
        }
    }
    return n;
}