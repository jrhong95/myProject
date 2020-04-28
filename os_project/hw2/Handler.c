#include "Handler.h"
/*
TODO: Thread가 죽으면 어떻게 할 지 handler
      
*/
int alarmc = 0;
void RR_handler(int sig)
{
  //printf("Alarm! %d\n", ++alarmc);
  if (!IsReadyQueueEmpty())
  {
    Thread *cur = pCurrentThead;
    Thread *next = FindNextinReadyQueue();
    //printf("%d\n", cur->pid);
    cur->status = THREAD_STATUS_READY;
    next->status = THREAD_STATUS_RUN;     //next thread status change
    __ContextSwitch(cur->pid, next->pid); //context switch
    InsertToReadyQueue(cur);
    pCurrentThead = next;
  }
  alarm(TIMESLICE);
}

void hand1(int sig)
{
  printf("handler call\n");
  alarm(TIMESLICE);
}