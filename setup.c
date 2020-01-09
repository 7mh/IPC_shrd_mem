#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "utils.h"
#include "checkin.h"

void logAnswers(int fd){
  printf("Logging previous submission\n");
  void *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  int *count = addr + sizeof(sem_t);
  user *answers = addr + sizeof(int) + sizeof(sem_t);
  FILE* file = fopen("answers.log", "w");
  for(int i = 0; i < *count; i++){
    fprintf(file, "%s: %d, %d\n", answers[i].login, answers[i].answer, answers[i].time);
  }
}

int main(int argc, char **argv) {
  /* First we should unlink our old file */
  int fd = shm_open(GRP, O_RDWR, 0666);
  if (fd > 0){
    logAnswers(fd);
    shm_unlink(GRP);
  }

  /* Setting file creation mask to 0 */
  umask(0);

  /* Open virtual file */
  fd = shm_open(GRP, O_RDWR | O_CREAT | O_TRUNC, 0666);

  ftruncate(fd, fsize);

  void *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  if (addr == NULL){
    fprintf(stderr,"%sUnable to map shared memory.%s\n", RED, COLOR_RESET);
    return 0;
  }

  /* Mutex to protect everything */
  sem_t *mutex = addr;

  /* Share the mutex between processes? */
  sem_init(mutex, 1, 1);

  /* Set our answer count to 0 just to be safe */
  answerCount = addr + sizeof(sem_t);
  if(answerCount)
    *answerCount = 0;
  else{
    fprintf(stderr, "%sThere is a problem with the shared memory.%s\n", RED, COLOR_RESET);
  }



}
