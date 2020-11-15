#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

int k,n,m;

int main(int argc, char const *argv[]){
    printf("Enter size of matrix. Enter N and M\n");
    scanf("%d %d", &n, &m);

    printf("Enter your K\n");
    fflush(stdout);
    scanf("%d", &k);


    return 0;
}
