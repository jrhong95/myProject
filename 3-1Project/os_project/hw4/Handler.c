#include "Handler.h" 

void RR_handler(int sig){
    if(!IsReadyQueueEmpty() || pCurrentThread != NULL){ //ReadyQ not empty
        Thread* cur;
        Thread* next;

        if(pCurrentThread == NULL){
            cur = FindNextinReadyQueue();   //delete readyQ head
            pCurrentThread = cur;
            next = cur;
            cur->status = THREAD_STATUS_RUN;
        }
        else{
            cur = pCurrentThread;
            InsertToReadyQueue(cur);
            cur->status = THREAD_STATUS_READY;
            next = FindNextinReadyQueue();
            pCurrentThread = next;
            pCurrentThread->status = THREAD_STATUS_RUN;
        }
        //printf("cur: %d || next: %d\n", cur->pid, next->pid);
        __ContextSwitch(cur->pid, next->pid);
        //PrintReadyQueue();
    }
    //printf("Alarm!\n");
    alarm(TIMESLICE);
}