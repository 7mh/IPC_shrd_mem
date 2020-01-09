#ifndef __UTILS_H_
#define __UTILS_H_

/* Colors as always are courtesy of Tyler */
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define TEAL "\x1b[36m"
#define RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

char *getUser();
unsigned long alignAddress(void *addr);

#endif // __UTILS_H_
