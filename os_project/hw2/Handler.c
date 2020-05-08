#include "Handler.h"
#include "Queue.h"

/* void handler(int sig){
    if(pCurrentThread != NULL){
        Thread *next = FindNextinReadyQueue();
        //printf("Cur not empty\n");
        if(pCurrentThread->status == THREAD_STATUS_ZOMBIE){ //스레드가 종료되었을 때
            if(next != NULL){
                next->status = THREAD_STATUS_RUN;
                pCurrentThread = next;
                kill(pCurrentThread->pid, SIGCONT);
                PrintReadyQueue();
                PrintWaitQueue(); 
            }
            else//readyQ is Empty
                pCurrentThread = NULL;
        }
        else{//thread is not exit
            InsertToReadyQueue(pCurrentThread);
            pCurrentThread->status = THREAD_STATUS_READY;
            
            next->status = THREAD_STATUS_RUN;     //next thread status change
            __ContextSwitch(pCurrentThread->pid, next->pid); //context switch
            pCurrentThread = next;
        }
    }
    else if(pCurrentThread == NULL && !IsReadyQueueEmpty()){  //레디큐에서 cpu로 스레드가 넘어오지 않았을 때
        pCurrentThread = FindNextinReadyQueue();
        //printf("[Handler] CurrentThread is %d\n", pCurrentThread->pid);
        pCurrentThread->status = THREAD_STATUS_RUN;
        kill(pCurrentThread->pid, SIGCONT);
        PrintReadyQueue();
        PrintWaitQueue();
    }
    alarm(TIMESLICE);
} */

void RR_handler(int sig){
    if(!IsReadyQueueEmpty()){ //ReadyQ not empty
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
    alarm(TIMESLICE);
}