/* Nick Muenchen
   CPSC 3220
	 Project 3
	 1 November 2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unist.h>
#include <pthread.h>

#define FREE 0
#define BUSY 0

typedef struct node_def{
	int priority;	/*priority of the waiting thread*/
	pthread_cond_t waitCV; /*thread waits on this condition variable */
	struct node_def *next;	/*node is part of a singly-linked list */
} node_t;

typedef struct{
	int value;	/*FREE or BUSY */
	pthread_mutex_t mlock;	/* mutex protecting the plock structure */
	node_t *head;	/* linked list of waiting threads */
} plock_t;

plock_t *plock_create();
void plock_destroy(plock_t *lock);
void plock_enter(plock_t *lock, int priority);
void plock_exit(plock_t *lock);
