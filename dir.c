#include<windows.h>
#include<stdlib.h>
#include<stdio.h>

char*pwd()
{
	char*b=(char*)malloc(512);
	GetCurrentDirectory(512,b);
	return b;
}
