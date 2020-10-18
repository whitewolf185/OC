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

//   ----------- parent----------
    else if (ID > 0) {//parent
        printf("This is parent\n");
        fflush(stdout);


        close(fd2[1]);
        close(fd1[0]);
        close(fd1[1]);

        int result;
        read(fd2[0], &result, sizeof(int));
        printf("result is %d\n", result);
        fflush(stdout);
        close(fd2[0]);
    }


        //---------child----------
    else {//child
        printf("This is child\n");
        fflush(stdout);

        close(fd2[1]);//write
        close(fd2[0]);
        close(fd1[0]);//read

        struct numbers nums;

        if (dup2(fd1[1], STDOUT_FILENO) < 0) {
            printf("Dup error\n");
            return 1;
        }

        printf("Enter nums\n");
        fflush(stdout);
        scanf("%d %d %d", &nums.n1, &nums.n2, &nums.n3);
        write(fd1[1], &nums, sizeof(struct numbers));

        char *const *arg = NULL;
        if (execv("child.out", arg) < 0) {
            printf("Exec error\n");
            return 1;
        }
        close(fd1[1]);

    }

    return 0;
}
