#include "Thread.h"
#include "Scheduler.h"
#include "MsgQueue.h"
#include "Handler.h"
#include "Queue.h"

int RunScheduler( void )
{
    if(pCurrentThread == NULL){
        pCurrentThread = FindNextinReadyQueue();
        pCurrentThread->status = THREAD_STATUS_RUN;
    }
    //printf("[Scheduler] Current Running Thread : %d\n", pCurrentThread->pid);
    kill(pCurrentThread->pid, SIGCONT);

    signal(SIGALRM, (void*)RR_handler);
	alarm(TIMESLICE);
    while(1);
}


void __ContextSwitch(int curpid, int newpid)
{
    kill(curpid, SIGSTOP);
    kill(newpid, SIGCONT);
}
