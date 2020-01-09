#ifndef __CHECKIN_H_
#define __CHECKIN_H_

#include <semaphore.h>

/* Constants */
#define GRP "GroupF"
#define MAX_USERS 64

/* Data structures */
typedef struct USER {
  char login[16];
  int time;   // Unix epoch time as returned by time(0)
  int answer; // could also be a single char (e.g., ’T’ or ’F’)
} user;

/* Global variables */
user *answers = 0;
int *answerCount = 0;
sem_t *mutex;
int fd;
int fsize = 4096;

/* Function Prototypes */
int inList(char login[16]);
int submitQuestion(int answer);
void addToList(char login[16], int answer);
void openSharedMemory();

#endif // __CHECKIN_H_
