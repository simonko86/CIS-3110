
#ifndef __SKO01_extras__ //Header guards incase header is already read
#define __SKO01_extras__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memory
{
    int lifeTime;
    int gap;
    int amounts;
    char processName;
    int size;
    struct memory *next;
}Space,Line;

void printStatus(int curProces,int curHoles,int memoryTaken,int totalMemory,int i);
void printEnd(int totalLoads, int totalProcess, int totalHoles, int totalMemory, int i);
Line *push(Line *process,int processName, int size, int amounts);
Line *makeprocessName(char processName, int size,int amounts);
Space *merge(Space *memory);
Space *makeSpace(char processName, int size, int gap, int amounts,int lifeTime);
Line *pop(Line **process);
int countHoles(Space * memory);



#endif