#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include "colList.h"
#include "participants.h"
#include "utils.h"

#define MANUAL 1
#define AUTO 2

void getParticipants();
int getLotteryMode();
void getCols(colList* colLst, int lotteryMode);
char* getName();
void freeList(colList colList);
void checkMemoryAllocation(void* ptr);
void firstOption();
void getListFromUser(colList* lstC, int* numOfCols);


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
    pList* pLst = (pList*)ourMalloc(sizeof(pList));
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
        getCols(&colLst, lotteryMode);
    }
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
void getCols(colList* colLst, int lotteryMode)
{
    int N; //desired num of columns
    printf("Please enter desired number of columns\n");
    scanf("%d", &N);
    switch (lotteryMode)
    {
    case MANUAL:

        break;
    case AUTO:
        for (int col = 0; col < N; col++)
            insertDataToEndList(colLst, getAutomaticCol());
        break;
    default:
        break;
    }
}


void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("Memory allocation has failed");
        exit(1);
    }
}


void getListFromUser(colList* lstC, int* numOfCols)
{
    int currChosenNum;
    Col currCol = (Col)ourMalloc(sizeof(int) * MAX_NUM_IN_COLS);

    for (int i = 0; i < numOfCols; i++)
    {
        for (int j = 0; j < MAX_NUM_IN_COLS; j++)//fill one col
        {
            scanf("%d", &currChosenNum);
            if (isDifferent(currCol, currChosenNum, j))
            {
                currCol[j] = currChosenNum;
            }
        }
        insertDataToEndList(lstC, &currCol);
    }
    //ToDo: Or will add creation of list
}
