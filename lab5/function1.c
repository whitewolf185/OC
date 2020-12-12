#include "functions.h"

void swap(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void Sort(int arr[SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        int do_swap = 0;
        int min = 0;
        for (int j = i+1; j < SIZE; ++j) {
            if(arr[i] > arr[j]){
                do_swap = 1;
                min = j;
            }
        }

        if(do_swap){
            swap(&arr[i],&arr[min]);
        }
    }
}

#define max(a, b) ((a) > (b)) ? (a) : (b)

int PrimeCount(int A, int B) {
    int n = 0, prime;
    for (int num = max(A, 2); num <= B; ++num) {
        prime = 1;
        for (int i = 2; i < num; ++i) {
            if (num % i == 0) {
                prime = 0;
                break;
            }
        }
        if (prime == 1) {
            ++n;
        }
    }
    return n;
}
