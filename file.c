#include<stdlib.h>
#include<stdio.h>

char*filestring(char*fn)
{
	FILE*f=fopen(fn,"r");
	char*b=NULL;
	int p=0;
	if(!f)return NULL;
	fseek(f,0,SEEK_END);
	b=malloc(ftell(f));
	b=calloc(1,ftell(f));
	fseek(f,0,SEEK_SET);
	while(!feof(f))
	{
		char c=fgetc(f);
		if(c!=EOF)b[p++]=c;
	}
	return b;
}
