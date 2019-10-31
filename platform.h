#pragma once

#if defined(WINDOWS)
	#define SLASH "\\"
	#include<windows.h>
	#define getcwd(x,y) GetCurrentDirectory(y,x)
#else
	#define SLASH "/"
	#include<unistd.h> // For getcwd
#endif

std::string cwd()
{
	char b[512];

	getcwd(b,512);

	std::string s=b;
	return s;
}
