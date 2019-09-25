#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#define MAX_LINE_SIZE 1024
#define MAX_ARG_NUM  10
#define BUFF_SIZE 100000
int shell_execute(char **args, int argc)
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
										close(STDOUT_FILENO);
										dup(p1[1]);
                    //dup2(p1[1],STDOUT_FILENO);
                    close(p1[1]);
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
                    //wait(&pid2);
										close(STDOUT_FILENO);
										dup(p1[0]);
										//dup2(p1[0],STDIN_FILENO);
										close(STDOUT_FILENO);
										dup(p2[1]);
                    //dup2(p2[1],STDOUT_FILENO);
                    close(p1[0]);close(p2[1]);close(p1[1]);
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
								close(STDIN_FILENO);
								dup(p2[0]);
                //dup2(p2[0],STDIN_FILENO);
                close(p2[0]);close(p2[1]);close(p1[0]);close(p1[1]);
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
