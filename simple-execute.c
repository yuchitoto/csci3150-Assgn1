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

	int i=0;
	while(args[i]!=NULL)
	{
		printf("%s\n",args[i]);
		i++;
	}
	printf("%d\n",argc);

	if ( strcmp(args[0], "EXIT") == 0 )
		return -1;

	if( (child_pid = fork()) < 0 ){
		printf("fork() error \n");
	}else if (child_pid == 0 ){
		if ( execvp(args[0], args) < 0){
			printf("execvp() error \n");
			exit(-1);
		}
	}else{
		if ( (wait_return = wait(&status) ) < 0 )
			printf("wait() error \n");
	}

	return 0;

}
