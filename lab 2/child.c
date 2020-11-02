#include "unistd.h"
#include "stdio.h"
#include "vector.h"

int transform(char x) {
    return x - '0';
}

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
    char tmp;
    int is_minus = 1;
    int digit_result = 0;
    int if_space = 0;
    while (read(STDIN_FILENO, &tmp, sizeof(char)) > 0) {
        if (tmp == '-') {
            if_space = 0;
            is_minus = -1;
        }

        if (tmp == ' ') {
            digit_result /= 10;
            digit_result = is_minus * digit_result;
            push_back(&nums, digit_result);
            is_minus = 1;
            digit_result = 0;
            if_space = 1;
        }

        if (tmp >= '0' && tmp <= '9') {
            if_space = 0;
            digit_result += transform(tmp);
            digit_result *= 10;
        }

        if (tmp == '\n') {
            if (!if_space) {
                digit_result /= 10;
                digit_result = is_minus * digit_result;
                push_back(&nums, digit_result);
                is_minus = 1;
                digit_result = 0;
                if_space = 1;
            }
            int i;
            int result = 0;
            for (i = 0; i < size(&nums); ++i) {
                result += nums.data[i];
                fprintf(output, "%d ", nums.data[i]);
            }
            fprintf(output, "= %d\n", result);
            destroy(&nums);
        }
    }
    fclose(output);
    return 0;
}
