#include "stdio.h"

int	main (int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*c = " ";
	printf("%i\n", c[0]);
	printf("Lalala\n");
	return (0);
}