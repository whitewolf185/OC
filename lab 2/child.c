#include "unistd.h"


struct numbers {
    int n1, n2, n3;
};

int main() {
//TODO здесь нужно пользоваться read, у которого первый аргумент будет ноль.
// pipe распределяется сам как ему надо
    struct numbers nums;
    while(read(STDIN_FILENO, &nums, sizeof(struct numbers)) > 0) {

        int result = nums.n1 + nums.n2 + nums.n3;
        write(STDOUT_FILENO, &result, sizeof(int));
    }

    return 0;
}
