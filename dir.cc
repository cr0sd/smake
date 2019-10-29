#if defined(WINDOWS)
	#include<windows.h>
	//#include<direct.h>
	#define getcwd(x,y) GetCurrentDirectory(y,x)
#else
	#include<unistd.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<string>

std::string cwd()
{
	char b[512];

	// Linux
	getcwd(b,512);

	// Both
	std::string s=b;
	return s;
}
