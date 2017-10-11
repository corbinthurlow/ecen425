#include "yakk.h"
#include "yaku.h"
#include "clib.h"

int YKCtxSwCount = NULL;
int YKIdleCount = NULL;
TCBptr priorityHead = NULL;
TCBptr RdyTask = NULL;
TCBptr YKCurrTask;
TCBptr YKRdyList;
TCBptr YKSuspList;
TCBptr YKAvailTCBList;
int isYKRunCalled = FALSE;
int stackpointer;
int oldStackPointer;

int YKIdleStk[MAX_IDLE_STACK_SIZE]; 

int compareTCBPriority(TCBptr new, TCBptr comparingTo);
void insertTCB(TCBptr newTask);
TCBptr getHighestPriorityTask(enum taskState state);

TCB YKTCBArray[MAX_TASKS+1];


void YKInitialize(void) {
	int i;
	
	YKAvailTCBList = &(YKTCBArray[0]);
	for (i = 0; i < MAX_TASKS; i++)
		YKTCBArray[i].next = &(YKTCBArray[i+1]);
	YKTCBArray[MAX_TASKS].next = NULL;
	
	
	YKCtxSwCount = 0;
	YKIdleCount = 0;
//	YKTickNum = 0;
	
	YKRdyList = NULL;
	YKSuspList = NULL;
	YKCurrTask = NULL;
	YKNewTask(YKIdleTask, (void *)&YKIdleStk[MAX_IDLE_STACK_SIZE], IDLE_TASK_PRIORITY_VALUE);
}

void YKEnterMutex(void);

void YKExitMutex(void);

void YKIdleTask(void){
	int i;
	while(1) {
		YKEnterMutex();
		for (i = 0; i < 32767; i++) {
			 printString("still inside idle task...\n"); // print it often to see if in idle task.
		}
		YKIdleCount++;
		YKExitMutex();
	}


}

void YKNewTask(void(*task(void)),void *taskStack,unsigned char priority) {
	TCBptr runningPtr = YKAvailTCBList;
	//YKEnterMutex();
	YKAvailTCBList->taskID = task;
	YKAvailTCBList->stackptr = &taskStack;
	YKAvailTCBList->state = ready;
	YKAvailTCBList->priority = priority;
	YKAvailTCBList->delay = 0;
	YKAvailTCBList->next = NULL;
	YKAvailTCBList->prev = NULL;
	runningPtr = YKAvailTCBList;
	YKAvailTCBList = runningPtr->next;
	printString("Inserting new task....\n");
	insertTCB(runningPtr);
	if (isYKRunCalled) {
		YKScheduler();	//calls the scheduler to begin kernel		
	}
	//YKExitMutex();
}

void insertTCB(TCBptr newTask) {
		if (priorityHead == NULL) { // if the very first insertion
		priorityHead = newTask;
		printString("Inserted first task here\n");
	}
	else { 				// else need to find a place to but in double link list
		TCBptr tempCurr = priorityHead;
		if (compareTCBPriority(newTask, tempCurr)) {
			priorityHead = newTask;
			newTask->next = tempCurr;
			tempCurr->prev = newTask;
			printString("Inserted task here!!!!!!!!!!!!\n");			
		}
		else {
			TCBptr tempPrev = priorityHead;			
			printString("GOT HERE\n");			
			while (tempCurr->next != NULL) {
			printString("Entered the while here\n");
				tempCurr = tempCurr->next;
				if (compareTCBPriority(newTask, tempCurr)) {
					newTask->prev = tempPrev;
					newTask->next = tempCurr;
					tempCurr->prev = newTask;
					printString("Inserted task\n");
					break;
				}
				else {
					tempPrev = tempCurr;
				}
			}
		}	
	}
}

TCBptr getHighestPriorityTask(enum taskState state){
	printString("Searching for tasks\n");
	if (priorityHead == NULL) { // if the head is null no tasks are found 
		return NULL;
	}
	else { 				// else need to find highest ready task
		TCBptr tempCurr = priorityHead;;	
			while (tempCurr->state != state && tempCurr->next != NULL) {
				tempCurr = tempCurr->next;
			}
			return tempCurr;
		}	
}



// compares the TCB priorities of the TCB wanting to insert, to the TCB of the one looking at
// returns true if TCB of the one wanting to insert is less (higher priority) than
// the one looking at
int compareTCBPriority(TCBptr new, TCBptr comparingTo) {
	if ((new->priority) < (comparingTo->priority)) {
		return TRUE;
	}	
	return FALSE;
}

void YKRun(void){	
	isYKRunCalled = TRUE;
	YKScheduler();	//calls the scheduler to begin kernel
}


void YKScheduler(void){
	//default set to 1 for now
	int saveContext = 1;
	TCBptr oldTask;
	TCBptr nextTask = getHighestPriorityTask(ready);
	oldTask = YKCurrTask;
	YKCurrTask = nextTask;	
	if(YKCurrTask != NULL && nextTask->priority == YKCurrTask->priority){
		//do nothing since the highest available task is running
		return;
	}
	//we will need to check interrup nestling level here.. not sure in this lab though
	//if the nesting level is greater than zero we won't save context with the dispatcher	
	YKCtxSwCount++;
	if(oldTask != NULL)
		oldTask->state = ready;

	YKCurrTask->state = running;
	RdyTask = YKCurrTask;
	//printInt(RdyTask->priority);
	//printString("Dispatching task\n");
	YKDispatcher(saveContext,oldTask);

}










