#include <stdio.h>
#include <stdlib.h>
#include "simple-execute.h"
#include "simple-shell2.h"

int main(void)
{
	char **args;
	char *line = "ls -l | grep D | wc -l \0";
	int argc = get_line_args(line,args);
	printf("%d\n",argc);
	for(int i=0;i<argc;i++)
	printf("%s\n",(args[i]!=NULL)?args[i]:"someNULL");
	int status = shell_execute(args,argc);
	return 0;
}
