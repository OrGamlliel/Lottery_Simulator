#ifndef participants_h
#define participants_h


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
void printPartipant(Participant* p);

#endif // participants_h
