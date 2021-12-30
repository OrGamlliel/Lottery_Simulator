#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colList.h"
#include "participants.h"
#include "utils.h"

#define MANUAL 1
#define AUTO 2
#define WIN "windows"
#define UNKNOWN "unknown"

#if defined _WIN32
    #define PLATFORM WIN
#else
    #define PLATFORM UNKNOWN
#endif

pList* getParticipants();
int getLotteryMode();
int getCols(colList* colLst, int lotteryMode);
char* getName();
void freeList(colList colList);
void checkMemoryAllocation(void* ptr);
void firstOption();
void getListFromUser(colList* lstC, int numOfCols);
int* getLotteryResult();
void lookupForHits(pList* participants, int* lotteryResult);
void checkHitsForParticipant(Participant* p, int* lotteryResult);
void sortColsByHits(pList* participants);

void main() 
{
    int userChoice;
    printf("Please choose one of the following option:\n");
    printf("1. Enter number of participants\n");
    printf("2. View latest lottery results\n");
    printf("3. Exit\n");
    scanf("%d", &userChoice);

    if (PLATFORM == "windows")
        system("cls");
 
    switch (userChoice)
    {
    case 1:
        firstOption();
        break;
    case 2:

        break;
    case 3:
        exit(0);
        break;
    default:
        break;
    }

}

void firstOption()
{
    pList* participants = getParticipants();
    int* lotteryResult = getLotteryResult();
    lookupForHits(participants, lotteryResult);
    sortColsByHits(participants);
    printPList(*participants);

}

void sortColsByHits(pList* participants)
{
  /*  Participant* p = participants->head;
    while (p != NULL)
    {
        colNode* colP = p->data->cols.head;

        while (colP != NULL)
        {
            for (int hits = 6; hits >= 0; hits--)
            {
                if (colP->hits == hits && colP->next != NULL)
                {
                    p->data->cols.head = colP->next;
                    p->data->cols.tail = colP;
                    colP->next = NULL;
                }
                colP = colP->next;
            }
        }
        p = p->next;
    }*/
}

void lookupForHits(pList* participants, int* lotteryResult)
{
    Participant* p = participants->head;

    while (p != NULL)
    {
        checkHitsForParticipant(p, lotteryResult);
        p = p->next;
    }
}

void checkHitsForParticipant(Participant* p, int* lotteryResult)
{
    colNode* currCol = p->data->cols.head;

    while (currCol != NULL)
    {
        for (int i = 0; i < MAX_NUM_IN_COLS; i++)
        {
            for (int j = 0; j < MAX_NUM_IN_COLS; j++)
            {
                if (currCol->col[i] == lotteryResult[j])
                    currCol->hits++;
            }
        }
        currCol = currCol->next;
    }
}

int* getLotteryResult()
{
    int* result = getAutomaticCol();

    printf("The winning column is:\t");
    printCol(result);
    printf("\n");

    return result;
}

pList* getParticipants()
{
    pList* pLst = (pList*)ourMalloc(sizeof(pList));
    makeEmptyPList(pLst); //make empty participants list

    Data* currData;

    colList colLst;
    int numOfParticipants;
    int lotteryMode;
    int i, pIndex;
    char* name;

    printf("Please enter the number of participants\n");
    scanf("%d", &numOfParticipants);

    int* numOfCols = (int*)ourMalloc(sizeof(int) * numOfParticipants);

    for (i = 0, pIndex = 0; i < numOfParticipants; i++, pIndex ++)
    {
        name = getName();
        makeEmptyColList(&colLst);
        currData = createDataForParticipant(name, colLst);
        insertPDataToEndPList(pLst, currData);
        lotteryMode = getLotteryMode();
        numOfCols[pIndex] = getCols(&pLst->tail->data->cols, lotteryMode); //OrG needs to continue
        printf("\n\n");
    }
    
    return pLst;
}

char* getName()
{
    int logSize = 0;
    int phsSize = INIT_SIZE;
    char c;

    char* name = (char*)ourMalloc(sizeof(char) * phsSize);
    printf("Please enter participant name:\n");
    c = getchar();
    c = getchar();
    while(c!='\n')
    {
        if (logSize >= phsSize-1)
        {
            phsSize *= 2;
            name = (char*)ourRealloc(name, sizeof(char) * phsSize);
        }
        name[logSize++] = c;
        c = getchar();
    }
    name[logSize++] = '\0';
    if (logSize != phsSize)
        name = (char*)ourRealloc(name, sizeof(char) * logSize);

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

int getCols(colList* colLst, int lotteryMode)
{
    int N; //desired num of columns
    printf("Please enter desired number of columns\n");
    scanf("%d", &N);
    switch (lotteryMode)
    {
    case MANUAL:
        getListFromUser(colLst, N);
        break;
    case AUTO:
        for (int col = 0; col < N; col++)
        {
            int* autoCol = getAutomaticCol();
            insertDataToEndList(colLst, autoCol);
            colLst->tail->hits = 0;
        }
        break;
    default:
        break;
    }
    return N;
}


void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("Memory allocation has failed");
        exit(1);
    }
}


void getListFromUser(colList* lstC, int numOfCols)
{
    int currChosenNum;

    for (int i = 0; i < numOfCols; i++)
    {
        int* currCol = (int*)ourMalloc(sizeof(int) * MAX_NUM_IN_COLS);

        printf("Please enter %d values:\n", MAX_NUM_IN_COLS);

        for (int j = 0; j < MAX_NUM_IN_COLS;)//fill one col
        {
            scanf("%d", &currChosenNum);
            if (isDifferent(currCol, currChosenNum, j) && isWithinRange(currChosenNum))
            {
                currCol[j] = currChosenNum;
                j++;
            }
            else
                printf("Sorry your last value is invalid, please enter another value");
        }
        insertDataToEndList(lstC, currCol);
        lstC->tail->hits = 0;
    }
}