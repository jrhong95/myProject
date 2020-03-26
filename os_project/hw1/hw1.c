#include "hw1.h"

void Init(){
    // pHashTableEnt 초기화
    int i;
    for(i = 0; i < HASH_TBL_SIZE; i++){
        pHashTableEnt[i].elmtCount = 0;
        pHashTableEnt[i].pHead = NULL;
        pHashTableEnt[i].pTail = NULL;
    }
    // Freelist 초기화
    pFreeListHead = NULL;
    pFreeListTail = NULL;
}

void InsertObjectToTail(Object* pObj, int ObjNum){
    // Object가 삽입 될 위치 저장
    int num = ObjNum % HASH_TBL_SIZE;

    // 그 위치가 비어있을 경우
    if(pHashTableEnt[num].pHead == NULL){
        pHashTableEnt[num].pHead = pObj; //insert to head
        pHashTableEnt[num].pTail = pObj; //Head and Tail same
        pHashTableEnt[num].elmtCount++;  //+1 count

        pObj->phNext = NULL;
        pObj->phPrev = NULL;
    }
    else{
        //obj next prev변경
        //pTail 변경
        pHashTableEnt[num].elmtCount++;  //+1 count
        pHashTableEnt[num].pTail->phNext = pObj;
        pObj->phPrev = pHashTableEnt[num].pTail;
        pObj->phNext = NULL;
        pHashTableEnt[num].pTail = pObj;
    }
}

void InsertObjectToHead(Object* pObj, int objNum){
    // Object가 삽입 될 위치 저장
    int num = objNum % HASH_TBL_SIZE;

    // 그 위치가 비어있을 경우
    if(pHashTableEnt[num].pHead == NULL){
        pHashTableEnt[num].pHead = pObj; //insert to head
        pHashTableEnt[num].pTail = pObj; //Head and Tail same
        pHashTableEnt[num].elmtCount++;  //+1 count

        pObj->phNext = NULL;
        pObj->phPrev = NULL;
    }
    else{
        pHashTableEnt[num].elmtCount++;  //+1 count
        pHashTableEnt[num].pHead->phPrev = pObj;
        pObj->phNext = pHashTableEnt[num].pHead;
        pObj->phPrev = NULL;
        pHashTableEnt[num].pHead = pObj;
    }
}

Object* GetObjectByNum(int objnum){    
    int num = objnum % HASH_TBL_SIZE;
    Object* cur = pHashTableEnt[num].pHead;

    while(cur != NULL){
        if(cur->objnum == objnum)
            return cur;
        else
            cur = cur->phNext;
    }

    printf("%d is not exist!\n", objnum);
    return NULL;
}

BOOL DeleteObject(Object* pObj){
    int num = pObj->objnum % HASH_TBL_SIZE;
    Object* cur = pHashTableEnt[num].pHead;

    while(cur != NULL){
        if(cur->objnum == pObj->objnum){
            //하나만 있는경우
            if(pHashTableEnt[num].elmtCount == 1){
                pHashTableEnt[num].pHead = NULL;
                pHashTableEnt[num].pTail = NULL;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            //head 인 경우
            else if(cur->objnum == pHashTableEnt[num].pHead->objnum){
                cur->phNext->phPrev = NULL;
                pHashTableEnt[num].pHead = cur->phNext;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            //tail인 경우
            else if(cur->objnum == pHashTableEnt[num].pTail->objnum){
                cur->phPrev->phNext = NULL;
                pHashTableEnt[num].pTail = cur->phPrev;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            //둘 다 아닌경우
            else{
                cur->phNext->phPrev = cur->phPrev;
                cur->phPrev->phNext = cur->phNext;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            
        }
        else
            cur = cur->phNext; //move next node
    }

    printf("This object is not exist!\n");
    return 1;
}

Object* GetObjectFromObjFreeList(){

}

void InsertObjectIntoObjFreeList(Object* pObj){

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
            printf("   total: %d  {Head: %d Tail: %d}\n", pHashTableEnt[i].elmtCount, pHashTableEnt[i].pHead->objnum, pHashTableEnt[i].pTail->objnum);
        }
        else
            printf("   total: %d  {Head: NULL Tail: NULL}\n", pHashTableEnt[i].elmtCount);
        
    }
}

