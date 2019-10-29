#include<string>
#include<windows.h>
#include<stdlib.h>
#include<stdio.h>

std::string pwd()
{
	//char*b=(char*)malloc(512);
	//GetCurrentDirectory(512,b);
	//return b;
	char b[512];
	GetCurrentDirectory(512,b);
	std::string s=b;
	return s;
}
