#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "Queue.h"
#include <stdio.h> //


#define CLONE_FLAG ( CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | SIGCHLD )
#define STACK_SIZE ( 1024 * 128 ) //

int thread_create(thread_t *thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{                 //thread is ThreadTblEnt's index
    Thread *tcb;
    pid_t pid;
    void* stack;
    int i;
    tcb = (Thread*)malloc(sizeof(Thread));
    stack = malloc(STACK_SIZE);
    if(stack == 0){
        perror("malloc error");
        exit(1);
    }

    pid = clone((*start_routine), (char*)stack + STACK_SIZE, CLONE_FLAG, arg);

    kill(pid, SIGSTOP);
    printf("Thread Create pid[%d]\n", pid);

    tcb->stackSize = STACK_SIZE;
    tcb->stackAddr = stack;
    tcb->exitCode = 0;///////???
    tcb->pid = pid;
    tcb->priority = priority;
    tcb->phNext = NULL;
    tcb->phPrev = NULL;

    //Insert to Thread table
    for(i = 0; i < MAX_THREAD_NUM; i++){
        if(pThreadTbEnt[i].bUsed == 0){ //Find empty entry
            pThreadTbEnt[i].bUsed = 1;
            *thread = i;                //return thread entry number
            pThreadTbEnt[i].pThread = tcb;
            break;
        }
        if(i == MAX_THREAD_NUM){        //If thread entry is full
            perror("Thread is full!\n");
            return 1;
        }
    }

    if(pCurrentThead == NULL)            //Not Run
        InsertToReadyQueue(tcb);
    else{
        if(pCurrentThead->priority <= tcb->priority){  // Priority : running <= created
            InsertToReadyQueue(tcb);         //Insert to ReadyQueue
        }
        else{                            // Run , Priority : running > created
            InsertToReadyQueue(pCurrentThead);

            __ContextSwitch(pCurrentThead->pid, tcb->pid); //Context Switch
            tcb->status = THREAD_STATUS_RUN;
            pCurrentThead = tcb;
        }
    }
    return 0;
}

int thread_suspend(thread_t tid)
{

}

int thread_cancel(thread_t tid) //thread id
{
    Thread* curThread;
    kill(tid, SIGKILL);
    if(curThread->status == THREAD_STATUS_READY){

    }
    else if(curThread->status == THREAD_STATUS_WAIT){

    }
}

int thread_resume(thread_t tid)
{

}

thread_t thread_self()
{
    pid_t pid = getpid();
    thread_t thread_id = TableSearch_Thread(pid);
    if(thread_id == -1){
        perror("thread compare error");
        return -1;
    }

    if(pid == pThreadTbEnt[thread_id].pThread->pid){
        return thread_id;
    }
    return 0;
}

int thread_join(thread_t tid, void** retval){

}

int thread_exit(void* retval){
    
}