#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    FILE *ResFile = fopen(argv[1], "a");
    if (ResFile == NULL) {
        perror("fopen error\n");
        return -1;
    }

    //инициализация mutex и тд
    int mutex1 = shm_open(argv[2], O_RDWR | O_CREAT, S_IRWXU);
    int mutex2 = shm_open(argv[3], O_RDWR | O_CREAT, S_IRWXU);
    int pipe1 = shm_open(argv[4], O_RDWR | O_CREAT, S_IRWXU);
    int pipe1Size = shm_open(argv[5], O_RDWR | O_CREAT, S_IRWXU);
    if (pipe1 == -1 || pipe1Size == -1 || mutex1 == -1 || mutex2 == -1) {
        perror("shm_open\n");
        return -1;
    }

    //навешивание mmap
    pthread_mutex_t *mutex = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
                                                      MAP_SHARED,
                                                      mutex1, 0);
    pthread_mutex_t *secondMutex = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t),PROT_READ | PROT_WRITE,
                                                            MAP_SHARED,
                                                            mutex2, 0);

    int *mmfData = (int *) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, pipe1, 0);
    int *mmfDataSize = (int *) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, pipe1Size, 0);
    if (mmfData == MAP_FAILED || mmfDataSize == MAP_FAILED || mutex == MAP_FAILED || secondMutex == MAP_FAILED) {
        perror("mmap error\n");
        return -1;
    }

    //тут выполняется вся работа
    while (*mmfDataSize != -2) {
        if (pthread_mutex_lock(mutex) != 0) {
            perror("pthread_mutex_lock error\n");
            return -1;
        }
        if (*mmfDataSize == -2) {
            break;
        }
        int result = 0;
        for (int i = 0; i < *mmfDataSize; ++i) {
            fprintf(ResFile, "%d ", mmfData[i]);
            result += mmfData[i];
        }
        fprintf(ResFile, "= %d\n", result);
        *mmfDataSize = -1;
        if (pthread_mutex_unlock(secondMutex) != 0) {
            perror("pthread_mutex_unlock error\n");
            return -1;
        }
    }
    fclose(ResFile);
    return 0;
}