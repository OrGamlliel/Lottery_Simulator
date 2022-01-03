//
//  lottery.c
//  finalProjectC
//
//  Created by Avital Rubichi on 03/01/2022.
//

#include "lottery.h"

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

void printMostSuccessfulParticipant(pList* participants)
{
    Participant* p = participants->head;
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

void printSumOfHits(int* arr)
{
    for (int i = 0; i < 7; i++)
    {
        printf("The number of columns with %d hits is: %d\n", i, arr[i]);
    }
    printf("\n\n");
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
                printf("Last value is invalid, please enter another value\n");
        }
        insertDataToEndList(lstC, currCol);
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
        currPNameLen = (int)strlen(currP->data->name);
        currNumOfCols = numOfCols[pInd++];
        colNode* currCol = currP->data->cols.head;

        fwrite(&currPNameLen, sizeof(int), 1, bf);
        fwrite(currP->data->name, sizeof(char), currPNameLen, bf);
        fwrite(&currNumOfCols, sizeof(int), 1, bf);
        while (currCol != NULL)
        {
            BYTE* compressedCol = singleColCompression(currCol->col);
            fwrite(compressedCol, sizeof(BYTE), SHORT_COL, bf);
            currCol = currCol->next;
        }
        currP = currP->next;
    }
    BYTE* compressedResult = singleColCompression(lotteryResult);
    fwrite(compressedResult, sizeof(BYTE), SHORT_COL, bf);
    ourFileClose(bf);
}

BYTE* singleColCompression(int* col)
{
    int i, j;
    int currVal, nextVal;
    BYTE* shortCol = (BYTE*)ourMalloc(sizeof(char) * SHORT_COL);

    for (i = j= 0; i < SHORT_COL; i++, j+=2) //i for byte array
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
    for (i = j = 0; i < SHORT_COL; i++, j+=2) //i for byte array
    {
        int val = compressedCol[i];
        longCol[j] = (val >> 4) & rightMask;
        longCol[j + 1] = val & rightMask;
    }
    return longCol;
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

void getListFromAutomator(colList* colLst, int numOfCols)
{
    for (int col = 0; col < numOfCols; col++)
    {
        int* autoCol = getAutomaticCol();
        insertDataToEndList(colLst, autoCol);
    }
}
