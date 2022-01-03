//
//  main.h
//  finalProjectC
//
//  Created by Avital Rubichi on 03/01/2022.
//
#define _CRT_SECURE_NO_WARNINGS

#ifndef main_h
#define main_h

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

#endif /* main_h */
