#include "colList.h"


void makeEmptyColList(colList* colst)
{
    colst->head = colst->tail = NULL;
}

bool isEmptyList(colList colList)
{
    return (colList.head == NULL);
}

colNode* createNode(int* data, colNode* next)
{
    colNode* result;

    result = (colNode*)ourMalloc(sizeof(colNode));

    result->col = data;
    result->next = next;

    return result;
}


void insertNodeToHead(colList* colList, colNode* newHead)
{
    newHead->next = colList->head;
    if (colList->tail == NULL)
    {
        colList->tail = newHead;
    }
    colList->head = newHead;
}

void insertNodeToEndList(colList* colList, colNode* newTail)
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
    colNode* newTail;
    newTail = createNode(col, NULL);
    insertNodeToEndList(colList, newTail);
}

void freeList(colList colList)
{
    colNode* p, * q;

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
    int* autoCol = (int*)ourMalloc(sizeof(int) * MAX_NUM_IN_COLS); //Will contain the selected col
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
            autoCol[i++] = numsPool[randomCell];
            numsPool[randomCell] = 0;
        }
    }
    return autoCol;
}

bool isDifferent(int* arr, int val, int index)
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

bool isWithinRange(int number)
{
    return (number > 0 && number < 15) ? true : false;
}

void printCol(int* col)
{
    for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            printf("[");
        printf("%d", col[i]);
        if (i < 5)
            printf(",");
        else if (i == 5)
            printf("]");
    }
    printf("\n");
}

void printColNode(colNode* colNode)
{
    printf("Column = ");
    printCol(colNode->col);
    printf("\n");
}
