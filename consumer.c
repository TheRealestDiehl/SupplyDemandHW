#include <string.h>
#include "shared.h"

int main(int argc, char *argv[]){
    
    int fd;
    char *shmpath, *string;
    size_t len;
    struct shmbuf *buff;

    if (argc != 3){
        fprintf(stderr, "Usage: %s /shm-path string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmpath = argv[1];
    string = argv[2];
    len = strlen(string);
    if (len>BUF_SIZE){
        fprintf(stderr, "String is too long\n");
        exit(EXIT_FAILURE);
    }
    fd = shm_open(shmpath, O_RDWR, 0);
    if (fd == -1) errExit("shm_open");
    buff = mmap(NULL, sizeof(*buff), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    if(buff == MAP_FAILED) errExit("mmap");
    buff->cnt = len;
    memcpy(&buff->buf,string,len);
    if(sem_post(&buff->sem1) == -1) errExit("sempost");
    if(sem_wait(&buff->sem2) == -1) errExit("semwait");
    write(STDOUT_FILENO, &buff->buf, len);
    write(STDOUT_FILENO, "\n", 1);
    exit(EXIT_SUCCESS);
}
