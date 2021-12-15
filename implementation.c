#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct col{
    int* chosenNumbers;
} Col;

typedef struct collistNode { //
    Col* data;
    struct listNode* next;//ptr to the next col
} colListNode;

typedef struct colList {
    colListNode* head;
    colListNode* tail;
} colList;

typedef struct data
{
    char* name;
    colList cols;
} Data;

typedef struct participant
{
    Data* data;
    struct participant* next;//ptr to the next participant
} Participant;

typedef struct pList {
    Participant* head;
    Participant* tail;
} pList;


void main() 
{
    int userChoice;
    int numOfParticipants;

    printf("Please choose one of the following option:\n");
    printf("1. Enter number of participants\n");
    printf("2. View latest lottery results\n");
    printf("3. Exit\n");

    switch (userChoice)
    {
    case 1:
        scanf("%d", &numOfParticipants);
        firstOption();
        break;
    case 2:

        break;
    case 3:
        break;
    default:
        break;
    }

}
void getParticipants()
{
    pList* pLst;
    makeEmptyPList(pLst); //make empty participants list

    Data* currData;

    colList colLst;
    int numOfParticipants;
    int i;
    char* name;

    printf("Please enter the number of participants\n");
    scanf("%d", &numOfParticipants);
    
    for (i = 0; i < numOfParticipants; i++)
    {
        scanf("%s", &name);
        makeEmptyColList(&colLst);
        currData = createDataForParticipant(name, colLst);
        insertDataToEndPList(pLst, currData);
    }
}

void makeEmptyPList(pList* plst)
{
    plst->head = plst->tail = NULL;
}

bool isEmptyPList(pList pList)
{
    return (pList.head == NULL);
}

Data* createDataForParticipant(char* name, colList cols)
{
    Data* result;

    result = (Data*)malloc(sizeof(Data));
    checkMemoryAllocation(result);

    result->name = name;
    result->cols = cols;

    return result;
}

Participant* createNewParticipant(Data* participantData, Participant* next)
{
    Participant* result;

    result = (Participant*)malloc(sizeof(Participant));
    checkMemoryAllocation(result);

    result->data = participantData;
    result->next = next;

    return result;
}

void insertDataToStartPList(pList* pList, Data* participantData)
{
    Participant* newHead;
    newHead = createNewParticipant(participantData, NULL);
    insertParticipantToHead(pList, newHead);
}
void insertDataToEndPList(pList* pList, Data* participantData)
{
    Participant* newTail;
    newTail = createNewParticipant(participantData, NULL);
    insertParticipantToTail(pList, newTail);
}
void insertParticipantToHead(pList* pList, Participant* newHead)
{
    newHead->next = pList->head;
    if (pList->tail == NULL)
        pList->tail = newHead;
    pList->head = newHead;
}

void insertParticipantToTail(pList* pList, Participant* newTail)
{
    newTail->next = NULL;

    if (isEmptyPList(*pList))
        pList->head = pList->tail = newTail;
    else
    {
        pList->tail->next = newTail;
        pList->tail = newTail;
    }
}

void printPList(pList pList)
{
    Participant* p;

    for (p = pList.head; p != NULL; p = p->next)
        //printf ("%d\n",p->data); toDo
        printf("\n");
}

void freePList(pList pList)
{
    Participant* p, * q;

    if (isEmptyPList(pList))
        return;

    p = pList.head;

    while (p->next != NULL)
    {
        q = p;
        p = p->next;
        free(q);
    }
    free(p);
}
void makeEmptyColList(colList* colst)
{
    colst->head = colst->tail = NULL;
}

bool isEmptyList(colList colList)
{
    return (colList.head == NULL);
}

colListNode* createNode(Col* data, colListNode* next)
{
    colListNode* result;

    result = (colListNode*)malloc(sizeof(colListNode));
    checkMemoryAllocation(result);

    result->data = data;
    result->next = next;

    return result;
}


void insertNodeToHead(colList* colList, colListNode* newHead)
{
    newHead->next = colList->head;
    if (colList->tail == NULL)
        colList->tail = newHead;
    colList->head = newHead;
}

void insertNodeToTail(colList* lst, colListNode* newTail)
{
    newTail->next = NULL;

    if (isEmptyList(*lst))
        lst->head = lst->tail = newTail;
    else
    {
        lst->tail->next = newTail;
        lst->tail = newTail;
    }
}

void printList(colList colList)
{
    colListNode* p;

    for (p = colList.head; p != NULL; p = p->next)
        //printf ("%d\n",p->data); toDo
        printf("\n");
}

void freeList(colList colList)
{
    colListNode* p, * q;

    if (isEmptyList(colList))
        return;

    p = colList.head;

    while (p->next != NULL)
    {
        q = p;
        p = p->next;
        free(q);
    }
    free(p);
}

void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("Memory allocation has failed");
        exit(1);
    }
}
