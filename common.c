#include <stdio.h>
#include <stdlib.h>
void error(char *err, char *detail)
{
	if(detail==NULL)
		detail="";
	
	printf("%s %s\n",err,detail);
	exit(EXIT_FAILURE);
}
