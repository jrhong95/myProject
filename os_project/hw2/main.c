#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"

#include<stdio.h>

void foo(int arg){
	int count = 0;

	count = 5;
	while (count > 0)
	{
		/* sleep for 1 seconds */
		sleep(2);
		printf("Tc1ThreadProc: my thread id (%ld), arg is (%d)\n",thread_self(), arg);
		count--;
	}
}


int main(void)
{
    thread_t pid1, pid2, pid3, pid4, pid5, pid6;

    Init();

    thread_create(&pid1, NULL, 0, (void*)foo, (int*)1);
    thread_create(&pid2, NULL, 1, (void*)foo, (int*)2);
    thread_create(&pid3, NULL, 0, (void*)foo, (int*)3);
    thread_create(&pid4, NULL, 1, (void*)foo, (int*)4);
    thread_create(&pid5, NULL, 2, (void*)foo, (int*)5);
    thread_create(&pid6, NULL, 3, (void*)foo, (int*)6);
    //printf("%d %d %d %d %d %d\n\n", pid1, pid2, pid3, pid4, pid5, pid6);
    PrintReadyQueue();

    RunScheduler();
    return 0;
}

void PrintReadyQueue(){
    int i, j;
    
    //printf("Current Running Thread : %d\n", pCurrentThead->pid);
    for(i = 0; i < MAX_READYQUEUE_NUM; i++){
        printf("index: %d |", i);

        Thread* temp= pReadyQueueEnt[i].pHead;
        for(j = 0 ;j < pReadyQueueEnt[i].queueCount; j++){
            printf(" %d",temp->pid);
            temp = temp->phNext;
        }
        printf("\n");
    }
}