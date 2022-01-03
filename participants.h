/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#ifndef participants_h
#define participants_h

#include "colList.h"
#include "utils.h"
#include <stdlib.h>

typedef struct data
{
    char* name;
    colList cols;
} Data;

typedef struct participant
{
    Data* data;
    struct participant* next;//ptr to the next participant
} Participant;

typedef struct plst {
    Participant* head;
    Participant* tail;
} pList;

void makeEmptyPList(pList* plst);
bool isEmptyPList(pList pList);
Data* createDataForParticipant(char* name, colList cols);
Participant* createNewParticipant(Data* participantData, Participant* next);
void insertPDataToEndPList(pList* pList, Data* participantData);
void insertPNodeToEndList(pList* pList, Participant* newTail);
void printPList(pList pList);
void printPartipant(Participant* p);
void freePList(pList pList);
void freeParticipant(Participant* p);

#endif // participants_h
