#define NULL 0
#define TRUE 1
#define FALSE 0

enum taskState{
	blocked,		//state for task being block
	ready,			//state for task being ready
	running,		//state for task being running
	interrupted		//state for task being interrupted
};

extern YKCtxSwCount;		//context switch counter 
extern YKIdleCount;		//idelcount counter

typedef struct taskblock* TCBptr;
typedef struct taskblock {
	int taskID;
	void *stackptr;		//pointer to current stack top
	enum taskState state;			//current state of TCB
	unsigned char priority;		//current priority
	int delay;			//# of ticks yet to wait
	TCBptr next;		//forward ptr for linked list
	TCBptr prev;		//backward ptr for linked list
} TCB;


void YKInitialize(void);
void YKEnterMutex(void);
void YKExitMutex(void);
void YKIdleTask(void);
void YKNewTask(void(*task(void)),void *taskStack,unsigned char priority);
void YKRun(void);
void YKScheduler(void);
void YKDispatcher(int savecontext,TCBptr higher);
