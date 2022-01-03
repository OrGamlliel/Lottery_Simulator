/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#ifndef mergeSort_h
#define mergeSort_h

#include <stdio.h>
#include "colList.h"

void MergeSort(colNode** headRef);
colNode* SortedMerge(colNode* head1, colNode* head2);
void middle(colNode* source, colNode** frontRef, colNode** backRef);

#endif /* mergeSort_h */
