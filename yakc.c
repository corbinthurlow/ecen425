#include <stdio.h>
#include <stdint.h>
#include "yakk.h"
#include "yaku.h"
#include "clib.h"

int YKCtxSwCount = NULL;
int YKIdleCount = NULL;
TCBptr priorityHead = NULL;
int isYKRunCalled = FALSE;

int YKIdleStk[MAX_IDLE_STACK_SIZE]; 

int compareTCBPriority(TCBptr new, TCBptr comparingTo);
void insertTCB(TCBptr newTask);

void YKInitialize(void) {
	// still needs to be worked on?
	YKNewTask(YKIdleTask, (void *)&YKIdleStk[MAX_IDLE_STACK_SIZE], IDLE_TASK_PRIORITY_VALUE);
}

void YKEnterMutex(void);

void YKExitMutex(void);


void YKIdleTask(void){
	int i = 0;
	while(1) {
		for (i = 0; i < 32767; i++) {
			 printString("still inside idle task...\n"); // print it often to see if in idle task.
		}
		YKIdleCount++;
	}


}

void YKNewTask(void(*task(void)),void *taskStack,unsigned char priority) {
	TCBptr newTask;
	newTask->(*(stackptr)) = &taskStack;
	newTask->state = ready;
	newTask->priority = priority;
	newTask->delay = 0;
	newTask->next = NULL;
	newTask->prev = NULL;

	insertTCB(newTask);
	if (isYKRunCalled) {
		//call scheduler
		
	}
}

void insertTCB(TCBptr newTask) {
		if (priorityHead == NULL) { // if the very first insertion
		priorityHead = newTask;
	}
	else { 				// else need to find a place to but in double link list
		TCBptr tempCurr = priorityHead;
		if (compareTCBPriority(newTask, tempCurr)) {
			priorityHead = newTask;
			newTask->next = tempCurr;
			tempCurr->prev = newTask;			
		}
		else {	
			TCBptr tempPrev = priorityHead;	
			while (tempCurr->next != NULL) {
				tempCurr = tempCurr->next;
				if (compareTCBPriority(newTask, tempCurr)) {
					newTask->prev = tempPrev;
					newTask->next = tempCurr;
					tempCurr->prev = newTask;
					break;
				}
				else {
					tempPrev = tempCurr;
				}
			}
		}	
	}
}

// compares the TCB priorities of the TCB wanting to insert, to the TCB of the one looking at
// returns true if TCB of the one wanting to insert is less (higher priority) than
// the one looking at
int compareTCBPriority(TCBptr new, TCBptr comparingTo) {
	if (new->priority < comparingTo->priority) {
		return TRUE;
	}
	return FALSE;
}

void YKRun(void){
	YKScheduler();	//calls the scheduler to begin kernel
}


void YKScheduler(void){
	//traverse through linked list to find any task

}
void YKDispatcher(TCBptr lower,TCBptr higher);

