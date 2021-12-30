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
void insertDataToStartPList(pList* pList, Data* participantData);
void insertParticipantToHead(pList* pList, Participant* newHead);
void printPList(pList pList);
printPartipant(Participant* p);

#endif // participants_h
