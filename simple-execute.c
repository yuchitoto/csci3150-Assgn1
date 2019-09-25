#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFF_SIZE 65535

//strcpy to some other array untill | or \0 met

void shell_execute1(char **args, int argc);

int shell_execute(char ** args, int argc)
{
	int child_pid, wait_return, status;

	if ( strcmp(args[0], "EXIT") == 0 )
		return -1;

	shell_execute1(args, argc);

	return 0;

}

void shell_execute1(char ** args, int argc)
{
	int child_pid, wait_return, status;
	int p1[2], p2[2];
	char **poi;
	char buf[BUFF_SIZE] = {0};

	int const stdout_cp = dup(STDOUT_FILENO);
	int const stdin_cp = dup(STDIN_FILENO);

	if ( strcmp(args[0], "EXIT") == 0 )
		return -1;

	int i=0;
	while(args[i]!=NULL)
	{
		printf("%s ",args[i]);
		i++;
	}
	printf("\n");

	printf("%d\n", argc);

	int k=0, j;
	for(int u=0;u<argc;u++) //detected SIGSEG
	{
		//unlimited loop
		if(strcmp(args[u],"|") == 0 || u==argc-1) //if compare a NULL array there will be segmentation fault
		{
			j=u-1;
			poi = malloc((j-k+2)*sizeof(char*));
			for(int me = 0; me<=j-k; me++)
			{
				poi[me] = strdup(args[me+k]);
			}
			poi[j+1] = NULL;

			for(int a=0;a<(j-k+2);a++)
				printf("%s\n",(poi[a]==NULL)?"someNULL":poi[a]);
 			//should add poi[me]=NULL;for execvp?becuase it needs a null-terminated
			if(pipe(p1)<0)
			{
				printf("Create pipe1 error!\n");
				exit(-3);
			}
			if(pipe(p2)<0)
			{
				printf("Create pipe2 error!\n");
				exit(-3);
			}
			//process forking
			if((child_pid = fork()) < 0)
			{
				printf("fork() error\n");
			}
			else if(child_pid == 0) //fork success child
			{
				if(strcmp(args[u], "|"))//if args[u]="|",the return value is 0,so the if statement is false
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
				fsync(p1[1]);

				dup(stdout_cp);

				if(wait_return = wait(&status) < 0)
					write(stdout_cp,"wait() error\n",13);

				if(args[i+1]!=NULL)
				{
					memset(buf, 0, sizeof(buf));
					read(p2[0], buf, sizeof(buf));
				}
				for(int me=0;me<=j-k;me++)
					free(poi[me]);
				k=u+1;
				free(poi);
			}
			//update index
		}
	}

	/*int p=0;

	while(args[p+1]!=NULL){

	if( (child_pid = fork()) < 0 ){
		printf("fork() error \n");
	}
	else if (child_pid == 0 ){
		if ( execvp(args[0], args) < 0){
			printf("execvp() error \n");
			exit(-1);
		}
		else
		{
			//pipes
		}
	}else{
		if ( (wait_return = wait(&status) ) < 0 )
			printf("wait() error \n");
	}
}*/


}

void shell_execute2(char **args, int argc)
{
	int counter=0;
	for(int i=0;i<argc;i++)
	{
		counter+=(strcmp(args[i],"|")==0)?1:0;
	}
	switch (counter) {
		case 0:
		case 1:
		case 2:
		default:
		printf("Input command out of bound!\n");
		exit(-99);
	}
}
