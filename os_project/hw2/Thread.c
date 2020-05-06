#include "Thread.h"
#include "Scheduler.h"
#include "Queue.h"
#include <stdio.h> //


#define CLONE_FLAG ( CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | SIGCHLD )
#define STACK_SIZE ( 1024 * 128 ) //

int flag;

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

    pid = clone((*start_routine), (char*)stack + STACK_SIZE, CLONE_FLAG, arg);

    kill(pid, SIGSTOP);
    
    tcb->stackSize = STACK_SIZE;
    tcb->stackAddr = stack;
    tcb->exitCode = 0;///////???
    tcb->pid = pid;
    tcb->priority = priority;
    tcb->phNext = NULL;
    tcb->phPrev = NULL;

    //Insert to Thread table
    for(i = 0; i < MAX_THREAD_NUM; i++){
        if(pThreadTblEnt[i].bUsed == 0){ //Find empty entry
            pThreadTblEnt[i].bUsed = 1;
            *thread = i;                //return thread entry number
            printf("Thread Create tid[%d] || pid[%d]\n", *thread, pid);
            pThreadTblEnt[i].pThread = tcb;
            break;
        }
        if(i == MAX_THREAD_NUM){        //If thread entry is full
            perror("Thread is full!\n");
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

int thread_suspend(thread_t tid)
{
    Thread* targetThread = pThreadTblEnt[tid].pThread;
    if(targetThread->status == THREAD_STATUS_READY){     //In ready Q
        DeleleToReadyQueue(targetThread->priority, targetThread->pid);
        InsertToWaitQueue(targetThread);
    }
    else if(targetThread->status == THREAD_STATUS_WAIT){ //In wait Q
        DeleteToWaitQueue(targetThread->pid);
        InsertToWaitQueue(targetThread);
    }
    else{
        printf("pid's status is %d\n", targetThread->status);
        return -1;
    }
    return 0;
}

int thread_cancel(thread_t tid) //thread id
{
    Thread* targetThread = pThreadTblEnt[tid].pThread;
    kill(targetThread->pid, SIGKILL);
    if(targetThread->status == THREAD_STATUS_READY)     //In ready Q
        DeleleToReadyQueue(targetThread->priority, targetThread->pid);
    else if(targetThread->status == THREAD_STATUS_WAIT) //In wait Q
        DeleteToWaitQueue(targetThread->pid);
    else{
        printf("pid's status is %d\n", targetThread->status);
        return -1;
    }
    pThreadTblEnt[tid].bUsed = 0;
    pThreadTblEnt[tid].pThread = NULL;

    free(targetThread->stackAddr);
    free(targetThread);

    return 0;
}

int thread_resume(thread_t tid)
{
    Thread* targetThread = pThreadTblEnt[tid].pThread;

    if(pCurrentThread == NULL){
        DeleteToWaitQueue(targetThread->pid);
        targetThread->status = THREAD_STATUS_RUN;

        return 0;
    }
    if(targetThread->priority < pCurrentThread->priority){  //context switching running to target
        InsertToReadyQueue(pCurrentThread);
        pCurrentThread->status = THREAD_STATUS_READY;

        DeleteToWaitQueue(targetThread->pid);
        targetThread->status = THREAD_STATUS_RUN;

        __ContextSwitch(pCurrentThread->pid, targetThread->pid);
        return 0;
    }
    else{                                                   // target move waitQ to readyQ
        targetThread->status = THREAD_STATUS_READY;
        DeleteToWaitQueue(targetThread->pid);
        InsertToReadyQueue(targetThread);

        return 0;
    }

    return -1;
}

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
int join_handler(int sig){
    flag = 1;
    //printf("join hanlder %d!\n", getpid());
}

/* int thread_join(thread_t tid, void** retval){           //wait parent until child exit
    Thread* child = pThreadTblEnt[tid].pThread;
    Thread* parent = pThreadTblEnt[TableSearch_Thread(getpid())].pThread;
    int exitCode = child->exitCode;

    struct sigaction act;
    act.sa_handler = join_handler;
    act.sa_flags = SA_NOCLDSTOP;

    //printf("parent: %d\nchild: %d\n", parent->pid, child->pid);

    if(child->status != THREAD_STATUS_ZOMBIE){          //child is not zombie
        
        if(parent->status == THREAD_STATUS_READY){
            // parent In ready Q
            DeleleToReadyQueue(parent->priority, parent->pid);
            //printf("[Join] Parent Ready\n");
            pCurrentThread = FindNextinReadyQueue();
            InsertToWaitQueue(parent);                  //parent move to waitQ
            parent->status = THREAD_STATUS_WAIT;
        }
        else if(parent->status == THREAD_STATUS_RUN){
            //printf("[Join] Parent RUN\n");
            pCurrentThread = NULL;
            //printf("Join================\n");
            //PrintReadyQueue();
            InsertToWaitQueue(parent);                  //parent move to waitQ
            parent->status = THREAD_STATUS_WAIT;
            //printf("Parent move to waitQ================\n");
            //PrintReadyQueue();
        }

        flag = 0;
        sigaction (SIGCHLD, &act, NULL);
        while(!flag);                                   //parent wait until get SIGCHLD
        flag = 0;
        //printf("while finish\n");
        DeleteToWaitQueue(parent->pid);
        //printf("move to ready\n");
        InsertToReadyQueue(parent);
        parent->status = THREAD_STATUS_READY;
    }

    *retval = &exitCode;
    DeleteToWaitQueue(child->pid);

    pThreadTblEnt[tid].bUsed = 0;
    pThreadTblEnt[tid].pThread = NULL;

    free(child->stackAddr);
    free(child);
    
    return 0;
} */

int thread_join(thread_t tid, void** retval){
    Thread* child = pThreadTblEnt[tid].pThread;
    Thread* parent = pThreadTblEnt[TableSearch_Thread(getpid())].pThread;
    int exitCode = child->exitCode;

    struct sigaction act;
    act.sa_handler = join_handler;
    act.sa_flags = SA_NOCLDSTOP;

    if(child->status != THREAD_STATUS_ZOMBIE){          //child is not zombie
        if(parent->status == THREAD_STATUS_READY){
            // parent In ready Q
            DeleleToReadyQueue(parent->priority, parent->pid);
            printf("[Join] Parent Ready\n");
            pCurrentThread = FindNextinReadyQueue();
            InsertToWaitQueue(parent);                  //parent move to waitQ
            parent->status = THREAD_STATUS_WAIT;
        }
        else if(parent->status == THREAD_STATUS_RUN){
            printf("[Join] Parent RUN\n");
            pCurrentThread = NULL;
            //printf("Join================\n");
            //PrintReadyQueue();
            InsertToWaitQueue(parent);                  //parent move to waitQ
            parent->status = THREAD_STATUS_WAIT;
            //printf("Parent move to waitQ================\n");
            //PrintReadyQueue();
        }

        if(IsReadyQueueEmpty()){
            pCurrentThread = NULL;
        }
        else{
            pCurrentThread = FindNextinReadyQueue();
            pCurrentThread->status = THREAD_STATUS_RUN;

            kill(pCurrentThread->pid, SIGCONT);
            flag = 0;
            sigaction (SIGCHLD, &act, NULL);
            while(!flag);                       //parent wait until get SIGCHLD

            DeleteToWaitQueue(parent->pid);
            InsertToReadyQueue(parent);
            parent->status = THREAD_STATUS_READY;
        }
    }

    *retval = &exitCode;
    DeleteToWaitQueue(child->pid);

    pThreadTblEnt[tid].bUsed = 0;
    pThreadTblEnt[tid].pThread = NULL;

    free(child->stackAddr);
    free(child);
    
    return 0;
}


int thread_exit(void* retval){
    Thread* target = pCurrentThread;
    target->exitCode = *((int*)retval);
    //printf("target->pid = %d, exit[%d]\n", target->pid, target->exitCode);
    InsertToWaitQueue(target);
    target->status = THREAD_STATUS_ZOMBIE;
    //printf("Thread[%d] is exit\n", TableSearch_Thread(getpid()));

    if(IsReadyQueueEmpty()){
        pCurrentThread = NULL;
    }
    else{
        pCurrentThread = FindNextinReadyQueue();
        pCurrentThread->status = THREAD_STATUS_RUN;
        kill(pCurrentThread->pid, SIGCONT);
    }
    exit(0);
    return 0;
}