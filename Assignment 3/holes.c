#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extras.h"

void firstFit(Line *process, int loops);
void nextFit(Line *process, int loops);
void bestFit(Line *process, int loops);
void worstFit(Line *process, int loops);

int main(int argc, char const *argv[])
{
	char word [255];
	FILE *file;
	int i = 0;
	char* type;
	int type2;
    char* size;
    int sizeNum;
	int loops;
    Line *process = NULL;

	file = fopen(argv[1], "r");

	while(!feof(file))
	{
		while (fgets(word,255,file) != NULL)
		{
			word[strlen(word)-1] = '\0';
			type = strtok(word, " ");
			type2 = atoi(type);
			size = strtok(NULL, type);
			sizeNum = atoi(size);
			//printf("%s %d %d\n",type, sizeNum, i);			
			process = push(process,type2,sizeNum,0);
			i++;
		}
	}
	loops = i*3;
	printf("\nFirst fit\n");
	firstFit(process, loops);
	printf("\nNext fit\n");
	nextFit(process, loops);
    printf("\nBest fit\n");
    bestFit(process, loops);
	printf("\nWorst fit\n");
	worstFit(process, loops);
	return 0;
}

void firstFit(Line *process, int loops)
{
    int i;
    int working = 1;
    int old=0;
    int numHoles=1;
    int curHoles=1;
    int numProcess=0;
    int curProces=0;
    int numLoads=0;
    int numMemory=0;
    int memoryTaken=0;
    char dash = '-';

    Line *clone = NULL;
    Line *temp = process;
    
    while(temp!=NULL)
    {
        clone=push(clone,temp->processName,temp->size,0);
        temp=temp->next;
    }
    Space *memory = makeSpace(dash,128,1,0,0);

    for(i=0;i < loops;++i)
    {
        Space *temp = memory;
        Line *added = pop(&clone);
        while(working == 1)
        {
            temp = memory;
            temp = merge(temp);

            curHoles = countHoles(temp);

            if(temp->gap || temp->next ==NULL)
            {
                if(temp->size+1 > added->size)
                {
                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,temp->size-added->size,1,0,0);

                    if(temp->size != added->size)
                    {
                        newMem->next = newH;
                        newH->next = temp->next;
                    }
                    else
                    {
                        newMem->next=temp->next;
                    }
                    curProces++;
                    numLoads++;
                    
                    memory = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    numHoles = numHoles + curHoles;
                    numProcess = numProcess + curProces;
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);
                    break;
                }
            }

            while(temp->next != NULL)
            {
                
                if((temp->next->gap) && (temp->next->size > added->size+1))
                {
                    curProces++;
                    numProcess= numProcess + curProces;
                    numHoles = numHoles + curHoles;
                   
                    Space *newMem=makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,(temp->next->size)-(added->size),1,0,0);
                    numLoads++;
                    if(temp->next->size == added->size)
                    {
                        newMem->next=temp->next->next;
                    }
                    else
                    {
                        newMem->next = newH;
                        newH->next = temp->next->next;
                    }

                    temp->next = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);         
                    goto exit1;
                }
                temp = temp->next;
            }

            Space *leave = memory;
            while(leave!=NULL)
            {
                if((leave->lifeTime == old) &&! (leave->gap))
                {
                    memoryTaken-=leave->size;
                    clone = push(clone,leave->processName,leave->size,leave->amounts+1);
                    leave->gap=1;
                    curProces--;
                    old++;
                    break;
                }
                leave=leave->next;
            }
        }
        exit1: free(added);
    }
    printEnd(numLoads, numProcess, numHoles, numMemory, i);
}

