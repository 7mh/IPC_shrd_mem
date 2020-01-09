#include "checkin.h"
#include "utilities.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>


/* Function to check if a user has submitted an answer already. */
int inList(char login[16]) {
  for (int i = 0; i < *answerCount; i++){
    if (strcmp(login, (answers+i)->login) == 0)
      return 1;
    answers = (user*) alignAddress(answers);
  }
  return 0;
}

/* Add an answer to the list */
void addToList(char login[16], int answer){
  if(*answerCount == MAX_USERS){
    fprintf(stderr, "%sThe maximum number of users have submitted an answer.%s\n", RED, COLOR_RESET);
    return;
  }
  answers[*answerCount].answer = answer;
  /* answers[*answerCount].login = strdup(login); */
  strcpy(answers[*answerCount].login, login);
  answers[*answerCount].time = time(0);
  printf("Username: %s\nAnswer: %d\n", answers[*answerCount].login, answers[*answerCount].answer);
  *answerCount = *answerCount + 1;
}

/* The critical section */
int submitQuestion(int answer){
  char *login = getUser();
  /* Wait for this users turn */
  sem_wait(mutex);
  if (inList(login)){
    fprintf(stderr, "%sYou have already submitted an answer to this question.%s\n", RED, COLOR_RESET);
  }
  else{
   addToList(login, answer);
  }
  /* Let other users in */
  sem_post(mutex);
  return 1;
}

/* Open our predefined section of shared memory */
void openSharedMemory(){
  fd = shm_open(GRP, O_RDWR, 0666);
  if (fd == -1){
    perror("Error opening shared memory.");
    exit(0);
  }
  /* Map memory */
  void *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  /* Close fd after we map our shared memory */
  close(fd);
  /* Paranoia */
  if (addr == NULL){
    fprintf(stderr,"%sUnable to map shared memory.%s\n", RED, COLOR_RESET);
    exit(0);
  }
  mutex = addr;
  answerCount = addr + sizeof(sem_t);
  answers = (user*)(addr + sizeof(sem_t) + sizeof(int));
  /* answers = (user*)alignAddress(answerCount); */
  printf("Size of ");
  printf("Mutex addr = %p\n",mutex );
  printf("Mutex Value = %d \n",*mutex);
  printf("Answer Count: %d\n", *answerCount);
  printf("Address of Count= %p \n", answerCount );
  printf("Address of Answers: %p\n", answers);
  printf("Size of Answers: %ld\n", sizeof(answers));
}

int main(int argc, char **argv) {
  printf("\nRunning checkin program.\n");
  if (argc < 2) {
    fprintf(stderr, "%sYou must enter an answer.%s\n", RED, COLOR_RESET);
    exit(-1);
  }
  openSharedMemory();

  char *endptr;
  long answer = strtol(argv[1], &endptr, 10);
  submitQuestion((int) answer);
  printf("Answer count: %d\n", *answerCount);
}
