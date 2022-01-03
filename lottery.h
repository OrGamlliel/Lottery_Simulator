//
//  lottery.h
//  finalProjectC
//
//  Created by Avital Rubichi on 03/01/2022.
//

#ifndef lottery_h
#define lottery_h

#define MANUAL 1
#define AUTO 2
#define SHORT_COL 3
#define FILENAME "result.bin"

#include <stdio.h>
#include <string.h>
#include "mergeSort.h"
#include "participants.h"

typedef unsigned char BYTE;

typedef struct latestRes
{
    pList* participants;
    int* lotteryResult;
    Participant* winner;//shall we keep the winner as well?
}latestResult;

pList* getParticipants(int** numOfAllCols, int* numOfParticipants);
int getLotteryMode();
int getCols(colList* colLst, int lotteryMode);
void printMostSuccessfulParticipant(pList* participants);
void printSumOfHits(int* arr);
void saveResultsToBfile(pList* participants, int* numOfCols, int* lotteryResult, int numOfParticipants);
BYTE* singleColCompression(int* col);
int* singleColDecompression(BYTE* compressedCol);
char* getName();
void getListFromUser(colList* lstC, int numOfCols);
void getListFromAutomator(colList* colLst, int numOfCols);
int* getLotteryResult();
void lookupForHits(pList* participants, int* lotteryResult, int** sum);
void checkHitsForParticipant(Participant* p, int* lotteryResult, int** arr);
void sortColsByHits(pList* participants);

#endif /* lottery_h */
