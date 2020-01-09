CC = gcc
CARGS1 = -Wall -g
CARGS2 = -lrt -lpthread

all: setup checkin 

setup: setup.c
	$(CC) $(CARGS1) setup.c -o setup $(CARGS2)

checkin: 	
	gcc -g -o checkin checkin.c checkin.h utilities.c utilities.h -lrt -lpthread

