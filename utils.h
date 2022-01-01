#pragma once
#pragma warning(disable:4996)

#ifndef utils_h
#define utils_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define ERROR 1
#define INIT_SIZE 2

#endif // utils_h

FILE* ourFileOpen(char*, char*);
void ourFileClose(FILE*);
void validateFOpen(FILE*);
void validateFClose(int);
void* ourMalloc(int);
void* ourRealloc(void*, int);
void* ourCalloc(int, int);
void validateMemAlloc(void*);
void exitWithMessage(char*);
void assertTrue(bool, char*);
