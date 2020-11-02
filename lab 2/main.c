#include "unistd.h"
#include "stdio.h"

#define WRITE 1
#define READ 0

int main() {
    printf("Enter filename\n");
    fflush(stdout);
    char arg[256];
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
//   ----------- parent----------
    else if (ID > 0) {
        close(fd1[READ]);
        close(fd2[WRITE]);
        char tmp;
        while (scanf("%c", &tmp) > 0) {
            write(fd1[WRITE], &tmp, sizeof(char));
        }
        close(fd1[WRITE]);
        close(fd2[READ]);
    }
        //---------child----------
    else {
        close(fd1[WRITE]);
        close(fd2[READ]);

        dup2(fd1[READ], STDIN_FILENO);
        execl("child.out", arg, NULL);

        close(fd1[READ]);
        close(fd2[WRITE]);

    }
    return 0;
}
