#ifndef __QUEUE_H__
#define __QUEUE_H__

#define _GNU_SOURCE

#include <stdio.h>
#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "MsgQueue.h"

void InsertToReadyQueue(Thread *tcb);
Thread* DeleleToReadyQueue(int priority, pid_t pid);

void InsertToWaitQueue(Thread *tcb);
void InsertToQcbWaitQueue(int mqd, Thread *tcb);
Thread* DeleteToWaitQueueHead(int mqd);
Thread* DeleteToWaitQueue(pid_t pid);

int IsReadyQueueEmpty();
Thread* FindNextinReadyQueue();

thread_t TableSearch_tid(pid_t pid);
void PrintReadyQueue();
void PrintWaitQueue();
#endif /* __QUEUE_H__ */
