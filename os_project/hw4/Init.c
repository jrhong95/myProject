#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "MsgQueue.h"
#include "Queue.h"
#include <stdlib.h>
#include <string.h>

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

    //MsgQueue Init
    for(i = 0; i < MAX_QCB_NUM; i++){
        qcbTblEntry[i].bUsed = 0;
        qcbTblEntry[i].mode = 0;
        qcbTblEntry[i].openCount = 0;
        qcbTblEntry[i].pQcb = NULL;
        strcpy(qcbTblEntry[i].name, "");
    }
}