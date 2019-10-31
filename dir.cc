#include<stdio.h>
#include<stdlib.h>
#include<string>
#include"platform.h"

std::string cwd()
{
	char b[512];

	getcwd(b,512);

	std::string s=b;
	return s;
}
