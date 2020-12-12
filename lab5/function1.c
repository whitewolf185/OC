#include "functions.h"

void swap(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void Sort(int arr[SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE - i; ++j) {
            if(arr[j] > arr[j+1] ){
                swap(&arr[j],&arr[j+1]);
            }
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
