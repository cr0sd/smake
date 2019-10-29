#include<stdio.h>
#include<string.h>
#include"dir.c"
#include"file.c"

void runcmd(char*cmd)
{
	puts("fake system call");
	return;
	printf("running '%s'\n",cmd);
	system(cmd);
	//*cmd=0;
}

int main(int argc,char**argv)
{
	// Allocate memory (because C)
	char*b=pwd();
	char*c=NULL;
	char*d=NULL;
	char e[512];
	char*drest=NULL;
	char cmd[1024]={0};
	char t[512];
	char tgt[512]={0};
	char label[512]={0};

	// Get CLI arguments
	strcpy(tgt,"all"); // Default value
	if(argc>=2)
		strcpy(tgt,argv[1]);
	//printf("argc:%d\n",argc);

	printf("TARGET:'%s'\n",tgt);

	// Open file called (( CWD + '\' + makefile ))
	strcpy(e,b);
	strcat(e,"\\makefile");
	c=filestring(e);

	// Tokenize string
	drest=c;
	//d=strtok_r(drest," \n\t",&drest);
	//do
	while(d=strtok_r(drest," \n\t",&drest))
	{
		if(d[strlen(d)-1]==':')
		{
			//printf("\n%s\n",d);
			//puts("----------");

			// Skip if no command
			if(*cmd==0)continue;

			// Run command
			d[strlen(d)-1]=0; // Clear colon
			strcpy(label,d);
			printf("TARGET IS NOW '%s'\n",label);
			//printf("comparing '%s' with '%s'\n",
			//if(strcmp(label,tgt)==0)
			{
				printf("matched target '%s'\n",label);
				runcmd(cmd);
				*cmd=0;
			}
		}
		else
		{
			sprintf(t,"%s ",d);
			strcat(cmd,t);
			//printf("%s ",d);
		}
	}

	//d[strlen(d)-1]=0; // Clear colon
	//if(*cmd && strcmp(label,tgt)==0)
	if(*cmd)
	{
		//puts("matched target");
		printf("matched target '%s'\n",label);
		runcmd(cmd);
		*cmd=0;
	}

	// Free resources
	free(b);
	free(c);
}
