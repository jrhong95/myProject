#include "hw1.h"
#include <stdlib.h>

void Init(){
    // pHashTableEnt reset
    int i;
    for(i = 0; i < HASH_TBL_SIZE; i++){
        pHashTableEnt[i].elmtCount = 0;
        pHashTableEnt[i].pHead = NULL;
        pHashTableEnt[i].pTail = NULL;
    }
    // Freelist reset
    pFreeListHead = NULL;
    pFreeListTail = NULL;
}

void InsertObjectToTail(Object* pObj, int ObjNum){
    int num = ObjNum % HASH_TBL_SIZE;    //get address where obj is saved

    // hashtable is empty
    if(pHashTableEnt[num].pHead == NULL){
        pHashTableEnt[num].pHead = pObj; //insert to head
        pHashTableEnt[num].pTail = pObj; //Head and Tail same
        pHashTableEnt[num].elmtCount++;  //+1 count

        pObj->phNext = NULL;
        pObj->phPrev = NULL;
        pObj->objnum = ObjNum;
    }
    else{
        //obj next prev change
        //pTail change
        pHashTableEnt[num].elmtCount++;  //+1 count
        pHashTableEnt[num].pTail->phNext = pObj;
        pObj->phPrev = pHashTableEnt[num].pTail;
        pObj->phNext = NULL;
        pHashTableEnt[num].pTail = pObj;
        pObj->objnum = ObjNum;
    }
}

void InsertObjectToHead(Object* pObj, int objNum){
    int num = objNum % HASH_TBL_SIZE;

    // HashTable is empty
    if(pHashTableEnt[num].pHead == NULL){
        pHashTableEnt[num].pHead = pObj; //insert to head
        pHashTableEnt[num].pTail = pObj; //Head and Tail same
        pHashTableEnt[num].elmtCount++;  //+1 count

        pObj->phNext = NULL;
        pObj->phPrev = NULL;
        pObj->objnum = objNum;
    }
    else{
        pHashTableEnt[num].elmtCount++; 
        pHashTableEnt[num].pHead->phPrev = pObj;
        pObj->phNext = pHashTableEnt[num].pHead;
        pObj->phPrev = NULL;
        pHashTableEnt[num].pHead = pObj;
        pObj->objnum = objNum;
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
    // objnum is not exist
    return NULL;
}

BOOL DeleteObject(Object* pObj){
    int num = pObj->objnum % HASH_TBL_SIZE;
    Object* cur = pHashTableEnt[num].pHead;

    while(cur != NULL){
        if(cur == pObj){
            //case 1: 1 obj in hashtable
            if(pHashTableEnt[num].elmtCount == 1){
                pHashTableEnt[num].pHead = NULL;
                pHashTableEnt[num].pTail = NULL;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            //case 2: obj is head
            else if(cur == pHashTableEnt[num].pHead){
                cur->phNext->phPrev = NULL;
                pHashTableEnt[num].pHead = cur->phNext;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            //case 3: obj is tail
            else if(cur == pHashTableEnt[num].pTail){
                cur->phPrev->phNext = NULL;
                pHashTableEnt[num].pTail = cur->phPrev;
                pHashTableEnt[num].elmtCount--;
                free(cur);
                return 0;
            }
            //case 4: not head and tail
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
    // if pObj is not exist
    printf("This object is not exist! Can't Delete\n");
    return 1;
}
// double linkedlist
Object* GetObjectFromObjFreeList(){
    //list empty
    //printf("List init\n head: %p  Tail: %p\n", pFreeListHead, pFreeListTail);
    if((pFreeListHead == NULL) && (pFreeListTail == NULL)){
        printf("FreeList is NULL\nhead: %p  Tail: %p\n", pFreeListHead, pFreeListTail);
        return NULL;
    }
    //In list one Node
    if(pFreeListHead == pFreeListTail){
        Object* temp = pFreeListTail;
        pFreeListHead = NULL;
        pFreeListTail = NULL;
        return temp;
    }
    //more than 2 obj
    else{
        Object* temp = pFreeListTail;
        pFreeListTail = pFreeListTail->phPrev;
        temp->phPrev->phNext = NULL;
        temp->phPrev = NULL;

        return temp;
    }
}
// double linkedlist
void InsertObjectIntoObjFreeList(Object* pObj){
    //If List is Empty
    if(pFreeListHead == NULL){
        pFreeListHead = pObj;
        pFreeListTail = pObj;
        pObj->phNext = NULL;
        pObj->phPrev = NULL;
        
    }
    else{//insert to Head
        pObj->phNext = pFreeListHead;
        pFreeListHead->phPrev = pObj;
        pFreeListHead = pObj;
    }
}
