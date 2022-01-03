/* LOTTERY PROJECT
Created by:
Avital Rubichi 301789178
Or Gamliel 209161603*/

#include "utils.h"

//The function opens a file named fileName in a specific mode, and validate the success of opening.
//In case of success it will return a pointer to the opened file, otherwise the program will exit
FILE* ourFileOpen(char* fileName, char* mode)
{
	FILE* f = fopen(fileName, mode);
	validateFOpen(f);
	return f;
}

//The function closes a file and validate the success of closing.
//In case of failure the program will exit
void ourFileClose(FILE* filePtr)
{
	int fCloseStatus = fclose(filePtr);
	validateFClose(fCloseStatus);
}

//The function validates success of opening file. In case of failure the program will exit with a relevant message.
void validateFOpen(FILE* fptr)
{
	assertTrue(fptr != NULL, "failed opening the file");
}

//The function validates success of closing file. In case of failure the program will exit with a relevant message.
void validateFClose(int fCloseStatus)
{
	assertTrue(fCloseStatus != -1, "failed closing the file");
}

//The function allocates memory and validates memory allocation. In case of success it will return a pointer to the mallocated memory,
//otherwise the program will exit.
void* ourMalloc(int size)
{
	void* res = malloc(size);
	validateMemAlloc(res);
	return res;
}

//The function allocates memory and validates memory allocation. In case of success it will return a pointer to the mallocated memory,
//otherwise the program will exit.
void* ourCalloc(int numOfElements, int size)
{
	void* res = calloc(numOfElements, size);
	validateMemAlloc(res);
	return res;
}

//The function reallocated memory and validates memory reallocation. In case of success it will return a pointer to the reallocated memory,
//otherwise the program will exit.
void* ourRealloc(void* block, int size)
{
	void* res = realloc(block, size);
	validateMemAlloc(res);
	return res;
}

//The function validates success of allocating memory. In case of failure the program will exit with a relevant message.
void validateMemAlloc(void* ptr)
{
	assertTrue(ptr != NULL, "Failed to allocate memory");
}

//The function prints an error message, then exits the program.
void exitWithMessage(char* message)
{
	printf("%s\n", message);
	exit(ERROR);
}

//The function asserts a condition is true, otherwise exits the program with a message.
void assertTrue(bool condition, char* message)
{
	if (!condition)
		exitWithMessage(message);
}

