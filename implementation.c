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

void showMenu();
pList* getParticipants();
int getLotteryMode();
int getCols(colList* colLst, int lotteryMode);
char* getName();
void freeList(colList colList);
void checkMemoryAllocation(void* ptr);
void firstOption();
void getListFromUser(colList* lstC, int numOfCols);
void getListFromAutomator(colList* colLst, int numOfCols);
int* getLotteryResult();
void lookupForHits(pList* participants, int* lotteryResult, int** sum);
void checkHitsForParticipant(Participant* p, int* lotteryResult, int** arr);
void sortColsByHits(pList* participants);
void printMostSuccessfulParticipant(pList* participants);
void sortColsByHits(pList* participants);
void MergeSort(colNode** headRef);
colNode* SortedMerge(colNode* head1, colNode* head2);
void middle(colNode* source, colNode** frontRef, colNode** backRef);
void printSumOfHits(int* arr);

void main()
{
    srand(time(NULL));
    showMenu();
}

void showMenu()
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
    int** sumOfHits;
    sumOfHits = (int**)ourMalloc(sizeof(int*));
    pList* participants = getParticipants();
    int* lotteryResult = getLotteryResult();
    lookupForHits(participants, lotteryResult, sumOfHits);
    sortColsByHits(participants);
    //printPList(*participants);
    printSumOfHits(*sumOfHits);
    printMostSuccessfulParticipant(participants);
}

void printMostSuccessfulParticipant(pList* participants)
{
    Participant* p = participants->head;
    int pIndex = 0;
    char* bestName = "No one";
    float bestHitsAvg = 0;

    while (p != NULL)
    {
        colNode* c = p->data->cols.head;
        int hitsSum = 0;
        int colsCount = 0;
        float hitsAvg = 0;

        while (c != NULL)
        {
            hitsSum += c->hits;
            colsCount++;
            c = c->next;
        }
        hitsAvg = (float)hitsSum / (float)colsCount;
        if (hitsAvg > bestHitsAvg)
        {
            bestHitsAvg = hitsAvg;
            bestName = p->data->name;
        }
        p = p->next;
    }
    printf("The participant with the best hit average is: %s, with %.2f hits", bestName, bestHitsAvg);
}


void sortColsByHits(pList* participants)
{
    Participant* p = participants->head;

    while (p != NULL)
    {
        MergeSort(&(p->data->cols.head)); 
        p = p->next;
    }
}
/* sorts the linked list by changing links */
void MergeSort(colNode** headRef) {
    colNode* head = *headRef;
    colNode* newHead1;
    colNode* newHead2;

    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }
    middle(head, &newHead1, &newHead2);
    MergeSort(&newHead1);
    MergeSort(&newHead2);
    *headRef = SortedMerge(newHead1, newHead2);
}

colNode* SortedMerge(colNode* head1, colNode* head2)
{
    colNode* res = NULL;
    /* Base cases */
    if (head1 == NULL)
        return (head2);
    else if (head2 == NULL)
        return (head1);
    if (head1->hits > head2->hits)
    {
        res = head1;
        res->next = SortedMerge(head1->next, head2);
    }
    else
    {
        res = head2;
        res->next = SortedMerge(head1, head2->next);
    }
    return (res);
}

//finds the middle node in the list
void middle(colNode* source, colNode** frontRef, colNode** backRef)
{
    colNode* slow;
    colNode* fast;
    slow = source;
    fast = source->next;

    //every loop the fast index advances 2 nodes and the slow advanced 1 node
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    //splits the list and define the head and tail in each
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void lookupForHits(pList* participants, int* lotteryResult, int** sum)
{
    Participant* p = participants->head;
    *sum = (int*)calloc(sizeof(int), 7);
   // check memory
    while (p != NULL)
    {
        checkHitsForParticipant(p, lotteryResult, sum);
        p = p->next;
    }
}

void checkHitsForParticipant(Participant* p, int* lotteryResult, int** arr)
{
    colNode* currCol = p->data->cols.head;
    bool isFound = false;
  
    while (currCol != NULL)
    {
        for (int i = 0; i < MAX_NUM_IN_COLS; i++) //i for numbers in one col
        {
            for (int j = 0; j < MAX_NUM_IN_COLS && !isFound; j++)// j for numbers in lottery result
            {
                if (currCol->col[i] == lotteryResult[j])
                {
                    currCol->hits++;
                    isFound = true;
                }
            }
            isFound = false;
        }//function to find num in array
        (*arr)[currCol->hits]++;
        currCol = currCol->next;
    }
}

int* getLotteryResult()
{
    int* result = getAutomaticCol();

    printf("The winning column is:\t");
    printCol(result);
    printf("\n\n");

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
        getListFromAutomator(colLst, N);
        break;
    default:
        break;
    }
    return N;
}

void getListFromAutomator(colList* colLst, int numOfCols)
{
    for (int col = 0; col < numOfCols; col++)
    {
        int* autoCol = getAutomaticCol();
        insertDataToEndList(colLst, autoCol);
        colLst->tail->hits = 0;
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
                printf("Sorry your last value is invalid, please enter another value\n");
        }
        insertDataToEndList(lstC, currCol);
        lstC->tail->hits = 0;
    }
}
void printSumOfHits(int* arr)
{
    for (int i = 0; i < 7; i++)
    {
        printf("The number of columns with %d hits is: %d\n", i, arr[i]);
    }
    printf("\n\n");
}