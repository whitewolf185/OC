#include "unistd.h"
#include "stdio.h"

int main() {
//TODO здесь нужно пользоваться read, у которого первый аргумент будет ноль.
// pipe распределяется сам как ему надо
    int result = n1 + n2 + n3;
    write(STDOUT_FILENO, &result, sizeof(int));

    return 0;
}
