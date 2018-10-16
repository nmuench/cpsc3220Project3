#include "plock.h"




plock_t *plock_create()
{
	//Allocates memory for the priority_lock.
	plock_t priority_lock = malloc(sizeof(plock_t));
	//Sets the priority_lock as free initially.
	priority_lock->value = FREE;
	//Uses pthread_mutex_init to initialize the mutex.
	pthread_mutex_init(&priority_lock->mlock, NULL);
	//Initially, there are no waiting threads, so head is set to NULL
	priority_lock->head = NULL;
	//Returns the created lock
	return priority_lock;
}


void plock_destroy(plock_t *lock)
{
	node_t * temp;
	//Garbage collects the remaining nodes on the waiting list
	while(lock->head != NULL)
	{
		//Temp now points to the next node in the list or NULL if this node doesn't exist.
		temp = lock->head->next;
		//Uses pthread_cond_destroy to destroy the condition inside the node
		pthread_cond_destroy(&lock->head->waitCV);
		//Frees the current node
		free(lock->head);
		//Sets lock->head to the next node to be freed, or NULL if all have been freed.
		lock->head = temp;
	}
	//Uses pthread_mutex_destroy to destroy the mutex within the plock
	pthread_mutex_destroy(&lock->mlock);
	//Frees the plock itself
	free(lock);
}

void plock_enter(plock_t *lock, int priority)
{
	//Enter the critical section.
	pthread_mutex_lock(&lock->mlock);

	//Checks if the plock is initially free, if it is not then a node must
	//be added into the waiting list.
	if(lock->value != FREE)
	{
		//Allocates memory for threadNode and initializes its variables.
		node_t *threadNode = malloc(sizeof(threadNode));
		threadNode->priority = priority;
		pthread_cond_init(&threadNode->waitCV);

		node_t * temp = lock->head;
		//Inserts at the head of the list if the head is NULL, or threadNode had the
		//highest priority.
		if(temp == NULL || temp->priority < threadNode->priority)
		{
			threadNode->next = temp;
			temp = threadNode;
		}
		//Otherwise, insert at the correct place in the linked list.
		else
		{
			//Continue advancing temp until it points at the node_t that should be
			//right before threadNode
			while(temp->next != NULL && temp->next->priority >= threadNode->priority)
			{
				temp = temp->next;
			}
			//Now insert threadNode into the list at this location.
			threadNode->next = temp->next;
			temp->next = threadNode;
		}

		//Now wait until threadNode is the highest priority node and the lock isn't
		//busy.
		while(lock->head != threadNode && lock->value != FREE)
		{
			pthread_cond_wait(&threadNode->waitCV, &lock->mlock);
		}
		//Once threadNode is the highest priority node, remove it from the linked
		//list by changing lock->head to lock->head->next and then destroying the
		//condition variable in the node and freeing the node.
		lock->head = lock->head->next;
		pthread_cond_destroy(&threadNode->waitCV);
		free(threadNode);
	}
	//Designate the plock as busy
	lock->value = BUSY;
	//Leaving Critical Section
	//Release the lock
	pthread_mutex_unlock(&lock->mlock);
}

void plock_exit(plock_t *lock)
{
	pthread_mutex_lock(&lock->mlock);
	//Set the lock to free
	lock->value = FREE;
	//If there are threads waiting, signal the thread with the highest priority.
	if(lock->head != NULL)
	{
		pthread_cond_signal(&lock->head->waitCV);
	}
	//Exit the critical Section
	pthread_mutex_unlock(&lock->mlock);

}
