#ifndef colList_h
#define colList_h

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#define MAX_NUM_IN_COLS 6

typedef struct colNode {
    int* col;             //col is an array of ints
    int hits;
    struct colListNode* next;//ptr to the next col
} colNode;

typedef struct colList {
    colNode* head;
    colNode* tail;
} colList;

void makeEmptyColList(colList* colst);
void insertDataToEndList(colList* colList, int* col);
int* getAutomaticCol();
bool isDifferent(int* arr, int val, int index);
bool isWithinRange(int number);
void printCol(int* col);
void printColNode(colNode*);

#endif // colList_h
