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

Thread* DeleleToReadyQueue(int priority, pid_t pid){ //pid == 0, Delete Head
                                                     //pid != 0, Delete pid
    if(pReadyQueueEnt[priority].pHead == NULL){ //Head is null
        printf("Priority: [%d] is Empty\n", priority);
        return NULL;
    }
    if(pid != 0){   //Delete pid tcb
        Thread* cur = pReadyQueueEnt[priority].pHead;
        while(cur != NULL){
            if(cur->pid == pid){
                if(pReadyQueueEnt[priority].pHead == pReadyQueueEnt[priority].pTail){    
                    // 1 Node in ReadyQueue
                    pReadyQueueEnt[priority].pHead = NULL;
                    pReadyQueueEnt[priority].pTail = NULL;
                    pReadyQueueEnt[priority].queueCount--;
                    return cur;
                }

                if(cur == pReadyQueueEnt[priority].pHead){
                    pReadyQueueEnt[priority].pHead = cur->phNext;
                    pReadyQueueEnt[priority].pHead->phPrev = NULL;
                    cur->phNext = NULL;
                    pReadyQueueEnt[priority].queueCount--;
                    return cur;
                }
                else if(cur == pReadyQueueEnt[priority].pTail){
                    pReadyQueueEnt[priority].pTail = cur->phPrev;
                    pReadyQueueEnt[priority].pTail->phNext = NULL;
                    cur->phPrev = NULL;
                    pReadyQueueEnt[priority].queueCount--;
                    return cur;
                }
                else{
                    cur->phNext->phPrev = cur->phPrev;
                    cur->phPrev->phNext = cur->phNext;
                    pReadyQueueEnt[priority].queueCount--;
                    return cur;
                }
            }
            cur = cur->phNext;
        }
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
        tcb->status = THREAD_STATUS_WAIT;
    }
    else{//insert to Tail
        tcb->phPrev = pWaitingQueueTail;
        pWaitingQueueTail->phNext = tcb;
        pWaitingQueueTail = tcb;
        tcb->status = THREAD_STATUS_WAIT;
    }
}
Thread* DeleteToWaitQueue(pid_t pid){
    Thread* cur = pWaitingQueueHead;

    if((pWaitingQueueHead == NULL) && (pWaitingQueueTail == NULL)){
        return NULL;
    }

    while(cur != NULL){
        if(cur->pid == pid){
            if(pWaitingQueueHead == pWaitingQueueTail){    // 1 Node in WaitQ
                pWaitingQueueHead = NULL;
                pWaitingQueueTail = NULL;
                return cur;
            }

            if(cur == pWaitingQueueHead){
                pWaitingQueueHead = cur->phNext;
                pWaitingQueueHead->phPrev = NULL;
                cur->phNext = NULL;
                
                return cur;
            }
            else if(cur == pWaitingQueueTail){
                pWaitingQueueTail = cur->phPrev;
                pWaitingQueueTail->phNext = NULL;
                cur->phPrev = NULL;

                return cur;
            }
            else{
                cur->phNext->phPrev = cur->phPrev;
                cur->phPrev->phNext = cur->phNext;
                return cur;
            }
        }
        cur = cur->phNext;
    }

    return NULL;
}
Thread* DeleteToWaitQueueHead(){
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
        Thread* temp = pWaitingQueueHead;
        pWaitingQueueHead = pWaitingQueueHead->phNext;
        temp->phNext->phPrev = NULL;
        temp->phNext = NULL;

        return temp;
    }
}

int IsReadyQueueEmpty(){ //not empty: 0, empty: 1
    int i;
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        if(pReadyQueueEnt[i].queueCount != 0){
            //printf("Not Empty\n");
            return 0;
        }
    }
    //printf("Empty\n");
    return 1;
}

Thread* FindNextinReadyQueue(){
    int i;
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        if(pReadyQueueEnt[i].pHead != NULL){
            return DeleleToReadyQueue(i, 0);
        }
    }
    return NULL;
}

thread_t TableSearch_Thread(pid_t pid)		//tid search in TableEntry
{
    int i;
	for(i = 0; i < MAX_THREAD_NUM; i++){
        if(pThreadTblEnt[i].pThread != NULL && pThreadTblEnt[i].pThread->pid == pid){
            return i;
        }  
    }
	return -1;
}

void PrintReadyQueue(){
    int i, j;
    if(pCurrentThread !=NULL)
        printf("=====================================\n[print] Current Running Thread : %d[%d]\n", pCurrentThread->pid, TableSearch_Thread(pCurrentThread->pid));
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        printf("index: %d [%d]|", i, pReadyQueueEnt[i].queueCount);

        Thread* temp= pReadyQueueEnt[i].pHead;
        for(j = 0 ;j < pReadyQueueEnt[i].queueCount; j++){
            printf(" %d[%d]",temp->pid, TableSearch_Thread(temp->pid));
            temp = temp->phNext;
        }
        printf("\n");
    }
    printf("\n");
}

void PrintWaitQueue(){
    Thread* cur = pWaitingQueueHead;
    printf("\n[print] Waiting Queue : ");
    while(cur != NULL){
        printf("%d[%d] status: %d |", cur->pid, TableSearch_Thread(cur->pid), cur->status);
        cur = cur->phNext;
    }

    printf("\n==============================================\n");
}