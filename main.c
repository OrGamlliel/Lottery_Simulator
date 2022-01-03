//
//  main.c
//  finalProjectC
//
//  Created by Avital Rubichi on 03/01/2022.
//

#include "main.h"

void main()
{
    srand(time(NULL));
    start();
}

void start()
{
    showMenu(NULL);
}

void showMenu(latestResult* savedRes)
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
        savedRes = firstOption();
         break;
    case 2:
        if (savedRes == NULL)//read from file
        {
            secondOption();
        }
        else
        {
            printPList(*(savedRes->participants));
            printf("The winning col is: ");
            printCol(savedRes->lotteryResult);
            printf("\n\n\n");
            //keep winner and his score
        }
        break;
    case 3:
        exit(0);
        break;
    default:
        exitWithMessage("Invalid input");
        break;
    }

}

latestResult* firstOption()
{
    latestResult* saveRes = (latestResult*)ourMalloc(sizeof(latestResult));

    char userWilling;
    int** sumOfHits;
    int** numOfAllColsArr; //how many cols for each p
    int* numOfParticipants;

    numOfAllColsArr = (int**)ourMalloc(sizeof(int*));
    numOfParticipants = (int*)ourMalloc(sizeof(int));
    sumOfHits = (int**)ourMalloc(sizeof(int*));
    saveRes->participants = getParticipants(numOfAllColsArr, numOfParticipants);
    saveRes->lotteryResult = getLotteryResult();

    lookupForHits(saveRes->participants, saveRes->lotteryResult, sumOfHits);
    sortColsByHits(saveRes->participants);
    printPList(*(saveRes->participants));
    printSumOfHits(*sumOfHits);
    printMostSuccessfulParticipant(saveRes->participants);
    
    printf("Would you like to save to file the latest lottery result (Y/N)?\n");//we are goung to keep the lottery result anyway, we need to mention if to file- change wording
    getchar();
    scanf("%c", &userWilling);
    switch (userWilling)
    {
    case 'Y':
        saveResultsToBfile(saveRes->participants, *numOfAllColsArr, saveRes->lotteryResult, *numOfParticipants);
        showMenu(saveRes);
        break;
    case 'N':
        showMenu(saveRes);//The file won't be saved, but the latest lottery result will still exist and can be used
        break;
    default:
        exitWithMessage("Invalid input. exiting\n"); //need to decide
        break;
    }
    return saveRes;
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
    for (int i = 0; i < numOfParticipants; i++)
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
            BYTE* compressedCol = (BYTE*)ourMalloc(sizeof(BYTE) * SHORT_COL);
            fread(compressedCol, sizeof(BYTE), SHORT_COL, bf);
            int* decompressedCol = singleColDecompression(compressedCol);
            insertDataToEndList(&(currParData->cols), decompressedCol);
        }
    }
    BYTE* winCompressedCol = (BYTE*)ourMalloc(sizeof(BYTE) * SHORT_COL);
    fread(winCompressedCol, sizeof(BYTE), SHORT_COL, bf);
    int* winDecompressedCol = singleColDecompression(winCompressedCol);

    int** sumOfHits = (int**)ourMalloc(sizeof(int*));

    lookupForHits(participants, winDecompressedCol, sumOfHits);
    printPList(*participants);
    printf("The winning col is: ");
    printCol(winDecompressedCol);
    printf("\n");
}
