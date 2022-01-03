/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#include "colList.h"

void makeEmptyColList(colList* colst)
{
    colst->head = colst->tail = NULL;
}

bool isEmptyList(colList colList)
{
    return (colList.head == NULL);
}

colNode* createNode(int* data, int hits, colNode* next)
{
    colNode* result;

    result = (colNode*)ourMalloc(sizeof(colNode));

    result->col = data;
    result->hits = hits;
    result->next = next;

    return result;
}

void insertNodeToEndList(colList* colList, colNode* newTail)
{
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
    newTail = createNode(col, 0, NULL);
    insertNodeToEndList(colList, newTail);
}

void freeList(colList* lst)
{
    colNode* curr;
    colNode* q;
    curr = lst->head;

    while (curr != NULL)
    {
        q = curr->next;
        if (curr->col != NULL)
            free(curr->col);
        free(curr);
        curr = q;
    }
    lst->head = NULL;
    lst->tail = NULL;
}

int* getAutomaticCol()
{
    int i;
    int randomNum;
    int* autoCol = (int*)ourMalloc(sizeof(int) * MAX_NUM_IN_COLS);

    for (i = 0; i < 6; )
    {
        randomNum = 1 + rand() % 15;

        if (isDifferent(autoCol, randomNum, i)) 
        {
            autoCol[i++] = randomNum;
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
    return (number > 0 && number <= 15) ? true : false;
}

void printCol(int* col, bool align)
{
    for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            printf("[");
        align ? printf("%2d", col[i]) : printf("%d", col[i]);
        if (i < 5)
            printf(",");
        else if (i == 5)
            printf("]");
    }
}

void printColNode(colNode* colNode)
{
    printCol(colNode->col, true);
    printf("- Hits: %d", colNode->hits);
    printf("\n");
}
