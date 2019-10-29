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
	char*drest=NULL;
	char cmd[1024]={0};
	char t[512];

	// Open file called (( CWD + '\' + makefile ))
	strcpy(e,b);
	strcat(e,"\\makefile");
	c=filestring(e);

	// Tokenize string
	drest=c;
	while(d=strtok_r(drest," \n\t",&drest))
	{
		if(d[strlen(d)-1]==':')
		{
			//printf("\n%s\n",d);
			//puts("----------");

			// Skip if no command
			if(*cmd==0)continue;

			// Run command
			system(cmd);
			printf("running '%s'\n",cmd);
			*cmd=0;
		}
		else
		{
			sprintf(t,"%s ",d);
			strcat(cmd,t);
			//printf("%s ",d);
		}
		
		
	}


	// Free resources
	free(b);
	free(c);
}
