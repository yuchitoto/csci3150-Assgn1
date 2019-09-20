#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFF_SIZE 65535

//strcpy to some other array untill | or \0 met

int shell_execute(char ** args, int argc)
{
	int child_pid, wait_return, status;
	int p1[2], p2[2];
	char **poi;
	char buf[BUFF_SIZE] = {0};

	int const stdout_cp = dup(STDOUT_FILENO);
	int const stdin_cp = dup(STDIN_FILENO);

	if(pipe(p1)<0)
	{
		printf("Pipe1 not created\nEXIT\n");
		exit(-1);
	}

	if(pipe(p2)<0)
	{
		printf("Pipe2 not created\nEXIT\n");
		exit(-1);
	}

	if ( strcmp(args[0], "EXIT") == 0 )
		return -1;

	int i=0;
	while(args[i]!=NULL)
	{
		printf("%s ",args[i]);
		i++;
	}
	printf("\n");

	i = 0;
	int k=0, j;
	while(args[i]!=NULL)
	{
		if(strcmp(args[i],"|") || args[i+1] == NULL)
		{
			j=i-1;
			poi = (char**)malloc((j-k+2)*sizeof(char*));
			for(int me = 0; me<=j-k; me++)
			{
				*poi = (char*)malloc(sizeof(args[me]));
				strcpy(poi[me],args[me+k]);
			}
			poi[j-k+1]=NULL;
			//process forking
			if((child_pid = fork()) < 0)
			{
				printf("fork() error\n");
			}
			else if(child_pid == 0) //fork success child
			{
				if(args[i] == "|")
				{
					close(p1[1]);
					close(STDIN_FILENO);
					dup(p1[0]);
					close(p2[0]);
					close(STDOUT_FILENO);
					dup(p2[1]);
					if(execvp(poi[0], poi)<0)
					{
						printf("execvp() error\n");
						exit(-1);
					}
					else
						exit(0);
				}
				else
				{
					close(p1[1]);
					close(STDIN_FILENO);
					dup(p1[0]);
					close(p1[0]);
					close(p2[1]);
					close(p2[0]);
					if(execvp(poi[0],poi)<0)
					{
						printf("execvp() error\n");
						exit(-1);
					}
					else
						exit(0);
				}
			}
			else
			{
				//parent
				close(p1[0]);
				close(STDOUT_FILENO);
				dup(p1[1]);
				close(p1[1]);
				write(p1[1],buf,BUFF_SIZE);
				dup(stdout_cp);

				if(wait_return = wait(&status) < 0)
					printf("wait() error\n");

				if(args[i+1]!=NULL)
				{
					memset(buf, 0, sizeof(buf));
					read(p2[0], buf, sizeof(buf));
				}
			}
			//update index
		}
	}

	/*if( (child_pid = fork()) < 0 ){
		printf("fork() error \n");
	}else if (child_pid == 0 ){
		if ( execvp(args[0], args) < 0){
			printf("execvp() error \n");
			exit(-1);
		}
	}else{
		if ( (wait_return = wait(&status) ) < 0 )
			printf("wait() error \n");
	}*/

	return 0;

}
