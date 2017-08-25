README

CIS3110 Assignment 3
Simon Ko
0906762
3/23/2017


*To Compile*
To compile, type 'make'

*To Run*
To run the Philosophers dining problem, type: ./diner [number of philosophers] [number times to eat]
To run the Memory management simulation, type: ./holes [file.txt]

*Operation*
The philosophers dining algorithm takes a number of philosphers, and tries to make them eat. However, when
a left or right is picked up, there is a mutex lock and so the other philosophers beside them cannot eat until
the current philosopher has finished eating.
The philosophers think, eat and finish eating when the number of times to eat is 0.

For memory management, there are 4 simulations, best fit, worst fit, first fit and next fit.
After reading the text file, the next process id is attempted to be filled accoring to the management technique.
If there is not enough space in memory, the oldest filled process id are emptied until there is enough space.
All holes that are adjacent are merged. Each time there is a load, the number of processes, the number of holes,
% memory, %cumulative memory are printed. At the end, the total load and average information is printed.

*Addtional*

Algorithm was given some help during my lab time with respect to the memory management
