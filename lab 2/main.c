#include "unistd.h"
#include "stdio.h"

struct numbers {
    int n1, n2;
};

int main() {
    printf("Enter filename\n");
    fflush(stdout);
    char arg[126];
    scanf("%s\n", arg);
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
//    1 is write, 0 is read
//   ----------- parent----------
    else if (ID > 0) {
        close(fd1[0]);
        close(fd2[1]);
        struct numbers nums;
        while (scanf("%d %d", &nums.n1, &nums.n2) > 0) {
            write(fd1[1], &nums, sizeof(struct numbers));
        }
        close(fd1[1]);
        close(fd2[0]);
    }
        //---------child----------
    else {
        close(fd1[1]);
        close(fd2[0]);

        dup2(fd1[0], STDIN_FILENO);
        execl("child.out", arg, NULL);

        close(fd1[0]);
        close(fd2[1]);

    }
    return 0;
}
