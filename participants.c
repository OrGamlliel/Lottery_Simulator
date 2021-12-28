#include "participants.h"

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

void insertPDataToEndPList(pList* pList, Data* participantData)
{
    Participant* newTail;
    newTail = createNewParticipant(participantData, NULL);
    insertPNodeToEndList(pList, newTail);
}

void insertPNodeToEndList(pList* pList, Participant* newTail)
{
    newTail->next = NULL;

    if (isEmptyPList(*pList))
    {
        pList->head = pList->tail = newTail;
    }
    else
    {
        pList->tail->next = newTail;
        pList->tail = newTail;
    }
}

void insertDataToStartPList(pList* pList, Data* participantData) //insert data to start p list
{
    Participant* newHead;
    newHead = createNewParticipant(participantData, NULL);
    insertParticipantToHead(pList, newHead);
}

void insertParticipantToHead(pList* pList, Participant* newHead) //insert node to start p list
{
    newHead->next = pList->head;
    if (pList->tail == NULL)
        pList->tail = newHead;
    pList->head = newHead;
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