
#include <stdio.h>
#include <stdlib.h>
#include "simple-execute.h"

int main(void)
{
	char **args =
	{
		"ls\0","-l\0","|\0",
		"grep\0","D\0","|\0",
		"wc\0","-l",
		NULL
	};
	int status = shell_execute(args,9);
	return 0;
}
