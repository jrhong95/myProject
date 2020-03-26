#include "hw1.h"

void PrintHashTable(){
    int i;
    Object* cur;
    for(i = 0; i < HASH_TBL_SIZE; i++){
        printf("HashTable[%d] :", i);
        if(pHashTableEnt[i].pHead != NULL){
            cur = pHashTableEnt[i].pHead;
            while(cur != NULL){
                printf("%d ", cur->objnum);
                cur = cur->phNext;
            }
            printf("   total: %d  {Head: %d Tail: %d}\n", pHashTableEnt[i].elmtCount, pHashTableEnt[i].pHead->objnum, pHashTableEnt[i].pTail->objnum);
        }
        else
            printf("   total: %d  {Head: NULL Tail: NULL}\n", pHashTableEnt[i].elmtCount);
        
    }
}

int main(){
    int i, num = 0;
    Object* pObj;

    printf("Main start\n");
    Init();
    
    for(i = 0; i <  20; i++){
        pObj = (Object*)malloc(sizeof(Object));
        pObj->objnum = i;

        //printf("%d ", i);
        if(i % 2 == 0)
            InsertObjectToHead(pObj, pObj->objnum);
        else
            InsertObjectToTail(pObj, pObj->objnum);
        
    }
    PrintHashTable(); 
    printf("==================================\n");
    while(num != -1){
        printf("Delete Number?? ");
        scanf("%d", &num);

        if(GetObjectByNum(num) != NULL){
            DeleteObject(GetObjectByNum(num));
        }
        PrintHashTable();
        printf("==================================\n");
    }
    return 0;
}