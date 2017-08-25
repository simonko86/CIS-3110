#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extras.h"

void printStatus(int curProces,int curHoles,int memoryTaken,int totalMemory,int i)
{
    printf("pid loaded,");
    printf(" # processes = %d,",curProces);
    printf(" num holes = %d,",curHoles);
    printf(" %%memusage = %d,",(memoryTaken*100)/128);
    printf(" cumulative %%mem = %d\n",totalMemory/((i+1)*128));
    return;
}

void printEnd(int totalLoads, int totalProcess, int totalHoles, int totalMemory, int i)
{
    printf("Total loads = %d,", totalLoads);
    printf(" average num processes = %d,",totalProcess/i );
    printf(" average num holes = %d,",totalHoles/i);
    printf(" cumulative %%mem = %d\n",totalMemory/(i*128));
    return;
}

int countHoles(Space *memory)
{
    Space *temp = memory;
    int count = 0;

    while(temp!=NULL)
    {
        if(temp->gap)
        {
            count++;
        }
        temp = temp->next;
    }
    return count;
}

Space *merge(Space *memory)
{
    Space * temp = memory;
    Space * temp2 = memory;

    if(temp->next == NULL)
    {
        return temp;
    }

    while(temp2->next->next != NULL)
    {
        if(temp2->next->gap&&temp2->next->next->gap)
        {
            Space *temp = makeSpace('-',(temp2->next->size)+(temp2->next->next->size),1,0,0);
            temp->next = temp2->next->next->next;
            temp2->next = temp;
            continue;
        }
        temp2=temp2->next;
    }


    return temp;
}

Space *makeSpace(char processName, int size, int gap, int amounts,int lifeTime)
{
    Space *temp = malloc(sizeof(Space));

    temp->processName=processName;
    temp->size=size;
    temp->gap=gap;
    temp->amounts=amounts;
    temp->lifeTime=lifeTime;
    temp->next=NULL;

    return temp;
}

Line *makeprocessName(char processName, int size,int amounts)
{
    Line *temp = malloc(sizeof(Line));

    temp->processName=processName;
    temp->size=size;
    temp->amounts=amounts;
    temp->next=NULL;
    
    return temp;
}

Line *push(Line *process,int processName, int size, int amounts)
{
    Line *temp = process;
    if(temp == NULL)
    {
        return makeprocessName(processName,size,amounts);
    }

    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = makeprocessName(processName,size,amounts);
    return process;
}

Line *pop(Line **process)
{
    Line *toReturn = (*process);
    if(!toReturn)
    {
        return NULL;
    }
    else
    {
        *process=(*process)->next;
    }
    return toReturn;
}