/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#include "participants.h"
#include "utils.h"

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

    result = (Data*)ourMalloc(sizeof(Data));

    result->name = name;
    result->cols = cols;

    return result;
}

Participant* createNewParticipant(Data* participantData, Participant* next)
{
    Participant* result;

    result = (Participant*)ourMalloc(sizeof(Participant));

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

void printPList(pList pList)
{
    Participant* p;
    
    printf(ANSI_COLOR_RED "PARTICIPANTS:" ANSI_COLOR_RESET "\n\n");

    for (p = pList.head; p != NULL; p = p->next)
        printPartipant(p);
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
        freeParticipant(q);
        free(q);
    }
    free(p);
}

void freeParticipant(Participant* p)
{
    if (p != NULL)
    {
        if (p->data != NULL)
        {
            if (p->data->name != NULL)
            {
                free(p->data->name);
            }
            free(p->data);
        }
        freeList(&p->data->cols);
    }
}

void printPartipant(Participant* p)
{
    int index = 1;
    colNode* ptr = p->data->cols.head;
    printf("Name: %s:\n", p->data->name);
    
    while (ptr != NULL)
    {
        printf("Col %d: ", index++);
        printColNode(ptr);
        ptr = ptr->next;
    }
    printf("\n");
}
