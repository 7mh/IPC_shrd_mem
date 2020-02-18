# Answer Polling Using Shared Memory IPC
## Order of running files

> Setup file:
   This configures a shared memory space on server system.
   
> "checkin" file executed by multiple clients individually (multiple processes)
  Through this program client systems access the shared memory space
  and register their answers. 
  Each client systems are allowed to access the shared memory space only once and one at a time because of the mutex implemented at shared memory space.
