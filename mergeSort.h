//
//  mergeSort.h
//  finalProjectC
//
//  Created by Avital Rubichi on 03/01/2022.
//

#ifndef mergeSort_h
#define mergeSort_h

#include <stdio.h>
#include "colList.h"

void MergeSort(colNode** headRef);
colNode* SortedMerge(colNode* head1, colNode* head2);
void middle(colNode* source, colNode** frontRef, colNode** backRef);

#endif /* mergeSort_h */
