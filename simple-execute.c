#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#define MAX_ARG_NUM  10 
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

	int i=0;
	while(args[i]!=NULL)
	{
		printf("%s ",args[i]);
		i++;
	}
	printf("\n");

	printf("%d\n", argc);

	int k=0, j;
	for(int u=0;u<argc-1;u++) //detected SIGPIPE
	{
		printf("%s\n",(args[u]==NULL)?"someNULL":args[u]);
		if(strcmp(args[u],"|") == 0 || u==argc-2) //SIGSEGV fixed, keep argc-2 would be fine
		{
			j=u-1;
			poi = malloc((j-k+2)*sizeof(char*));
			for(int me = 0; me<=j-k; me++)
			{
				poi[me] = strdup(args[me+k]);
			}
			poi[j+1] = NULL;

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

int shell_execute2(char **args, int argc)
{
int counter=0,p1[2],p2[2],count=argc-1,ret,pid1,pid2;
    char ** poi1;
    char ** poi2;
    char ** poi3;
    char ** str1;
    char ** str2;
	char ** arg;
	for(int i=0;i<count;i++)
	{
		counter+=(strcmp(args[i],"|")==0)?1:0;
	}
	switch (counter) {
		case 0:
			arg=malloc(MAX_ARG_NUM*sizeof(char*));
            for(int j=0;j<argc;j++)
            {
                arg[j]=args[j];
            }
            if(execvp(arg[0],arg)<0)
            {
                printf("execvp error\n");
                exit(-1);
            }
            free(arg);
			break;
		case 1:
            str1=malloc(MAX_ARG_NUM*sizeof(char*));
            str2=malloc(MAX_ARG_NUM*sizeof(char*));
            int k=0;
            while(strcmp(args[k],"|")!=0)
            {
                str1[k]=args[k];
                k++;
            }
            str1[k]=NULL;
            k++;
            for(int l=0;k<argc;l++,k++)
            {
                str2[l]=args[k];
            }
            if(pipe(p1)<0)
            {
                printf("fail to pipe\n");
                exit(-3);
            }
            if((ret=fork())>0)
            {//parent
                close(1);
                dup(p1[1]);close(p1[0]);close(p1[1]);
                if(execvp(str1[0],str1)<0)
                {
                    printf("execvp error\n");
                    exit(-1);
                }
            }
            else if(ret==0)
            {//Child
                close(0);
                dup(p1[0]);close(p1[0]);close(p1[1]);
                if(execvp(str2[0],str2)<0)
                {
                    printf("execvp error\n");
                    exit(-1);
                }
            }
            else
            {
                printf("fork() error\n");
                exit(-1);
            }
            free(str1);
            free(str2);
			break;
		case 2:
            poi1=malloc(MAX_ARG_NUM*sizeof(char*));
            poi2=malloc(MAX_ARG_NUM*sizeof(char*));
            poi3=malloc(MAX_ARG_NUM*sizeof(char*));
            int m=0,n=0;
            while(strcmp(args[m],"|")!=0)
            {
                poi1[m]=args[m];
                m++;
            }
            poi1[m]=NULL;
            m++;
            while(strcmp(args[m],"|")!=0)
            {
                poi2[n]=args[m];
                n++;m++;
            }
            poi2[n]=NULL;
            m++;
            for(n=0;m<argc;m++,n++)
            {
                poi3[n]=args[m];
            }
            if(pipe(p1)<0)
            {
                printf("fail to pipe\n");
                exit(-3);
            }
            if(pipe(p2)<0)
            {
                printf("fail to pipe\n");
                exit(-3);
            }
            pid1=fork();
            if(pid1==0)
            {
                pid2=fork();
                if(pid2==0)
                {
                    //grand child
                    dup(p1[1]);
                    close(p1[1]);close(STDOUT_FILENO);//help me check whether close wrong std
                    close(p1[0]);close(p2[1]);close(p2[0]);
                    if(execvp(poi1[0],poi1)<0)
                    {
                        printf("execvp error\n");
                        exit(-1);
                    }
                }
                else
                {
                    //child
                    wait(&pid2);
                    close(p1[1]);
                    dup(p1[0]);dup(p2[1]);close(STDOUT_FILENO);//help me check whether close wrong
                    close(STDIN_FILENO);close(p1[0]);close(p2[1]);
                    close(p2[0]);
                    if(execvp(poi2[0],poi2)<0)
                    {
                        printf("execvp error\n");
                        exit(-1);
                    }
                }
                
            }
            else
            {
                //parent
		//should I wait here?
		//wait(&pid1);
                dup(p2[0]);close(STDIN_FILENO);close(p2[0]);//help me check..again
                close(p2[1]);close(p1[0]);close(p1[1]);
                if(execvp(poi3[0],poi3)<0)
                {
                    printf("execvp error\n");
                    exit(-1);
                }
            }
            free(poi1);
            free(poi2);
            free(poi3);
			break;
		default:
		printf("Input command out of bound!\n");
		exit(-99);
	}
    return 0;
}
