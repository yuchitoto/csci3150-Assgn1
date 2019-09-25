#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#define MAX_LINE_SIZE 1024
#define MAX_ARG_NUM  10 
#define BUFF_SIZE 100000
int shell_execute(char ** args, int argc)
{
	int fd[2],fd1[2];
	int child_pid,ret, wait_return, status;
	int tpos=0;
	int lpos=0;
	int const stdin_cp=dup(STDIN_FILENO);
	int const stdout_cp=dup(STDOUT_FILENO);
	char pipe_buff[BUFF_SIZE]={0};
	int i,count;
	char ** arg;
	count=argc-1;
	if(strcmp(args[0],"EXIT") == 0)
		return -1;
	for(i=0;i<argc;i++)
	{
		printf("%d\n",i);
		if(strcmp(args[i],"|") == 0 || i == count)
		{
			printf("==| and arg-1\n");
			arg=malloc(MAX_ARG_NUM*sizeof(char*));
			int j=0;
			lpos=i;
			for(;j<lpos-tpos;j++)
			{
				arg[j]=strdup(args[j+tpos]);
				printf("%s\n",arg[j]);
			}
			arg[j]=NULL;
			if(pipe(fd)<0){
				printf("fail to pipe\n");
				exit(-3);
			}
			if(pipe(fd1)<0){
				printf("faile to pipe2\n");
				exit(-3);
			}
			//signal(SIGCHLD,SIG_IGN);
			if((child_pid=fork())==0)
			{//child
				printf("CHILD\n");
				if(strcmp(args[i],"|"))
				{
					printf("LAST ONE\n");
					close(fd[1]);
					close(STDIN_FILENO);
					dup(fd[0]);
					close(fd1[0]);
					close(STDOUT_FILENO);
					dup(fd1[1]);
					if(execvp(arg[0],arg)<0)
					{
						printf("execvp error\n");
						exit(-1);
					}
					else
						exit(0);
				}
				else
				{
					printf("NOT THE LAST\n");
					close(fd[1]);
					close(STDIN_FILENO);
					dup(fd[0]);
					close(fd[0]);
					close(fd1[1]);
					close(fd1[0]);
					if(execvp(arg[0],arg)<0)
					{
						printf("execvp error\n");
					}
					else
						exit(0);
				}
			}
			else
			{//parent
				printf("PARENT\n");
				close(fd[0]);
				close(STDOUT_FILENO);
				dup(fd[1]);
				close(fd[1]);
				write(fd[1],pipe_buff,BUFF_SIZE);
				fsync(fd[1]);
				dup(stdout_cp);
				if((wait_return=wait(&status)))
				{
					write(stdout_cp,"wait() error\n",13);   //wait error
				}
				if(args[argc+1]!=NULL)
				{
					memset(pipe_buff,0,sizeof(pipe_buff));
					read(fd1[0],pipe_buff,sizeof(pipe_buff));
				}
				for(int me=0;me<lpos-tpos;me++)
					free(arg[me]);
				tpos=lpos++;
				free(arg);
			}
		}
	}
	return 0;

}
