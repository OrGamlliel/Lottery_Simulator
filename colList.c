#include "colList.h"


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

    result->col = data;
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

void insertDataToEndList(colList* colList, int* col)
{
    colListNode* newTail;
    newTail = createNode(col, NULL);
    insertNodeToEndList(colList, newTail);
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


int* getAutomaticCol()
{
    int numsPool[15]; //Will contain all the possible number options
    int* col = (int*)malloc(sizeof(int) * 6); //Will contain the selected col
    int i;
    int randomCell; //Will be used to select a random cell from numPools

    for (i = 0; i < 15; i++)
        numsPool[i] = i + 1;

    for (i = 0; i < 6; )
    {
        srand(time(NULL));
        randomCell = rand() % 15;

        if (numsPool[randomCell] != 0)          //Verify that the random number wasn't selected before
        {
            col[i++] = numsPool[randomCell];
            numsPool[randomCell] = 0;
        }
    }
    return col;
}

bool isDifferent(Col arr, int val, int index)
{
    int i;
    bool isdiff = true;

    for (i = 0; i < index && isdiff; i++)
    {
        if (arr[i] == val)
        {
            isdiff = false;
        }
    }
    return isdiff;
}