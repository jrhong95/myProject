#include "hw1.h"

void PrintHashTable();
void PrintFreeList();
void Print();
void InitFreeList();

/* int main(){
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
    Print(); 
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
} */
int main(){
    Object* pObj = NULL;

    Init();
    InitFreeList();
    PrintFreeList();

    pObj = GetObjectFromObjFreeList();
    pObj->objnum = 10;
    InsertObjectToTail(pObj, 10);
    PrintHashTable();

    PrintFreeList();

    return 0;
}


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
            //printf("   total: %d  {Head: %d Tail: %d}\n", pHashTableEnt[i].elmtCount, pHashTableEnt[i].pHead->objnum, pHashTableEnt[i].pTail->objnum);
            printf("   total: %d  {Head: %d Tail: %d} Head: 0x%p Tail: 0x%p\n", pHashTableEnt[i].elmtCount, pHashTableEnt[i].pHead->objnum, pHashTableEnt[i].pTail->objnum, pHashTableEnt[i].pHead, pHashTableEnt[i].pTail);
        }
        else
            printf("   total: %d  {Head: NULL Tail: NULL}\n", pHashTableEnt[i].elmtCount);
        
    }
}

void PrintFreeList(){
    Object* cur = pFreeListHead;
    while(cur != NULL){
        printf("[%d]", cur->objnum);
        cur = cur->phNext;
        if(cur != NULL)
            printf(" - ");
    }

    printf("\n");
    printf("Head: 0x%p Tail: 0x%p\n=====================\n\n", pFreeListHead, pFreeListTail);
}

void Print(){
    PrintHashTable();
    PrintFreeList();
}

void InitFreeList() {
	int i;
	Object* pObject;
	for (i = 0; i < 20; i++) {
		pObject = (Object*)malloc(sizeof(Object));
		pObject->objnum = i;
		InsertObjectIntoObjFreeList(pObject);
	}
}

