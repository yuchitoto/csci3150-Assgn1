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

void shell_execute2(char **args, int argc);

int shell_execute(char ** args, int argc)
{
	int child_pid, wait_return, status;

	if ( strcmp(args[0], "EXIT") == 0 )
		return -1;

	shell_execute2(args, argc);

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

void shell_execute2(char **args, int argc)
{
	int counter=0, p1[2], p2[2], count=argc-1, pid1, pid2;
	int wait_return, status, child_pid;
	int m, n, k, t;
	char **tmp;
  char ** str1;
  char ** str2;
  char ** str3;
	char ** arg;
	for(int i=0;i<count;i++)
	{
		counter+=(strcmp(args[i],"|")==0)?1:0;
	}
	switch (counter) {
		case 0:
		if( (child_pid = fork()) < 0 ){
			printf("fork() error \n");
		}
		else if (child_pid == 0 ){
			if ( execvp(args[0], args) < 0){
				printf("execvp() error \n");
				exit(-1);
			}
		}else{
			if ( (wait_return = wait(&status) ) < 0 )
				printf("wait() error \n");
		}
			break;
		case 1:
						k=0;
						tmp = malloc(MAX_ARG_NUM*sizeof(char*));
						while(strcmp(args[k], "|") != 0)
						{
							tmp[k] = args[k];
							k++;
						}
						str1 = malloc(k*sizeof(char*));
						for(int i=0;i<k;i++)
						{
							str1[i] = args[i];
							//printf("%s\n", (args[i]!=NULL)?args[i]:"someNULL");
						}
            str1[k]=NULL;
						//printf("finished first copy\n");

            k++;
						str2 = malloc((argc-k+2)*sizeof(char*));
            for(int l=0;k<argc;l++,k++)
            {
                str2[l]=args[k];
            }
						//printf("finished second copy\n");

						if((child_pid=fork()) < 0)
						{
							printf("Fork error\n");
						}
						else if(child_pid == 0){
						if(pipe(p1)<0)
            {
                printf("Fail to create pipe\n");
                exit(-3);
            }
            if((pid1=fork())>0)
            {//parent
                close(STDOUT_FILENO);
                dup(p1[1]);
								close(p1[0]);
								close(p1[1]);
                if(execvp(str1[0],str1)<0)
                {
                    printf("execvp error\n");
                    exit(-1);
                }
            }
            else if(pid1==0)
            {//Child
                close(STDIN_FILENO);
                dup(p1[0]);
								close(p1[1]);
								close(p1[0]);
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
					}
					else
					if((wait_return = wait(&status)) < 0)
						printf("wait error");
			break;
		case 2:
            m=0;
						n=0;
						tmp = malloc(MAX_ARG_NUM*sizeof(char*));
            while(strcmp(args[m], "|") != 0)
            {
              tmp[m] = args[m];
              m++;
            }
						str1 = malloc(m*sizeof(char*));
						for(int i=0;i<m;i++)
							str1[i] = args[i];
            str1[m]=NULL;
            m++;

						//printf("finished first copy\n");
						t=m;

            while(strcmp(args[m], "|") != 0)
            {
              tmp[n]=args[m];
              n++;m++;
            }
						str2 = malloc(n*sizeof(char*));
						for(int i=0;i<n;i++,t++)
						{
							str2[i] = args[t];
						}
            str2[n]=NULL;
            m++;

						//printf("finished second copy\n");

						str3 = malloc((argc-m+1)*sizeof(char*));
            for(n=0;m<argc;m++,n++)
            {
              str3[n]=args[m];
            }
						//printf("finished third copy\n");

            if(pipe(p1)<0)
            {
              printf("Failed to create p1\n");
              exit(-3);
            }

						if(pipe(p2)<0)
						{
							printf("Failed to create p2\n");
							exit(-3);
						}

						if((pid1=fork())<0)
						{
							printf("1st fork error\n");
							exit(-2);
						}

            if(pid1==0)
            {
                if((pid2=fork())<0)
								{
									printf("2nd fork error\n");
									exit(-2);
								}
                if(pid2==0)
                {
                    //grand child

										close(p1[0]);
										close(STDOUT_FILENO);
										dup(p1[1]);
										close(p2[1]);
										close(p2[0]);

                    if(execvp(str1[0],str1)<0)
                    {
                        printf("execvp error\n");
                        exit(-1);
                    }
                }
                else
                {
                    //child

										close(p1[1]);
										close(STDIN_FILENO);
										dup(p1[0]);
										close(p2[0]);
										close(STDOUT_FILENO);
										dup(p2[1]);

                    if(execvp(str2[0],str2)<0)
                    {
                        printf("execvp error\n");
                        exit(-1);
                    }
                }

            }
            else
            {
                //parent
								close(p2[1]);
                close(STDIN_FILENO);
								dup(p2[0]);
								close(p1[1]);
								close(p1[0]);

                if(execvp(str3[0],str3)<0)
                {
                    printf("execvp error\n");
                    exit(-1);
                }
            }
            free(str1);
            free(str2);
            free(str3);
			break;
		default:
		printf("Input command out of bound!\n");
		exit(-99);
	}
}
