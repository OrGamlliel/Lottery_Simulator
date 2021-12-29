#ifndef colList_h
#define colList_h

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#define MAX_NUM_IN_COLS 6

typedef int* Col;

typedef struct collistNode {
    Col* col;             //col is a ptr to array
    struct colListNode* next;//ptr to the next col
} colListNode;

typedef struct colList {
    colListNode* head;
    colListNode* tail;
} colList;

void makeEmptyColList(colList* colst);
void insertDataToEndList(colList* colList, Col* col);
Col* getAutomaticCol();
bool isDifferent(Col arr, int val, int index);
void printCol(Col col);
void printColNode(colListNode*);

#endif // colList_h