void nextFit(Line *process, int loops)
{
    int i;
    int working = 0;
    int old=0;
    int numProcess=0;
    int numHoles=1;
    int curProces=0;
    int curHoles=1;
    int numLoads=0;
    int numMemory=0;
    int memoryTaken=0;
    char last = '-';
    char saved;
    int flag = 0;
    char dash = '-';

    Line *clone = NULL;
    Line *tempMem = process;
    
    while(tempMem != NULL)
    {
        clone = push(clone,tempMem->processName,tempMem->size,0);
        tempMem = tempMem->next;
    }
    Space *memory = makeSpace(dash,128,1,0,0);

    for(i=0;i < loops;++i)
    {
        Space *temp = memory;
        Line *added = pop(&clone);
        working = 0;

        while(working != 1)
        {
            temp = memory;
            temp = merge(temp);

            curHoles = countHoles(temp);

            if(last == '-')
            {
                working = 1;
                curProces++;
                numHoles = numHoles + curHoles;
                numProcess= numProcess + curProces;
                Space *newH = makeSpace(dash,temp->size-added->size,1,0,0);
                Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                newMem->next = newH;
                memory = newMem;
                memoryTaken = memoryTaken + newMem->size;
                numMemory = memoryTaken + (memoryTaken*100);
                numLoads++;
                last = added->processName;
                printStatus(curProces,curHoles,memoryTaken,numMemory,i);
                goto exit1;
            }

            while((temp->next!=NULL) && (temp->processName != last))
            {
                temp=temp->next;
            }

            flag = 0;
            saved = last;

            while(1)
            {
                if((flag == 1) && (temp->next!=NULL) && (saved == temp->next->processName))
                {
                    break;
                }
               
                if(temp->next!=NULL && temp->next->gap && temp->next->size >= added->size)
                {
                    
                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,temp->next->size-added->size,1,0,0);
                    if(temp->next->size!=added->size)
                    {
                        newMem->next = newH;
                        newH->next = temp->next->next;
                    }
                    else
                    {
                        free(newH);
                        newMem->next = temp->next->next;
                    }
                    numLoads++;
                    curProces++;
                    last = added->processName;
                    
                    temp->next = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    numProcess = numProcess + curProces;
                    numHoles = numHoles + curHoles;
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);

                    goto exit1;

                }
                
                if(temp->next == NULL)
                {

                    last = added->processName;

                    if(memory->gap && memory->size >= added->size)
                    {
                        Space *newH = makeSpace(dash,memory->size-added->size,1,0,0);
                        Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                        
                        if(memory->size != added->size)
                        {
                            newMem->next=newH;
                            newH->next = memory->next;
                        }
                        else
                        {
                            free(newH);
                            newMem->next = temp->next->next;
                        }
                        last = added->processName;
                        numLoads++;
                        curProces++;
                        
                        memory = newMem;
                        numProcess = numProcess + curProces;
                        numHoles = numHoles + curHoles;
                        memoryTaken = memoryTaken + (newMem->size);
                        numMemory = numMemory + (memoryTaken*100);
                        printStatus(curProces,curHoles,memoryTaken,numMemory,i);
                        goto exit1;
                    }
                    else if(memory->gap && memory->size < added->size)
                    {
                        temp = memory;   
                        break;
                    }
                    
                    temp = memory;
                    break;
                }
                else
                {
                    temp = temp->next;
                }
                flag = 1;
            }

            Space *leave = memory;
            while(leave != NULL)
            {
                if((leave->lifeTime == old)&&!(leave->gap))
                {   
                    curProces--;
                    old++;
                    memoryTaken = leave->size;
                    clone = push(clone,leave->processName,leave->size,leave->amounts+1);
                    leave->gap = 1;
                    break;
                }
                leave=leave->next;
            }
        }
        exit1: free(added);
    }
    printEnd(numLoads, numProcess, numHoles, numMemory, i);
}

void bestFit(Line *process, int loops)
{
    int i;
    int working = 1;
    int old=0;
    int numProcess=0;
    int numHoles=1;
    int curProces=0;
    int curHoles=1;
    int numLoads=0;
    int numMemory=0;
    int memoryTaken=0;
    char dash = '-';

    int counter;
    int min;
    int save; 
    int failed = 0;

    Line *clone = NULL;
    Line *temp = process;
    
    while(temp!=NULL)
    {
        clone=push(clone,temp->processName,temp->size,0);
        temp=temp->next;
    }
    Space *memory = makeSpace(dash,128,1,0,0);

    for(i=0;i < loops;++i)
    {
        Space *temp = memory;
        Line *added = pop(&clone);
        while(working == 1)
        {
            temp = memory;
            temp = merge(temp);

            curHoles = countHoles(temp);

            if(temp->gap || temp->next ==NULL)
            {
                if(temp->size >= added->size)
                {
                    Space *newH = makeSpace(dash,temp->size-added->size,1,0,0);
                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);

                    if(temp->size != added->size)
                    {
                        newMem->next = newH;
                        newH->next = temp->next;
                    }
                    else
                    {
                        newMem->next=temp->next;
                    }

                    numLoads++;
                    curProces++;
                    
                    memory = newMem;
                    numProcess = numProcess + curProces;
                    numHoles = numHoles + curHoles;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);
                    break;
                }
            }

            counter=0;
            min = 100;
            save = -1;            
            temp = memory;

            while(temp->next != NULL)
            {             
                if((temp->next->gap) && (temp->next->size > added->size+1))
                {
                    if(min > ((temp->next->size)-(added->size)))
                    {
                        min = temp->next->size-added->size;
                        save = counter;
                    }
                    counter++;
                }
                temp = temp->next;
            }
            temp=memory;
            
            if(min != 100)
            {
                if(save == -1)
                {
                    if((!temp->gap) || (temp->size < added->size))
                    {
                        break;
                    }

                    Space *newH = makeSpace(dash,temp->size-added->size,1,0,0);
                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                    if(temp->size!=added->size)
                    {
                        newMem->next=newH;
                        newH->next = temp->next;
                    }
                    else
                    {
                        free(newH);
                        newMem->next=temp->next;
                    }
                    curProces++;
                    numLoads++;
                    
                    memory = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    numProcess = numProcess + curProces;
                    numHoles = numHoles + curHoles;
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);

                    goto exit1;
                }
                else
                {
                    failed = 0;
                    while(temp->next!=NULL)
                    {

                        if(temp->next->gap)
                        {
                            failed++;
                        }

                        if(failed == save+1)
                        {
                            break;
                        }
                        temp=temp->next;
                    }
                    numLoads++;
                    working = 1;
                    curProces++;
                    numHoles = numHoles + curHoles;
                    numProcess = numProcess + curProces;
                    
                    Space *newMem=makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,temp->next->size-added->size,1,0,0);
                    if(temp->next->size!=added->size)
                    {
                        newMem->next=newH;
                        newH->next = temp->next->next;
                    }
                    else
                    {
                        free(newH);
                        newMem->next=temp->next->next;
                    }
                    
                    free(temp->next);
                    temp->next = newMem;
                    memoryTaken = memoryTaken + newMem->size;
                    numMemory = numMemory + (memoryTaken*100);
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);
                
                    goto exit1;
                }
            }

            Space *leave = memory;
            while(leave!=NULL)
            {
                if((leave->lifeTime == old) &&! (leave->gap))
                {
                    memoryTaken-=leave->size;
                    clone = push(clone,leave->processName,leave->size,leave->amounts+1);
                    curProces--;
                    old++;
                    leave->gap = 1;
                    break;
                }
                leave=leave->next;
            }
        }
        exit1: free(added);
    }
    printEnd(numLoads, numProcess, numHoles, numMemory, i);
}

