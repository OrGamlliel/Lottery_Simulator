/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#include "mergeSort.h"

// Sorts the linked list by changing links
void MergeSort(colNode** headRef) {
    colNode* head = *headRef;
    colNode* newHead1;
    colNode* newHead2;

    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }
    middle(head, &newHead1, &newHead2);
    MergeSort(&newHead1);
    MergeSort(&newHead2);
    *headRef = SortedMerge(newHead1, newHead2);
}

// A helper function
colNode* SortedMerge(colNode* head1, colNode* head2)
{
    colNode* res = NULL;
    /* Base cases */
    if (head1 == NULL)
        return (head2);
    else if (head2 == NULL)
        return (head1);
    if (head1->hits >= head2->hits)
    {
        res = head1;
        res->next = SortedMerge(head1->next, head2);
    }
    else
    {
        res = head2;
        res->next = SortedMerge(head1, head2->next);
    }
    return (res);
}

//finds the middle node in the list
void middle(colNode* source, colNode** frontRef, colNode** backRef)
{
    colNode* slow;
    colNode* fast;
    slow = source;
    fast = source->next;

    //every loop the fast index advances 2 nodes and the slow advanced 1 node
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    //splits the list and define the head and tail in each
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
