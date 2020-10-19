#include "unistd.h"
#include "stdio.h"
#include "vector.h"

int main(int count, char *filename[]) {

    FILE *output = NULL;
    output = fopen(filename[0], "a");
    if (output == NULL) {
        printf("file error");
        fclose(output);
        return 1;
    }
    vector nums;
    create(&nums, 0);
    int tmp;
    while (read(STDIN_FILENO, &tmp, sizeof(int)) > 0) {
        push_back(&nums, tmp);
    }
    int i;
    int result = 0;
    for (i = 0; i < size(&nums); ++i) {
        result += nums.data[i];
        fprintf(output, "%d ", nums.data[i]);
    }
    fprintf(output, "= %d\n", result);


    fclose(output);
    return 0;
}
