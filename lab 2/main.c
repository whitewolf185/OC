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

//    1 is write, 0 is read

//   ----------- parent----------
    else if (ID > 0) {//parent
        printf("This is parent\n");
        fflush(stdout);
        close(fd1[0]);
        close(fd2[1]);

        struct numbers nums;

        while(scanf("%d %d %d", &nums.n1, &nums.n2, &nums.n3) > 0) {

            write(fd1[1], &nums, sizeof(struct numbers));

            int result;
            if(read(fd2[0], &result, sizeof(int)) < 0){
                printf("Read error");
                return 1;
            }
            printf("result is %d\n", result);

        }
        close(fd1[1]);
        close(fd2[0]);


    }


        //---------child----------
    else {//child
        printf("This is child\n");
        fflush(stdout);
        close(fd1[1]);
        close(fd2[0]);

        dup2(fd1[0],STDIN_FILENO);
        dup2(fd2[1], STDOUT_FILENO);

        char * const * arg = NULL;

        execv("child.out", arg);

        close(fd1[0]);
        close(fd2[1]);

    }



    return 0;
}
