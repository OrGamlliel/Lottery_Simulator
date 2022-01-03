/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#define _CRT_SECURE_NO_WARNINGS

#ifndef screens_h
#define screens_h

#define WIN "windows"
#define UNKNOWN "unknown"

#if defined _WIN32
#define PLATFORM WIN
#else
#define PLATFORM UNKNOWN
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "lottery.h"
#include "colList.h"
#include "participants.h"
#include "utils.h"

void start();
void showMenu(latestResult* savedRes);
void freeList(colList* colList);
latestResult* firstOption();
void secondOption();
void askToSave(int** numOfAllColsArr, int* numOfParticipants, latestResult* saveRes);


#endif
