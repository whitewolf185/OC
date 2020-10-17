#include "unistd.h"
#include "stdio.h"

struct numbers {
    int n1, n2, n3;
};

int main() {
    int fd2[2];
    int fd1[2];
    if (pipe(fd2) < 0) {
        printf("Pipe error!");
        return -1;
    }
    if (pipe(fd1) < 0) {
        printf("Pipe error!");
        return -1;
    }

    int ID = fork();
    if (ID < 0) {
        printf("Fork error!");
        return 1;
    }
    else if (ID > 0) {//parent
        printf("This is parent\n");
        fflush(stdout);
        close(fd2[1]);//write
        close(fd1[0]);//read

        int result;
        read(fd2[0], &result, sizeof(int));
        printf("result is %d\n", result);
        fflush(stdout);
        close(fd2[0]);

//      TODO нужно сделать переопределение дескриптора с помощью dup2.
//       И читать scanf нужно здесь
        struct numbers nums;
        scanf("%d %d %d", &nums.n1, &nums.n2, &nums.n3);
        write(fd1[1], &nums, sizeof(struct numbers));
        void *arg = NULL;
        dup2()
        execl("child.c", arg);
        close(fd1[1]);
    }

    else {//child
        close(fd2[0]);
        printf("This is child\n");
        fflush(stdout);


    }

    return 0;
}
