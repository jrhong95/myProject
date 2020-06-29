#include "MsgQueue.h"
#include "Queue.h"
#include "Thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pmqd_t pmq_open(const char* name, int flags, mode_t perm, pmq_attr* attr)
{
    int i = 0, curIndex = -1;
    Qcb *curQcb;

    while(i < MAX_QCB_NUM){
        //비어있는 인덱스 탐색
        if(curIndex < 0 && qcbTblEntry[i].bUsed == 0){
            curIndex = i;
        }
        // 이미 열려있는 경우
        else if(strcmp(qcbTblEntry[i].name, name) == 0){
            qcbTblEntry[i].openCount ++;
            return i;
        }
        i++;
    }
    //비어있는 인덱스가 없는 경우
    if(curIndex < 0){
        perror("Qcb Table is full");
        return -1;
    }

    curQcb = (Qcb *)malloc(sizeof(Qcb));//Qcb초기화 
    curQcb->msgCount = 0;
    curQcb->pMsgHead = NULL;
    curQcb->pMsgTail = NULL;
    curQcb->pWaitQHead = NULL;
    curQcb->pWaitQTail = NULL;
    curQcb->waitThreadCount = 0;

    qcbTblEntry[curIndex].bUsed = 1;
    strcpy(qcbTblEntry[curIndex].name, name);
    qcbTblEntry[curIndex].mode = perm;
    qcbTblEntry[curIndex].openCount++;
    qcbTblEntry[curIndex].pQcb = curQcb;

    return curIndex;
}

int pmq_close(pmqd_t mqd)
{
    if(qcbTblEntry[mqd].bUsed == 0) //Qcb가 없는 경우
        return -1;
    else if(qcbTblEntry[mqd].openCount > 1){
        qcbTblEntry[mqd].openCount--;//Qcb count -1
    }
    else{                           //Qcb초기화
        qcbTblEntry[mqd].openCount = 0;
        free(qcbTblEntry[mqd].pQcb);
        qcbTblEntry[mqd].pQcb = NULL;
        strcpy(qcbTblEntry[mqd].name, "");
        qcbTblEntry[mqd].mode = 0;
        qcbTblEntry[mqd].bUsed = 0;
    }
    return 0;
}
int pmq_send(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int msg_prio)
{
    Message *newMsg;
    Message *msgListHead, *msgListTail, *curMsg, *prevMsg;

    if(qcbTblEntry[mqd].bUsed == 0){
        perror("Can't open MessageQueue");
        return -1;
    }

    //Copy to new Message obj 
    newMsg = (Message *)malloc(sizeof(Message));
    strcpy(newMsg->data, msg_ptr);
    newMsg->priority = msg_prio;
    newMsg->size = msg_len;
    newMsg->pNext = NULL;
    newMsg->pPrev = NULL;

    //insert to Qcb's msg list
    qcbTblEntry[mqd].pQcb->msgCount++;
    if(qcbTblEntry[mqd].pQcb->pMsgHead == NULL){    //MsgList is empty
        qcbTblEntry[mqd].pQcb->pMsgHead = newMsg;
        qcbTblEntry[mqd].pQcb->pMsgTail = newMsg;
    }
    else{
        curMsg = qcbTblEntry[mqd].pQcb->pMsgTail;
        while(curMsg != NULL){
            if(curMsg->priority < newMsg->priority){
                curMsg = curMsg->pNext;
            }
            else{
                break;
            }
        }
        if(curMsg == NULL){             //newMsg is head
            curMsg = qcbTblEntry[mqd].pQcb->pMsgHead;
            newMsg->pPrev = curMsg;
            curMsg->pNext = newMsg;
            qcbTblEntry[mqd].pQcb->pMsgHead = newMsg;
        }
        else if(curMsg->pPrev == NULL){ //newMsg is tail
            newMsg->pNext = curMsg;
            curMsg->pPrev = newMsg;
            qcbTblEntry[mqd].pQcb->pMsgTail = newMsg;
        }
        else{                           //newMsg is middle in list
            curMsg->pPrev->pNext = newMsg;
            newMsg->pPrev = curMsg->pPrev;
            curMsg->pPrev = newMsg;
            newMsg->pNext = curMsg;
        }
    }

    return 0;
}
ssize_t pmq_receive(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int* msg_prio)
{
    Message *curMsg;
    pid_t tid = thread_self();
    Thread *curThread = pThreadTblEnt[tid].pThread;
    int retVal;
    if(qcbTblEntry[mqd].openCount == 0){    //msqList가 비어있는 경우
        //sigstop
        DeleleToReadyQueue(curThread->priority, curThread->pid);
        InsertToWaitQueue(mqd, curThread);
    }

    curMsg = qcbTblEntry[mqd].pQcb->pMsgHead;
    strcpy(msg_ptr, curMsg->data);
    *msg_prio = curMsg->priority;
    qcbTblEntry[mqd].openCount--;
    if(qcbTblEntry[mqd].pQcb->pMsgHead == qcbTblEntry[mqd].pQcb->pMsgTail){
        qcbTblEntry[mqd].pQcb->pMsgHead = NULL;
        qcbTblEntry[mqd].pQcb->pMsgTail = NULL;
    }
    else{
        qcbTblEntry[mqd].pQcb->pMsgHead = curMsg->pPrev;
        curMsg->pPrev = NULL;
        curMsg->pPrev->pNext = NULL;
    }
    retVal = curMsg->size;
    free(curMsg);
    return retVal;
}

void PrintMsgList(pmqd_t mqd){
    Message *curMsg = qcbTblEntry[mqd].pQcb->pMsgTail;
    printf("%d list : ", mqd);
    while(curMsg != NULL){
        printf("%d ", curMsg->priority);
        curMsg = curMsg->pNext;
    }
    printf("\n");
}
