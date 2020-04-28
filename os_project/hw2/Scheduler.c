#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Handler.h"

int RunScheduler( void )
{
    if(pCurrentThead == NULL){
        pCurrentThead = FindNextinReadyQueue();
        pCurrentThead->status = THREAD_STATUS_RUN;
    }
    printf("Current Running Thread : %d\n", pCurrentThead->pid);
    kill(pCurrentThead->pid, SIGCONT);
    //PrintReadyQueue();
    signal(SIGALRM, (void*)RR_handler);
	alarm(TIMESLICE);
    while(1);
}


void __ContextSwitch(int curpid, int newpid)
{
    kill(curpid, SIGSTOP);
    kill(newpid, SIGCONT);
}
