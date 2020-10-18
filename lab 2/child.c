#include "unistd.h"
#include "stdio.h"


struct numbers {
    int n1, n2, n3;
};

int main() {
//TODO здесь нужно пользоваться read, у которого первый аргумент будет ноль.
// pipe распределяется сам как ему надо
    struct numbers nums;
    FILE* output = NULL;
    output = fopen("out.txt","a");
    if(output == NULL){
        printf("file error");
        fclose(output);
        return 1;
    }
    while(read(STDIN_FILENO, &nums, sizeof(struct numbers)) > 0) {

        int result = nums.n1 + nums.n2 + nums.n3;
        write(STDOUT_FILENO, &result, sizeof(int));
        fprintf(output, "%d + %d + %d = %d\n",nums.n1, nums.n2, nums.n3, result);
    }
    fclose(output);
    return 0;
}
