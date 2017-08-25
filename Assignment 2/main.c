/***************
Simon Ko 0906762
CIS3110 Assignment 2
2/27/2017
****************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void roundRobin(int detailedFlag, int verboseFlag, int quantumValue);
void firstComeFirstServe(int verboseFlag, int detailedFlag);

int main(int argc, char const *argv[])
{
	int detailedFlag = 0;
	int verboseFlag;
	int roundRobinFlag;
	int quantumValue = 0;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i],"-d") == 0)
		{
			//Assign detailed flag
			detailedFlag = 1;
		}
		if (strcmp(argv[i],"-v") == 0)
		{
			//Assign verbose flag
			verboseFlag = 1;
		}
		if (strcmp(argv[i],"-r") == 0)
		{
			//Assign roundrobin flag else FCFS
			roundRobinFlag = 1;
			if (argv[i+1] != NULL)
			{
				quantumValue = atoi(argv[i+1]); //Get quantum value
			}
			else
			{
				printf("forgot to enter quantumValue\n");
				return -1;
			}				

		}
	}

	if (roundRobinFlag == 1)
	{
		roundRobin(detailedFlag, verboseFlag , quantumValue);
		//call function for round robin
	}
	else
	{
		firstComeFirstServe(detailedFlag,verboseFlag);
		//call function for first come first serve
	}

}

void roundRobin(int detailedFlag, int verboseFlag, int quantumValue)
{
	int numProcess = 0;
	int threadSwitch = 0;
	int processSwitch = 0;

	int procNum = 0;
	int numThread = 0;
	int totalThread = 0;
	int threadNum = 0;
	int arrivalTime = 0;
	int numberCPU = 0;

	int number = 0;
	int cpuTime = 0;
	int ioTime = 0;

	int totalIOtime = 0;
	int totalCPUtime = 0;

	int ioTimeTurnAround = 0;
	int cpuTimeTurnAround = 0;
	int turnaroundTime;
	int finishTime;
	int totalTurnaroundTime = 0;
	
	float avTurn;
	float cpuUtil;

	int l = 0;
	int data[500][7];
	int clock_time = 0;
	int subtract;
	int temp;
	int token = 0;
	int token2 = 0;
	int threadSwitchCount = 0;
	int processSwitchCount = 0;
	int currentProcessValue = 0;
	int currentThreadValue = 0;

	scanf("%d %d %d",&numProcess, &threadSwitch, &processSwitch ); //parse text value
	for (int i = 1; i <= numProcess; ++i)
	{
		scanf("%d %d", &procNum, &numThread);
		totalThread = totalThread + numThread;
		for (int j = 1; j <= numThread; ++j)
		{
			scanf("%d %d %d", &threadNum, &arrivalTime, &numberCPU);

			for (int k = 1; k < numberCPU; ++k)
			{
				scanf("%d %d %d", &number, &cpuTime, &ioTime);
				totalIOtime = ioTime + totalIOtime;
				totalCPUtime = cpuTime + totalCPUtime;


				ioTimeTurnAround = ioTime + ioTimeTurnAround;
				cpuTimeTurnAround = cpuTime + cpuTimeTurnAround;
			}
			scanf("%d %d", &number, &cpuTime); //final CPU time
			totalCPUtime = cpuTime + totalCPUtime;
			cpuTimeTurnAround = cpuTime + cpuTimeTurnAround;
			//assign values to an array
			data[l][0] = i; //process
			data[l][1] = j; //thread
			data[l][2] = totalIOtime; //totalio
			data[l][3] = totalCPUtime; //total cpu
			data[l][4] = arrivalTime; //arrival
			data[l][5] = totalCPUtime+totalIOtime; //remaining time is total cputime
			l++;

			totalCPUtime = 0;
			totalIOtime = 0;
		}

	}

	for (int n = 0; n < l; ++n) //sort by arrival time by brute force
	{
		for (int p = 0; p < l; ++p)
		{
			if (data[n][4] < data[p][4])
			{
				temp = data[p][0];
				data[p][0] = data[n][0];
				data[n][0] = temp;

				temp = data[p][1];
				data[p][1] = data[n][1];
				data[n][1] = temp;

				temp = data[p][2];
				data[p][2] = data[n][2];
				data[n][2] = temp;

				temp = data[p][3];
				data[p][3] = data[n][3];
				data[n][3] = temp;

				temp = data[p][4];
				data[p][4] = data[n][4];
				data[n][4] = temp;

				temp = data[p][5];
				data[p][5] = data[n][5];
				data[n][5] = temp;
			}
		}
	}

	if (verboseFlag == 1)
	{
		for (int n = 0; n < l; ++n)
		{
			printf("At time: %d Thread %d of process %d moves from new to ready\n", data[n][4],data[n][1],data[n][0]);
		}
	}


	for (int m = 0; m <= l; ++m) //RR algorithm
	{
		for (int q = 0; q <= l; ++q) //check if any remaining time left in any array
		{
			if (data[q][5] == 0)
			{
				token++;
			}
		}

		if (token == l) //if there is then rr end algorithm
		{
			break;
		}
		token = 0;
		
		if (data[m][1] != currentThreadValue)
		{
			threadSwitchCount++;
			clock_time = clock_time + threadSwitch;
		}
		
		if (data[m][0] != currentProcessValue)
		{
			processSwitchCount++;
			clock_time = clock_time + processSwitch;
		}



		if (verboseFlag == 1 && token2 == 0 )
		{
			printf("At time: %d Thread %d of process %d moves from ready to running\n", clock_time, data[m][1], data[m][0]);
		}
		
		if (data[m][5] > 0)//if there is still more remaining cpu
		{	
			subtract = data[m][5] - quantumValue; //subtract quantum
			currentProcessValue = data[m][0];
			currentThreadValue = data[m][1];

			if (subtract > 0) //if there is still time reamaining
			{
				clock_time = clock_time + quantumValue; //increase clock time
				data[m][5] = data[m][5] - quantumValue; //decrease cpu left
				if (verboseFlag == 1)
				{
					printf("At time: %d Thread %d of process %d moves from running to blocked\n", clock_time, data[m][1], data[m][0]);
					//at quantum time: move from
				}
			}
			else //less cpu time than quantum
			{
				clock_time = clock_time + data[m][5]; //increase time by remaining
				finishTime = clock_time;
				data[m][5] = 0;
				turnaroundTime = finishTime - data[m][4]; //finish - arrival
				
				if (verboseFlag == 1)
				{
					printf("At time: %d Thread %d of process %d moves from running to termination\n", clock_time, data[m][1], data[m][0]);
					token2 = 1;
				}			
				if (detailedFlag == 1)
				{			
					printf("\nThread %d of Process %d\n\n", data[m][1],data[m][0]);
					printf("Arrival time: %d\n", data[m][4]);
					printf("Service time: %d\n", data[m][3]);
					printf("I/O time: %d\n", data[m][2]);
					printf("Turnaround Time: %d\n", turnaroundTime); //clock time-arrival
					printf("Finish time: %d\n\n", finishTime); //clock time
				}

				totalTurnaroundTime = totalTurnaroundTime + turnaroundTime; //store turnaround time
			}

		}
		if (m == l) //loop back to beginning
		{
			m = 0;
		}
	}	

	//finishTime = finishTime + (processSwitch*processSwitchCount) + (threadSwitch*threadSwitchCount);
	printf("\nTotal time required is %d units\n", finishTime);
	avTurn = (totalTurnaroundTime)/(totalThread);
	printf("Average turnaround time is %.1f time units\n", avTurn);
	cpuUtil = (cpuTimeTurnAround * 200)/(finishTime);
	printf("CPU Utilization is %.0f%%\n",cpuUtil);
	return;
}

void firstComeFirstServe(int detailedFlag, int verboseFlag)
{
	int numProcess = 0;
	int threadSwitch = 0;
	int processSwitch = 0;

	int procNum = 0;
	int numThread = 0;
	int totalThread = 0;
	int threadNum = 0;
	int arrivalTime = 0;
	int numberCPU = 0;

	int number = 0;
	int cpuTime = 0;
	int ioTime = 0;

	int totalIOtime = 0;
	int totalCPUtime = 0;

	int cpuTimeTurnAround = 0;
	int turnaroundTime;
	int finishTime;
	int totalTurnaroundTime = 0;
	
	float avTurn;
	float cpuUtil;
	int data[100][7]; 
	int l = 0;
	int temp;

	int threadSwitchCount = 0;
	int processSwitchCount = 0;
	int currentThreadValue = 0;
	int currentProcessValue = 0;

	scanf("%d %d %d",&numProcess, &threadSwitch, &processSwitch );
	for (int i = 1; i <= numProcess; ++i)
	{
		scanf("%d %d", &procNum, &numThread);
		totalThread = totalThread + numThread;
		for (int j = 1; j <= numThread; ++j)
		{
			scanf("%d %d %d", &threadNum, &arrivalTime, &numberCPU);

			for (int k = 1; k < numberCPU; ++k)
			{
				scanf("%d %d %d", &number, &cpuTime, &ioTime);
				
				totalIOtime = ioTime + totalIOtime;
				totalCPUtime = cpuTime + totalCPUtime;
			}
			scanf("%d %d", &number, &cpuTime); //final CPU time
			totalCPUtime = cpuTime + totalCPUtime;
			
			data[l][0] = i; //process
			data[l][1] = j; //thread
			data[l][2] = totalIOtime; //totalio
			data[l][3] = totalCPUtime; //total cpu
			data[l][4] = arrivalTime; //arrival
			l++;

			totalCPUtime = 0;
			totalIOtime = 0;
		}
	}

	//store everything
	//sort by arrival brute force
	for (int n = 0; n < l; ++n) //set in arrival time
	{
		for (int p = 0; p < l; ++p)
		{
			if (data[n][4] < data[p][4])
			{
				temp = data[p][0];
				data[p][0] = data[n][0];
				data[n][0] = temp;

				temp = data[p][1];
				data[p][1] = data[n][1];
				data[n][1] = temp;

				temp = data[p][2];
				data[p][2] = data[n][2];
				data[n][2] = temp;

				temp = data[p][3];
				data[p][3] = data[n][3];
				data[n][3] = temp;

				temp = data[p][4];
				data[p][4] = data[n][4];
				data[n][4] = temp;
			}
		}
	}

	if (verboseFlag == 1)
	{
		for (int n = 0; n < l; ++n)
		{
			printf("At time: %d Thread %d of process %d moves from new to ready\n", data[n][4],data[n][1],data[n][0]);
		}
	}



	for (int m = 0; m < l; ++m) //fcfs algorithm
	{
		if (data[m][1] != currentThreadValue)
		{
			threadSwitchCount++;
		}

		if (data[m][0] != currentProcessValue)
		{
			processSwitchCount++;
		}
		turnaroundTime = data[m][2] + data[m][3] - data[m][4]; //finish - arrival time
		finishTime = data[m][4] + turnaroundTime + finishTime; //arrival + turnaround

		if (verboseFlag == 1)
		{
			printf("At time: %d Thread %d of process %d moving from ready to running\n",finishTime-data[m][2]-data[m][3], data[m][1],data[m][0]);
		}

		if (detailedFlag == 1)
		{
			printf("\nThread %d of Process %d\n\n", data[m][1],data[m][0]);
			printf("Arrival time: %d\n", data[m][4]);
			printf("Service time: %d\n", data[m][3] );
			printf("I/O time: %d\n", data[m][2]);
			printf("Turnaround Time: %d\n", turnaroundTime);
			printf("Finish time: %d\n", finishTime );
		}
		totalTurnaroundTime = totalTurnaroundTime + turnaroundTime;
		currentThreadValue = data[m][1];
		currentProcessValue = data[m][0];
		
		if (verboseFlag == 1)
		{
			printf("At time: %d Thread %d of process %d moving from running to termination\n",finishTime, data[m][1],data[m][0]);
		}

		cpuTimeTurnAround = data[m][3] + cpuTimeTurnAround; //keep on adding cpu time		
	}

	//finishTime = finishTime + (processSwitch*processSwitchCount) + (threadSwitch*threadSwitchCount);
	printf("\nTotal time required is %d units\n", finishTime);
	avTurn = (totalTurnaroundTime)/(totalThread);
	printf("Average turnaround time is %.1f time units\n", avTurn);
	cpuUtil = (cpuTimeTurnAround * 200)/(finishTime);
	printf("CPU Utilization is %.0f%%\n",cpuUtil);

	return;
}