#include "hw1.h"

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