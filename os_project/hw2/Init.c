#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

void Init(void)
{
    int i;
    // pThreadEnt Reset
    for(i = 0; i < MAX_THREAD_NUM; i++){
        pThreadTblEnt[i].bUsed = 0;
        pThreadTblEnt[i].pThread = NULL;
    }

    //ReadyQueue Reset
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        pReadyQueueEnt[i].queueCount = 0;
        pReadyQueueEnt[i].pHead = NULL;
        pReadyQueueEnt[i].pTail = NULL;
        
    }
    //Waiting Queue Reset
    pWaitingQueueHead = NULL;
    pWaitingQueueTail = NULL;

    pCurrentThread = NULL;
}