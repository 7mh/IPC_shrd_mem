#include "utilities.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* addr + (size-1) & ~8UL */

unsigned long alignAddress(void *addr){
  return (unsigned long) (addr + (sizeof(void*)-1UL)) & ~ 8UL;
}

char *getUser() {
  /* Store information about the current user running the program*/
  struct passwd *login;

  /* Get the uid - from the Man Pages */
  int id = getuid();
  login = getpwuid(id);
  if (!login) {
    fprintf(stderr, "%sError parsing user information.%s\n", RED, COLOR_RESET);
    exit(-1);
  }
  return login->pw_name;
}
