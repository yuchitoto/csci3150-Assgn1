#include <stdio.h>
#include <stdlib.h>
#include "simple-execute.h"

int main(void)
{
	char *args[9] = {"ls","-l","|","grep","D","|","wc","-l",NULL};
	for(int i=0;i<9;i++)
	//printf("%s\n",(args[i]!=NULL)?args[i]:"someNULL");
	int status = shell_execute(args,9);
	return 0;
}
