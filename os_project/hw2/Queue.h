#ifndef __QUEUE_H__
#define __QUEUE_H__

#define _GNU_SOURCE

#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"

void InsertToReadyQueue(Thread *tcb);
Thread* DeleleToReadyQueue(int priority);

void InsertToWaitQueue(Thread *tcb);
Thread* DeleteToWaitQueue();

int IsReadyQueueEmpty();
Thread* FindNextinReadyQueue();

thread_t TableSearch_tid(pid_t pid);
#endif /* __QUEUE_H__ */
