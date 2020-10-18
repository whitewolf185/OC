#include "unistd.h"
#include "stdio.h"

struct numbers {
    int n1, n2, n3;
};

int main() {
//TODO здесь нужно пользоваться read, у которого первый аргумент будет ноль.
// pipe распределяется сам как ему надо
    struct numbers nums;
    read(0, &nums, sizeof(struct numbers));

    int result = nums.n1 + nums.n2 + nums.n3;
    write(STDOUT_FILENO, &result, sizeof(int));

    return 0;
}
