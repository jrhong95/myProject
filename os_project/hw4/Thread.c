#include "Thread.h"
#include "Scheduler.h"
#include "MsgQueue.h"
#include "Queue.h"
#include <stdio.h>


#define CLONE_FLAG ( CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | SIGCHLD )

Thread* parent;
Thread* child;

int thread_create(thread_t *thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{                 //thread is pThreadTblEnt's index
    Thread *tcb;
    pid_t pid;
    void* stack;
    int i;
    tcb = (Thread*)malloc(sizeof(Thread));
    stack = malloc(STACK_SIZE);
    if(stack == 0){
        perror("malloc error");
        return -1;
    }

    pid = clone((int(*)(void*))(*start_routine), (char*)stack + STACK_SIZE, CLONE_FLAG, arg);

    kill(pid, SIGSTOP);
    
    tcb->stackSize = STACK_SIZE;
    tcb->stackAddr = stack;
    tcb->exitCode = 0;
    tcb->pid = pid;
    tcb->priority = priority;
    tcb->phNext = NULL;
    tcb->phPrev = NULL;

    //Insert to Thread table
    for(i = 0; i < MAX_THREAD_NUM; i++){
        if(pThreadTblEnt[i].bUsed == 0){ //Find empty entry
            pThreadTblEnt[i].bUsed = 1;
            *thread = i;                //return thread entry number
            //printf("Thread Create tid[%d] || pid[%d]\n", *thread, pid);
            pThreadTblEnt[i].pThread = tcb;
            break;
        }
        if(i == MAX_THREAD_NUM){        //If thread entry is full
            perror("Thread is full");
            return -1;
        }
    }

    if(pCurrentThread == NULL)            //Not Run
        InsertToReadyQueue(tcb);
    else{
        if(pCurrentThread->priority <= tcb->priority){       // Priority : running <= created
            InsertToReadyQueue(tcb);                         //Insert to ReadyQueue
        }
        else{                                                // Run , Priority : running > created
            InsertToReadyQueue(pCurrentThread);

            __ContextSwitch(pCurrentThread->pid, tcb->pid);  //Context Switch
            tcb->status = THREAD_STATUS_RUN;
            pCurrentThread = tcb;
        }
    }
    return 0;
}

// int thread_suspend(thread_t tid)
// {
//     Thread* targetThread = pThreadTblEnt[tid].pThread;
//     if(targetThread->status == THREAD_STATUS_READY){     //In ready Q
//         DeleleToReadyQueue(targetThread->priority, targetThread->pid);
//         InsertToWaitQueue(targetThread);
//     }
//     else if(targetThread->status == THREAD_STATUS_WAIT){ //In wait Q
//         DeleteToWaitQueue(targetThread->pid);
//         InsertToWaitQueue(targetThread);
//     }
//     else if(targetThread == pCurrentThread){
//         InsertToWaitQueue(targetThread);
//         return 0;
//     }
//     else
//     {
//         perror("thread_suspend Error");
//         return -1;
//     }
    
//     return 0;
// }

// int thread_cancel(thread_t tid) //thread id
// {
//     Thread* targetThread = pThreadTblEnt[tid].pThread;

//     if(targetThread == NULL) {
//         perror("thread_cancel error");
//         return -1;
//     }
//     kill(targetThread->pid, SIGKILL);
//     if(targetThread->status == THREAD_STATUS_READY)     //In ready Q
//         DeleleToReadyQueue(targetThread->priority, targetThread->pid);
//     else if(targetThread->status == THREAD_STATUS_WAIT) //In wait Q
//         DeleteToWaitQueue(targetThread->pid);
//     else if(targetThread->status == THREAD_STATUS_RUN){
//         pCurrentThread = NULL;
//     }
//     pThreadTblEnt[tid].bUsed = 0;
//     pThreadTblEnt[tid].pThread = NULL;

//     free(targetThread->stackAddr);
//     free(targetThread);

//     return 0;
// }

// int thread_resume(thread_t tid)
// {
//     Thread* targetThread = pThreadTblEnt[tid].pThread;

//     if(pCurrentThread == NULL){
//         DeleteToWaitQueue(targetThread->pid);
//         targetThread->status = THREAD_STATUS_RUN;

//         return 0;
//     }
//     if(targetThread->priority < pCurrentThread->priority){  //context switching running to target
//         Thread* cur = pCurrentThread;
 
//         InsertToReadyQueue(cur);
//         cur->status = THREAD_STATUS_READY;

//         DeleteToWaitQueue(targetThread->pid);
//         targetThread->status = THREAD_STATUS_RUN;
        
//         pCurrentThread = targetThread;
//         __ContextSwitch(cur->pid, pCurrentThread->pid);
//         return 0;
//     }
//     else{                                                   // target move waitQ to readyQ
//         targetThread->status = THREAD_STATUS_READY;
//         DeleteToWaitQueue(targetThread->pid);
//         InsertToReadyQueue(targetThread);

//         return 0;
//     }
//     perror("thread_resume error");
//     return -1;
// }

thread_t thread_self()
{
    pid_t pid = getpid();
    thread_t thread_id = TableSearch_Thread(pid);
    if(thread_id == -1){
        perror("thread compare error");
        return -1;
    }

    if(pid == pThreadTblEnt[thread_id].pThread->pid){
        return thread_id;
    }

    return 0;
}
// int join_handler(int sig){
// }

// int thread_join(thread_t tid, void** retval)
// {
//     parent = pThreadTblEnt[TableSearch_Thread(getpid())].pThread;
//     child = pThreadTblEnt[tid].pThread;
//     int exitCode;

//     struct sigaction act;
//     act.sa_handler = (void*)join_handler;
//     act.sa_flags = SA_NOCLDSTOP;

//     if(child->status != THREAD_STATUS_ZOMBIE){          //child is not zombie

//         if(parent->status == THREAD_STATUS_RUN){
//             pCurrentThread = NULL;
//             InsertToWaitQueue(parent);                  //parent move to waitQ
//             parent->status = THREAD_STATUS_WAIT;
//         }
//         else{
//             return -1;
//         }

//         if(IsReadyQueueEmpty()){
//             pCurrentThread = NULL;
//         }
//         else{
//             pCurrentThread = FindNextinReadyQueue();
//             kill(pCurrentThread->pid, SIGCONT);
            
//             sigaction (SIGCHLD, &act, NULL);
//             pause();                       //parent wait until get SIGCHLD
        
//             DeleteToWaitQueue(parent->pid);
//             InsertToReadyQueue(parent);
//             parent->status = THREAD_STATUS_READY;
//             kill(parent->pid, SIGSTOP);
//         }
//     }

//     exitCode = child->exitCode;
//     *retval = &exitCode;
//     if(DeleteToWaitQueue(child->pid) == NULL){
//         perror("Delete Child Error");
//         return -1;
//     }

//     pThreadTblEnt[tid].bUsed = 0;               //Delete Thread table node
//     pThreadTblEnt[tid].pThread = NULL;

//     free(child->stackAddr);                     //free memory
//     free(child);
//     return 0;
// }


// int thread_exit(void* retval){
//     Thread* target = pThreadTblEnt[TableSearch_Thread(getpid())].pThread;

//     if(target == NULL)
//         return -1;

//     target->exitCode = *((int*)retval);
    
//     InsertToWaitQueue(target);
//     target->status = THREAD_STATUS_ZOMBIE;

//     if(IsReadyQueueEmpty()){
//         pCurrentThread = NULL;
//     }
//     else{
//         pCurrentThread = FindNextinReadyQueue();
//         pCurrentThread->status = THREAD_STATUS_RUN;
//         kill(pCurrentThread->pid, SIGCONT);
//     }
//     exit(0);
//     return 0;
// }