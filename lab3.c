
#include <stdio.h>
#include <stdlib.h>
#include "simple-execute.h"
#include "simple-shell.h"

int main(void)
{
	char **args;
	char *line = "ls -l | grep D | wc -l \0";
	int argc = get_line_args(line,args);
	int status = shell_execute(args,argc);
	return 0;
}
