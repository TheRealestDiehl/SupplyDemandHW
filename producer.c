#include <ctype.h>
#include "shared.h"

int main(int argc, char *argv[]){
    
    int fd;
    char *shmpath;
    struct shmbuf *buff;

    if (argc != 2){
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmpath = argv[1];

    fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) errExit("shm_open");
    if (ftruncate(fd,sizeof(struct shmbuf)) == -1) errExit("ftruncate");
    buff = mmap(NULL, sizeof(*buff), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    if(buff == MAP_FAILED) errExit("mmap");
    if(sem_init(&buff->sem1, 1, 0) == -1) errExit("seminit,sem1");
    if(sem_init(&buff->sem2, 1, 0) == -1) errExit("seminit,sem2");
    if(sem_wait(&buff->sem1) == -1) errExit("wait");
    for(size_t j = 0; j < buff->cnt; j++) buff->buf[j] = toupper((unsigned char) buff->buf[j]);
    if(sem_post(&buff->sem2) == -1) errExit("post");
    shm_unlink(shmpath);
    exit(EXIT_SUCCESS);
}