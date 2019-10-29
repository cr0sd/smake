#include<stdio.h>
#include<string.h>
#include"dir.c"
#include"file.c"

int main()
{
	// Allocate memory (because C)
	char*b=pwd();
	char*c=NULL;
	char*d=NULL;
	char e[512];

	// Open file called (( CWD + '\' + makefile ))
	strcpy(e,b);
	strcat(e,"\\makefile");
	c=filestring(e);

	// Tokenize string
	d=strtok(c," \n\t");
	do puts(d);
	while(d=strtok(NULL," \n\t"));

	// Free resources
	free(b);
	free(c);
}
