#include "unistd.h"
#include "stdio.h"

struct numbers {
    int n1, n2;
};

int main(int count, char *filename[]) {

    struct numbers nums;
    FILE *output = NULL;
    output = fopen(filename[0], "a");
    if (output == NULL) {
        printf("file error");
        fclose(output);
        return 1;
    }
    while (read(STDIN_FILENO, &nums, sizeof(struct numbers)) > 0) {
        int result = nums.n1 + nums.n2;
        fprintf(output, "%d + %d = %d\n", nums.n1, nums.n2, result);
    }
    fclose(output);
    return 0;
}
