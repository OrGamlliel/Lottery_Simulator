#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colList.h"
#include "participants.h"
#include "utils.h"
#include <string.h>

#define MANUAL 1
#define AUTO 2
#define WIN "windows"
#define UNKNOWN "unknown"
#define FILENAME "result.bin"

#if defined _WIN32
#define PLATFORM WIN
#else
#define PLATFORM UNKNOWN
#endif

typedef unsigned char BYTE;

void showMenu();
pList* getParticipants(int** numOfAllCols, int* numOfParticipants);
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
void saveResultsToBfile(pList* participants, int* numOfCols, int* lotteryResult, int numOfParticipants);
BYTE* singleColCompression(int* col);
void secondOption();
int* singleColDecompression(BYTE* compressedCol);


void main()
{
    srand(time(NULL));
    showMenu();
}

void showMenu()
{
    if (PLATFORM == "windows")
        system("cls");

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
        secondOption();
        break;
    case 3:
        exit(0);
        break;
    default:
        exitWithMessage("Invalid input");
        break;
    }

}

void secondOption()
{
    FILE* bf;
    int numOfParticipants, nameLen, numOfCols;
    pList* participants = (pList*)ourMalloc(sizeof(pList));
    makeEmptyPList(participants);

    bf = fopen(FILENAME, "rb");
    if (bf == NULL)
    {
        exitWithMessage("The lottery has not yet taken place\n");
    }
    fread(&numOfParticipants, sizeof(int), 1, bf);
    for(int i=0; i<numOfParticipants; i++)
    {
        colList* currColList = (colList*)ourMalloc(sizeof(colList));
        makeEmptyColList(currColList);
        fread(&nameLen, sizeof(int), 1, bf);//read name length
        char* participantName = (char*)ourMalloc(sizeof(char) * (nameLen + 1));//allocation for name  
        fread(participantName, sizeof(char), nameLen, bf);//read name
        participantName[nameLen] = '\0';

        Data* currParData = createDataForParticipant(participantName,*currColList);
        insertPDataToEndPList(participants, currParData);

        fread(&numOfCols, sizeof(int), 1, bf);
        
        for (int j = 0; j < numOfCols; j++)
        {
            BYTE* compressedCol = (BYTE*)ourMalloc(sizeof(BYTE) * 3);
            fread(compressedCol, sizeof(BYTE), 3, bf);
            int* decompressedCol = singleColDecompression(compressedCol);
            insertDataToEndList(&(currParData->cols), decompressedCol);
        }
    }
    BYTE* winCompressedCol = (BYTE*)ourMalloc(sizeof(BYTE) * 3);
    fread(winCompressedCol, sizeof(BYTE), 3, bf);
    int* winDecompressedCol = singleColDecompression(winCompressedCol);

    int** sumOfHits = (int**)ourMalloc(sizeof(int*));

    lookupForHits(participants, winDecompressedCol, sumOfHits);
    printPList(*participants);
    printf("The winning col is: ");
    printCol(winDecompressedCol);
}

void firstOption()
{
    char userWilling;
    int** sumOfHits;
    int** numOfAllColsArr; //how many cols for each p
    int* numOfParticipants;

    numOfAllColsArr = (int**)ourMalloc(sizeof(int*));
    numOfParticipants = (int*)ourMalloc(sizeof(int));
    sumOfHits = (int**)ourMalloc(sizeof(int*));
    pList* participants = getParticipants(numOfAllColsArr, numOfParticipants);
    int* lotteryResult = getLotteryResult();
   
    lookupForHits(participants, lotteryResult, sumOfHits);
    sortColsByHits(participants);
    printPList(*participants);
    printSumOfHits(*sumOfHits);
    printMostSuccessfulParticipant(participants);
    
    printf("Would you like to keep the latest lottery result (Y/N)?\n");
    getchar();
    scanf("%c", &userWilling);
    switch (userWilling)
    {
    case 'Y':
        saveResultsToBfile(participants, *numOfAllColsArr, lotteryResult, *numOfParticipants);
        showMenu();
        break;
    case 'N':
        exitWithMessage("Thank you! See you next time\n");
        break;
    default:
        exitWithMessage("Invalid input. exiting\n");
        break;
    }
}
void saveResultsToBfile(pList* participants, int* numOfCols, int* lotteryResult, int numOfParticipants)
{
    FILE* bf;
    bf = ourFileOpen(FILENAME, "wb");

    fwrite(&numOfParticipants, sizeof(int), 1, bf);

    Participant* currP = participants->head;
    int currPNameLen = 0;
    int pInd = 0;
    int currNumOfCols = 0;

    while (currP != NULL)
    {
        currPNameLen = strlen(currP->data->name);
        currNumOfCols = numOfCols[pInd++];
        colNode* currCol = currP->data->cols.head;

        fwrite(&currPNameLen, sizeof(int), 1, bf);
        fwrite(currP->data->name, sizeof(char), currPNameLen, bf);
        fwrite(&currNumOfCols, sizeof(int), 1, bf);
        while (currCol != NULL)
        {
            BYTE* compressedCol = singleColCompression(currCol->col);
            fwrite(compressedCol, sizeof(BYTE), 3, bf);
            currCol = currCol->next;
        }
        currP = currP->next;
    }
    BYTE* compressedResult = singleColCompression(lotteryResult);
    fwrite(compressedResult, sizeof(BYTE), 3, bf);
    ourFileClose(bf);
}

BYTE* singleColCompression(int* col)
{
    int i, j;
    int currVal, nextVal;
    BYTE* shortCol = (BYTE*)ourMalloc(sizeof(char) * 3);

    for (i = j= 0; i < 3; i++, j+=2) //i for byte array
    {
        currVal = col[j];
        nextVal = col[j + 1] ;

        shortCol[i] = currVal << 4 | nextVal;
    }
    return shortCol;
}

int* singleColDecompression(BYTE* compressedCol)
{
    int i, j;
    int* longCol = (int*)ourMalloc(sizeof(int) * MAX_NUM_IN_COLS);
    BYTE rightMask = 0x0F;
    for (i = j = 0; i < 3; i++, j+=2) //i for byte array
    {
        int val = compressedCol[i];
        longCol[j] = (val >> 4) & rightMask;
        longCol[j + 1] = val & rightMask;
    }
    return longCol;
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
    printf("The participant with the best hit average is: %s, with %.2f hits\n\n", bestName, bestHitsAvg);
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
    *sum = (int*)ourCalloc(sizeof(int), 7);
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

pList* getParticipants(int** numOfAllCols, int* numOfParticipants)
{
    pList* pLst = (pList*)ourMalloc(sizeof(pList));
    makeEmptyPList(pLst); //make empty participants list


    Data* currData;

    colList colLst;
    int lotteryMode;
    int i, pIndex;
    char* name;

    printf("Please enter the number of participants\n");
    scanf("%d", numOfParticipants);

    *numOfAllCols = (int*)ourMalloc(sizeof(int) * (*numOfParticipants));


    for (i = 0, pIndex = 0; i < *numOfParticipants; i++, pIndex ++)
    {
        name = getName();
        makeEmptyColList(&colLst);
        currData = createDataForParticipant(name, colLst);
        insertPDataToEndPList(pLst, currData);
        lotteryMode = getLotteryMode();
        (*numOfAllCols)[pIndex] = getCols(&pLst->tail->data->cols, lotteryMode);
       
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