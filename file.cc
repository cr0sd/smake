#include<stdlib.h>
#include<stdio.h>

std::string filestring(const char*fn)
{
	FILE*f=fopen(fn,"r");
	char*b=NULL;
	int p=0;
	if(!f)
	{
		printf("failed to open '%s'\n",fn);
		exit(1);
		return NULL;
	}
	fseek(f,0,SEEK_END);
	//b=malloc(ftell(f));
	b=(char*)calloc(1,ftell(f));
	fseek(f,0,SEEK_SET);
	while(!feof(f))
	{
		char c=fgetc(f);
		if(c!=EOF)b[p++]=c;
	}
	std::string s=b;
	free(b);
	return s;
}
