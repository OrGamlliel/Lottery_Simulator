/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#include "lottery.h"

/*this function creates the participants list,
returns updated number of participants and updated array that includes how many col each participans has*/
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

    //for each participan get name and cols list
    for (i = 0, pIndex = 0; i < *numOfParticipants; i++, pIndex ++)
    {
        name = getName();
        makeEmptyColList(&colLst);
        currData = createDataForParticipant(name, colLst);
        insertPDataToEndPList(pLst, currData);
        lotteryMode = getLotteryMode(); //choose manual or auto

        (*numOfAllCols)[pIndex] = getCols(&pLst->tail->data->cols, lotteryMode);
       
        printf("\n\n");
    }
    return pLst;
}

/* The function asks the user to insert lottery mode (auto or manual) and returns the wanted result */
int getLotteryMode()
{
    int lotteryMode;
    printf("1. Manual lottery\n");
    printf("2. Auto lottery\n");
    scanf("%d", &lotteryMode);
    if (lotteryMode != 1 && lotteryMode != 2)
    {
        printf("Invalid mode was chosen. Please try again:\n");
        return getLotteryMode();
    }
    return lotteryMode;
}

/* The function asks the user to insert how many cols he would like to insert, then gets the list of cols */
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
    }
    return N;
}

/* The function prints the info of the most successful participant (has the highest avg of hits per cols */
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

/* The function prints how many columns have each number of hits (0-6) */
void printSumOfHits(int* arr)
{
    for (int i = 0; i < 7; i++)
    {
        printf("The number of columns with %d hits is: %d\n", i, arr[i]);
    }
    printf("\n\n");
}

/* The function asks the user to insert chosen manual col as a List */
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

/* The function saves the lottery results to a .bin file*/
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
    free(compressedResult);
}

/* The function compresses a single col into 3 bytes */
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

/* The function decompresses 3 bytes to a single col */
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

/* The function sorts the cols by hits desc.*/
void sortColsByHits(pList* participants)
{
    Participant* p = participants->head;

    while (p != NULL)
    {
        MergeSort(&(p->data->cols.head));
        p = p->next;
    }
}

/* The function lookups for hits and populates the hits field inside each col node */
void lookupForHits(pList* participants, int* lotteryResult, int** sum)
{
    Participant* p = participants->head;
    *sum = (int*)ourCalloc(sizeof(int), 7);
    while (p != NULL)
    {
        checkHitsForParticipant(p, lotteryResult, sum);
        p = p->next;
    }
}

/* The function compares the colList of participant to the winner lottery col, and counts the hits */
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
        }
        (*arr)[currCol->hits]++;
        currCol = currCol->next;
    }
}

/* The function decides on the winner column and prints it */
int* getLotteryResult()
{
    int* result = getAutomaticCol();

    printf("The winning column is:\t");
    printCol(result,false);
    printf("\n\n");

    return result;
}

/* The function receives a participant name (unknown size) */
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

/* The function gets an automated colList */
void getListFromAutomator(colList* colLst, int numOfCols)
{
    for (int col = 0; col < numOfCols; col++)
    {
        int* autoCol = getAutomaticCol();
        insertDataToEndList(colLst, autoCol);
    }
}

void freeSavedRes(latestResult* savedRes)
{
    if (savedRes != NULL)
    {
        if (savedRes->lotteryResult != NULL)
            free(savedRes->lotteryResult);
        if (savedRes->participants != NULL)
            freePList(*savedRes->participants);
        free(savedRes);
    }
}