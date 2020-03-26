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
        pObj->phPrev = pHashTableEnt[num].pTail;
        pObj->phNext = NULL;
        pHashTableEnt[num].pTail = pObj; //tail change
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
        //pHead를 변경
        //obj next prev변경
        pHashTableEnt[num].elmtCount++;  //+1 count
        pObj->phPrev = NULL;
        pObj->phNext = pHashTableEnt[num].pHead;
        pHashTableEnt[num].pHead = pObj; //head change
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

Object* GetObjectFromObjFreeList(){

}

BOOL DeleteObject(Object* pObj){
    int num = pObj->objnum % HASH_TBL_SIZE;
    Object* cur = pHashTableEnt[num].pHead;

    while(cur != NULL){
        if(cur->objnum == pObj->objnum){
            //head인 경우, tail인 경우, 둘 다 아닌경우
            //
            if(cur->objnum == pHashTableEnt[num].pHead->objnum){
                
            }
        }
        else
            cur = cur->phNext;
    }
}

void InsertObjectIntoObjFreeList(Object* pObj){

}

