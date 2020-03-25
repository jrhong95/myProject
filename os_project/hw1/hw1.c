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
    else if(pHashTableEnt[num].pTail != NULL){
        // pTail 있는경우
        // pTail 가리키는 obj부터 next가 null이 될 때까지 이동
        Object *cur = pHashTableEnt[num].pTail;
        while(cur->phNext != NULL){
            cur = cur->phNext;
        }
        cur->phNext = pObj;
        pObj->phPrev = cur;
        pObj->phNext = NULL;
        pHashTableEnt[num].elmtCount++;  //+1 count

        pHashTableEnt[num].pTail = pObj; //Tail 변경
    }
}

void InsertObjectToHead(Object* pObj, int objNum){

}

Object* GetObjectByNum(int objnum){    

}

Object* GetObjectFromObjFreeList(){

}

BOOL DeleteObject(Object* pObj){

}

void InsertObjectIntoObjFreeList(Object* pObj){

}

