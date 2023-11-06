#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <pthread.h>

#define errExit(msg) do{perror(msg); exit(EXIT_FAILURE);} while (0);
#define BUF_SIZE 1024 /*Max size for exchanged string*/

/*Structure that will be shared*/
struct shmbuf{
    sem_t sem1;
    sem_t sem2;         /*POSIX unnamed semaphores*/
    size_t cnt;         /*Number of bytes in buf*/
    char buf[BUF_SIZE]; /*Data being transferred*/
};