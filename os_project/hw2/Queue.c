#include "Queue.h"

void InsertToReadyQueue(Thread *ptcb){ //Insert to Tail
    int priority = ptcb->priority;
    // ReadyQueue empty
    if(pReadyQueueEnt[priority].pHead == NULL){
        pReadyQueueEnt[priority].pHead = ptcb; //insert to head
        pReadyQueueEnt[priority].pTail = ptcb; //Head and Tail same
        pReadyQueueEnt[priority].queueCount++;  //+1 count

        ptcb->phNext = NULL;
        ptcb->phPrev = NULL;
    }
    else{
        //thread next prev change
        //pTail change
        pReadyQueueEnt[priority].queueCount++;  //+1 count
        pReadyQueueEnt[priority].pTail->phNext = ptcb;
        ptcb->phPrev = pReadyQueueEnt[priority].pTail;
        ptcb->phNext = NULL;
        pReadyQueueEnt[priority].pTail = ptcb; //insert to tail
    }

    ptcb->status = THREAD_STATUS_READY;
}

Thread* DeleleToReadyQueue(int priority){ //Delete Head
    if(pReadyQueueEnt[priority].pHead == NULL){ //Head is null
        printf("Priority: [%d] is Empty\n", priority);
        return NULL;
    }

    if(pReadyQueueEnt[priority].queueCount == 1){ //ReadyQueue count is 1
        Thread* tmptcb = pReadyQueueEnt[priority].pHead;

        pReadyQueueEnt[priority].pHead = NULL;
        pReadyQueueEnt[priority].pTail = NULL;
        pReadyQueueEnt[priority].queueCount--;

        return tmptcb;
    }
    else{
        Thread* tmptcb = pReadyQueueEnt[priority].pHead;
        pReadyQueueEnt[priority].pHead = tmptcb->phNext;
        pReadyQueueEnt[priority].queueCount--;
        tmptcb->phNext->phPrev = NULL;
        tmptcb->phNext = NULL;

        return tmptcb;
    }
}

void InsertToWaitQueue(Thread *tcb){
    //If List is Empty
    if(pWaitingQueueHead == NULL){
        pWaitingQueueHead = tcb;
        pWaitingQueueTail = tcb;
        tcb->phNext = NULL;
        tcb->phPrev = NULL;
        
    }
    else{//insert to Head
        tcb->phNext = pWaitingQueueHead;
        pWaitingQueueHead->phPrev = tcb;
        pWaitingQueueHead = tcb;
    }
}

Thread* DeleteToWaitQueue(){
    //list empty
    //printf("List init\n head: %p  Tail: %p\n", pWaitingQueueHead, pWaitingQueueTail);
    if((pWaitingQueueHead == NULL) && (pWaitingQueueTail == NULL)){
        return NULL;
    }
    //In list one Thread
    if(pWaitingQueueHead == pWaitingQueueTail){
        Thread* temp = pWaitingQueueTail;
        pWaitingQueueHead = NULL;
        pWaitingQueueTail = NULL;
        return temp;
    }
    //more than 2
    else{
        Thread* temp = pWaitingQueueTail;
        pWaitingQueueTail = pWaitingQueueTail->phPrev;
        temp->phPrev->phNext = NULL;
        temp->phPrev = NULL;

        return temp;
    }
}

int IsReadyQueueEmpty(){
    int i;
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        if(pReadyQueueEnt[i].queueCount != 0)
            return 0;
    }
    return 1;
}

Thread* FindNextinReadyQueue(){
    int i;
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        if(pReadyQueueEnt[i].pHead != NULL){
            return DeleleToReadyQueue(i);
        }
    }
    return NULL;
}

thread_t TableSearch_Thread(pid_t pid)		//tid search in WaitQueue
{
    int i;
	for(i = 0; i < MAX_THREAD_NUM; i++){
        if(pThreadTbEnt[i].pThread->pid == pid){
            return i;
        }  
    }
	return -1;
}