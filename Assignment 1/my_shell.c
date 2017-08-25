#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


extern char **getln();


int main() {
	int i;
	char **args; 

	int p = 0;
	char letter[10];
	int count = 0;
	int valid;
	pid_t pid;
	int token;
	int status;
	char ** argsS = calloc(100, sizeof(char *));
	int length;
	char str[80];

	 //loops and parses, and displays each argument
	while(1) 
	{
		printf(">");
		args = getln();
			if (args[0] == NULL) //no command
			{
				printf("Invalid command\n");
			}
			else if (strcmp(args[0], "exit") == 0) //command is exit
			{
				exit(1);
			}
			else if (strcmp(args[0], "add") == 0) //command is add
			{
				count = 0;
				for (i = 1; args[i] != NULL; i++)
				{
					count++;
				}

				for (i = 1; args[i] != NULL; i++)
				{
					if (i < count)
					{
						printf("%s + ", args[i]);
					}
					else
					{
						printf("%s", args[i]);
					}
				}
				count = 0;
				for(i = 1; args[i] != NULL; i++)
				{								
						strcpy(letter,args[i]);
						if (letter[1] == 'x')
						{
							//printf("This is a hex%s\n", letter);
							valid = (int)strtol(args[i], NULL,0);						
						}
						else if (isdigit(*args[i]))
						{
							//printf("this is int\n");
							valid = atoi(letter);
						}
						else
						{
							//printf("incorrect\n");
							count = 0;
							valid = 0;
						}
							count = valid + count;

				}
				printf(" = %d\n", count);
			}
			else if (strcmp(args[0], "zero") == 0) //command is zero
			{
				if (isdigit(*args[1]))
				{
					int numZero = (int)strtol(args[1], NULL, 0);
					for (i = 0; i < numZero ; i++)
					{
						printf("0 ");
					}
					printf("\n");
				}
				else
				{
					printf("error\n");
				}

			}
			else if (strcmp(args[0], "arg") == 0) //command is number of arguments
			{
				count = 0;
				token = 0;
				length = 0;
				strcpy(str, "");

				for(i = 1; args[i] != NULL; i++) //go through each argument
				{			
					strcpy(letter, args[i]);
					length = strlen(letter);
					if ((letter[0] == '\"') && (token == 0)) //if begins at quote
					{
						count++;
						token = 1;
					}
					else if (letter[length-1] == '\"')  //if quote ends
					{
						token = 0;
					}
					else if (token == 0) //if outside of quotes
					{
						count++;
					}
					else if (token == 1) //this must be inside quotes
					{
						/* do nothing */
					}

					strcat(str,letter);

					if (token == 0)
					{
						strcat(str,","); //if outside quotes then add ,
					}
					if (token == 1)
					{
						strcat(str, " "); //if inside quotes then add " "
					}
				}

				printf("argc = %d args = ",count);
				length = strlen(str);
				str[length-1] = ' '; //remove the last ,
				printf("%s\n",str);
			}
			else if (args[1] != NULL) //more than one command
			{
				count = 0;
				for(i = 1; args[i] != NULL; i++)
				{			
					count++;
				}

				if (strcmp(args[count], "&") == 0)
				{
					//printf("Last command is '&'\n");
					args[count] = NULL;
					/* shell executes command in the background. does not wait for command to finish */
					/* ls & gives ls command in child*/
					int pid = fork();

					for (int i = 0; args[i] != NULL; ++i)
					{
						argsS[p] = args[i];
						p++;
					}
					argsS[p+1] = NULL;

					if (pid < 0)
					{
						printf("error\n");
						exit(0);
					}

					if (pid == 0)
					{
						printf("Executing in child: %d\n", getpid());
						execvp(args[0],argsS);
					}

					waitpid(pid, &status, 0);
					printf("Back to main: %d\n", getpid());
				}
				else //,ls -l: -rw-rw-r-- 1 simon simon    63 Jan 20 16:00 foo
				// ls -l > foo: write to foo
				// sort < foo: sort out from foo
				{
					p = 0;
					int stored = 0;
					int stored2 = 0;

					pid = fork();

					if (pid < 0)
					{
						printf("error\n");
						exit(0);
					}

					if (pid == 0)
					{

						for (i = 0; args[i] != NULL; ++i)
						{
							argsS[p] = args[i];
							if (strcmp(">", args[i]) == 0)
							{	
								freopen(args[i+1], "w", stdout); //write to next argument
								stored = 1; // null is set to 2 for ls -l > foo	

							}
							else if (strcmp("<", args[i]) == 0)  //else if (letter[0] == '<')
							{
								freopen(args[i+1], "r", stdin);
								stored2 = 1;
							}
							p++;
						}
						//printf("%d\n",p );
						if ((stored == 1) || (stored2 == 1))
						{
							p=p-3;
						}
						//set last argument to null //ls -l = argsS = 2
						argsS[p+1] = NULL;

						execvp(args[0],argsS);
					}
					else
					{
						wait(&status);
					}

				}
			}
			else //single command
			{
				pid = fork();

				if (pid < 0)
				{
					printf("error\n");
					exit(0);
				}

				if (pid == 0)
				{
					args[1] = NULL;
					execvp(args[0],args);
				}
				else
				{
					wait(&status);
				}
			}
	}
	return 0;
}

