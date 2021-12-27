#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MANUAL 1
#define AUTO 2

typedef struct col{
    int* chosenNumbers;
} Col;

typedef struct collistNode { 
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

void getParticipants();
int getLotteryMode();
void getCols(colList* colLst, int lotteryMode);
void makeEmptyPList(pList* plst);
void makeEmptyColList(colList* colst);
bool isEmptyPList(pList pList);
void insertDataToEndList(colList* colList, int* col);
Data* createDataForParticipant(char* name, colList cols);
char* getName();
Participant* createNewParticipant(Data* participantData, Participant* next);
void insertPDataToEndPList(pList* pList, Data* participantData);
void insertPNodeToEndList(pList* pList, Participant* newTail);
void insertDataToStartPList(pList* pList, Data* participantData);
void insertParticipantToHead(pList* pList, Participant* newHead);
void printPList(pList pList);
void freeList(colList colList);
void checkMemoryAllocation(void* ptr);
int* getAutomaticCol();
void firstOption();

void main() 
{
    int userChoice;

    printf("Please choose one of the following option:\n");
    printf("1. Enter number of participants\n");
    printf("2. View latest lottery results\n");
    printf("3. Exit\n");
    scanf("%d", &userChoice);

    switch (userChoice)
    {
    case 1:
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

void firstOption()
{
    getParticipants();
}

void getParticipants()
{
    pList* pLst = (pList*)malloc(sizeof(pList));
    makeEmptyPList(pLst); //make empty participants list

    Data* currData;

    colList colLst;
    int numOfParticipants;
    int lotteryMode;
    int i;
    char* name;

    printf("Please enter the number of participants\n");
    scanf("%d", &numOfParticipants);
    
    for (i = 0; i < numOfParticipants; i++)
    {
        name = getName();
        makeEmptyColList(&colLst);
        currData = createDataForParticipant(name, colLst);
        insertPDataToEndPList(pLst, currData);
        lotteryMode = getLotteryMode();
    }
}

char* getName()
{
    int logSize = 0;
    int phsSize = 2;
    char c;

    char* name = (char*)malloc(sizeof(char) * phsSize);
    printf("Please enter participant name:\n");
    c = getchar();
    while(c!='\n')
    {
        if (logSize >= phsSize-1)
        {
            phsSize *= 2;
            name = (char*)realloc(name, sizeof(char) * phsSize);
        }
        name[logSize++] = c;
    }
    if (logSize != phsSize)
        name = (char*)realloc(name, sizeof(char) * logSize);

    return name;
}

int getLotteryMode()
{
    int lotteryMode;
    printf("1. Manual lottery\n");
    printf("2. Auto lottery\n");
    scanf("%d", &lotteryMode);
    return lotteryMode;
}
void getCols(int lotteryMode)
{
    int N; //desired num of columns
    printf("Please enter desired number of columns\n");
    scanf("%d", &N);
    switch (lotteryMode)
    {
    case MANUAL:

        break;
    case AUTO:

        break;
    default:
        break;
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
    {
        colList->tail = newHead;
    }
    colList->head = newHead;
}

void insertNodeToEndList(colList* colList, colListNode* newTail)
{
    newTail->next = NULL;

    if (isEmptyList(*colList))
        colList->head = colList->tail = newTail;
    else
    {
        colList->tail->next = newTail;
        colList->tail = newTail;
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
