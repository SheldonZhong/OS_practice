#include <pthread.h>
#define MIN_PID 300
#define MAX_PID 5000
#define PID_LEN MAX_PID - MIN_PID + 1

pthread_mutex_t mutex;
int PID[MAX_PID - MIN_PID];

/* 
Creates and initializes a data structure for representing pids; 
returns 0 if unsuccessful, 1 if successful
*/
int allocate_map(void);

/*
Allocates and returns a pid;
returns 1 if unable to allocate a pid (all pids are in use)
*/
int allocate_pid(void);

/*
Releases a pid
*/
void release_pid(int);

/*
Init for mutex lock
*/
void init_pid(void);