void worstFit(Line *process, int loops)
{
    int i;
    int working = 1;
    int old=0;
    int numProcess=0;
    int numHoles=1;
    int curProces=0;
    int curHoles=1;
    int numLoads=0;
    int numMemory=0;
    int memoryTaken=0;
    char dash = '-';

    int counter;
    int max;
    int save;
    int failed;

    Line *clone = NULL;
    Line *temp = process;
    
    while(temp!=NULL)
    {
        clone=push(clone,temp->processName,temp->size,0);
        temp=temp->next;
    }
    Space *memory = makeSpace(dash,128,1,0,0);

    for(i=0;i < loops;++i)
    {
        Space *temp = memory;
        Line *added = pop(&clone);
        while(working == 1)
        {
            temp = memory;
            temp = merge(temp);
            curHoles = countHoles(temp);

            if(temp->gap || temp->next ==NULL)
            {
                if(temp->size >= added->size)
                {
                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,temp->size-added->size,1,0,0);

                    if(temp->size != added->size)
                    {
                        newMem->next = newH;
                        newH->next = temp->next;
                    }
                    else
                    {
                        newMem->next=temp->next;
                    }
                    curProces++;
                    numLoads++;
                    
                    memory = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    numProcess = numProcess + curProces;
                    numHoles = numHoles + curHoles;
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);
                    break;
                }
            }

            save = max = -1;
            counter = 0;
            temp = memory;

            while(temp->next!=NULL)
            {
                if(temp->next->gap && temp->next->size >= added->size)
                {
                    if(max < temp->next->size-added->size)
                    {
                        max = temp->next->size-added->size;
                        save = counter;
                    }
                    counter++;
                }
                temp = temp->next;
            }

            temp=memory;
            
            if(max != -1)
            {
                if(save == -1)
                {
                    if((!temp->gap) || (temp->size < added->size))
                    {
                        break;
                    }

                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,temp->size-added->size,1,0,0);
                    if(temp->size != added->size)
                    {
                        newMem->next = newH;
                        newH->next = temp->next;
                    }
                    else
                    {
                        free(newH);
                        newMem->next=temp->next;
                    }
                    curProces++;
                    numLoads++;
                    
                    memory = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    numProcess = numProcess + curProces;
                    numHoles = numHoles + curHoles;
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);

                    goto exit1;
                }
                else
                {
                    failed = 0;
                    
                    while(temp->next != NULL)
                    {
                        if(temp->next->gap)
                        {
                            failed++;
                        }

                        if(failed == save+1)
                        {
                            break;
                        }
                        temp = temp->next;
                    }
                    numLoads++;
                    numHoles = numHoles + curHoles;
                    working = 1;
                    curProces++;
                    numProcess = numProcess + curProces;
                    
                    Space *newMem = makeSpace(added->processName,added->size,0,added->amounts,i);
                    Space *newH = makeSpace(dash,temp->next->size-added->size,1,0,0);
                    if(temp->next->size!=added->size)
                    {
                        newMem->next=newH;
                        newH->next = temp->next->next;
                    }
                    else
                    {
                        free(newH);
                        newMem->next = temp->next->next;
                    }
                    
                    free(temp->next);
                    temp->next = newMem;
                    memoryTaken = memoryTaken + (newMem->size);
                    numMemory = numMemory + (memoryTaken*100);
                    printStatus(curProces,curHoles,memoryTaken,numMemory,i);                
                    goto exit1;
                }
            }

            Space *leave = memory;
            while(leave != NULL)
            {
                if(leave->lifeTime == old&&!leave->gap)
                {
                    old++;
                    curProces--;
                    memoryTaken-=leave->size;
                    clone = push(clone,leave->processName,leave->size,leave->amounts+1);
                    leave->gap=1;
                    break;
                }
                leave = leave->next;
            }
        }
        exit1: free(added);
    }
    printEnd(numLoads, numProcess, numHoles, numMemory, i);
}


